#define IMGUI_DEFINE_MATH_OPERATORS
#include "cheat.hpp"
#include "Hacks/skeleton.hpp"
#include "Overlay/menu.hpp"

std::atomic<bool> Cheat::running{ true };

struct FrameData {
	std::vector<Player> players;
	Player localPlayer;
};

static FrameData poolA;
static FrameData poolB;

static std::atomic<FrameData*> frontBuffer{ &poolA };
static std::atomic<FrameData*> backBuffer{ &poolB };

static Matrix g_viewPool[2];
static std::atomic<Matrix*> g_viewPtr{ &g_viewPool[0] };

Matrix Cheat::GetViewMatrix()
{
	return *g_viewPtr.load(std::memory_order_acquire);
}

void Cheat::MatrixUpdater()
{
	uint64_t viewRender = drv::Read<uint64_t>(Global::GameBase + Offset::Misc::ViewRender);
	uint64_t viewMatrixPtr = drv::Read<uint64_t>(viewRender + Offset::Misc::ViewMatrix);

	while (running.load(std::memory_order_acquire))
	{
		Matrix* current = g_viewPtr.load(std::memory_order_relaxed);
		Matrix* next = (current == &g_viewPool[0]) ? &g_viewPool[1] : &g_viewPool[0];
		*next = drv::Read<Matrix>(viewMatrixPtr);
		g_viewPtr.store(next, std::memory_order_release);

		std::this_thread::yield();
	}
}

void Cheat::WorkerThread()
{
	while (running.load(std::memory_order_acquire))
	{
		uintptr_t localPtr = GetLocalPlayerPtr();
		if (!localPtr)
		{
			backBuffer.load(std::memory_order_relaxed)->players.clear();
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			continue;
		}

		Player local;
		local.Ptr = localPtr;
		local.Position = GetPosition(localPtr);
		local.TeamID = GetTeamID(localPtr);
		local.Zoomed = drv::Read<int>(localPtr + Offset::Player::Zooming);
		local.CameraPos = drv::Read<Vector3>(localPtr + Offset::Player::CameraPos);

		FrameData* working = backBuffer.load(std::memory_order_relaxed);
		working->players.clear();
		working->localPlayer = local;

		std::string levelName = GetLevelName();
		int loopSize = (levelName == "mp_rr_canyonlands_staging_mu1") ? 10000 : 64;

		for (int i = 0; i < loopSize; i++)
		{
			uintptr_t ent = GetEntityPtr(i);
			if (!ent || ent == localPtr || !drv::IsValidPointer(ent)) continue;

			std::string signifier = GetSignifier(ent);
			if (signifier != "npc_dummie" && signifier != "player") continue;

			Vector3 entPos = GetPosition(ent);
			float dist = GameUnitsToMeters(GetDistance(local.Position, entPos));
			if (dist > cheatConfig::RenderDistance) continue;

			if (GetTeamID(ent) == local.TeamID) continue;

			if (drv::Read<int>(ent + Offset::Player::LifeState) != 0) continue;

			Player p;
			p.Ptr = ent;
			p.Position = entPos;
			p.ToDistance = dist;
			p.IsVisible = GetVisible(localPtr, ent);
			p.Health = drv::Read<int>(ent + Offset::Player::Health);
			p.Shield = drv::Read<int>(ent + Offset::Player::ShieldHealth);
			p.MaxShield = drv::Read<int>(ent + Offset::Player::ShieldHealthMax);
			p.IsKnocked = GetKnocked(ent);
			p.Velocity = drv::Read<Vector3>(ent + Offset::Player::Velocity);

			working->players.push_back(p);
		}

		FrameData* oldFront = frontBuffer.exchange(working, std::memory_order_acq_rel);
		backBuffer.store(oldFront, std::memory_order_release);

		std::this_thread::sleep_for(std::chrono::nanoseconds(1));
	}
}

void Cheat::Run()
{
	FrameData* renderData = frontBuffer.load(std::memory_order_acquire);
	Player localPlayer = renderData->localPlayer;
	Matrix viewMatrix = GetViewMatrix();

	for (const auto& plyer : renderData->players)
	{
		ImVec4 box = CalcRect(plyer, viewMatrix);
		if (!IsBoxValid(box)) continue;

		if (cheatConfig::BoxESP)
		{
			if (plyer.IsKnocked && !cheatConfig::KnockESP) continue;

			Draw::DrawBox(box.x, box.y, box.z, box.w,
				plyer.IsVisible ? cheatConfig::BoxCol_vis : cheatConfig::BoxCol_unvis,
				1.f, cheatConfig::Outline);
		}

		if (cheatConfig::LineESP)
		{
			if (plyer.IsKnocked && !cheatConfig::KnockESP)
				continue;

			Draw::DrawLine(ImVec2((box.x + box.z) / 2.f, box.w),
				ImVec2(Global::SightCenter.x, Global::ScreenSize.y), cheatConfig::LineCol,
				1.f, cheatConfig::Outline);
		}

		if (cheatConfig::HealthESP)
			Draw::DrawHealthBar({ box.x, box.y }, { box.z, box.w }, plyer.Health, 100);
		if (cheatConfig::ArmorESP)
			Draw::DrawArmorBar({ box.x, box.y }, { box.z, box.w }, plyer.Shield, plyer.MaxShield);

		if (cheatConfig::BoneESP)
		{
			Skeleton::DrawSkeleton(plyer.Ptr, viewMatrix,
				plyer.IsKnocked ? cheatConfig::BoneCol_Knock : (plyer.IsVisible ? cheatConfig::BoneCol : cheatConfig::BoneCol_Unvis));
		}
	}

	static DWORD lastTick = 0;
	DWORD currentTick = GetTickCount();
	if (((GetAsyncKeyState(VK_INSERT) & 0x8000) || (GetAsyncKeyState(VK_DELETE) & 0x8000)) && currentTick - lastTick >= 250)
	{
		Global::MenuOpen = !Global::MenuOpen;
		lastTick = currentTick;
	}
	if (Global::MenuOpen)
		GUI::DrawMenu();
}

ImVec4 Cheat::CalcRect(Player entity, Matrix m)
{
	Vector3 bs, hs;
	Vector3 HeadPosition = NewHitbox(entity.Ptr, 0);
	HeadPosition.z += 12.f;
	world_to_screen(entity.Position, m, Global::ScreenSize.x, Global::ScreenSize.y, bs);
	world_to_screen(HeadPosition, m, Global::ScreenSize.x, Global::ScreenSize.y, hs);

	float height = fabsf(fabsf(hs.y) - fabsf(bs.y));
	float width = height / 2.f;
	float boxLeft = bs.x - (width / 2);
	float boxRight = bs.x + (width / 2);
	int widthC = (int)boxRight - (int)boxLeft;
	int x1, y1, x2, y2;
	x1 = (int)boxLeft; y1 = (int)hs.y; x2 = (int)boxRight; y2 = (int)bs.y;

	return ImVec4((int)boxLeft, (int)hs.y, (int)boxRight, (int)bs.y);
}

bool Cheat::IsBoxValid(ImVec4 box)
{
	if (box.x == 0 && box.z == 0) return false;
	if (box.y == 0 && box.w == 0) return false;
	return true;
}
