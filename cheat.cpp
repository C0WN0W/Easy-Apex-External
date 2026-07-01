#define IMGUI_DEFINE_MATH_OPERATORS
#include "cheat.hpp"
#include "Hacks/skeleton.hpp"
#include "Overlay/menu.hpp"
#include <cstring>

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

	constexpr size_t BASE_OFF = 0x170;
	constexpr size_t BASE_SIZE = 0x524; // 0x694 - 0x170
	constexpr size_t VIS_OFF = 0x1A74;
	constexpr size_t VIS_SIZE = 0x10;
	constexpr size_t KNOCK_OFF = 0x27E0;
	constexpr size_t KNOCK_SIZE = 4;
	constexpr size_t LOCAL_STATE_OFF = 0x1CC1;
	constexpr size_t LOCAL_STATE_SIZE = 0x100;
	constexpr size_t LOCAL_TIME_OFF = 0x2178;
	constexpr size_t LOCAL_TIME_SIZE = 4;
	constexpr int ENTITY_BATCH = 64;

	alignas(64) char localBase[BASE_SIZE];
	alignas(64) char localState[LOCAL_STATE_SIZE];
	alignas(64) char localTime[LOCAL_TIME_SIZE];
	alignas(64) char entBase[BASE_SIZE];
	alignas(64) char entVis[VIS_SIZE];
	alignas(64) char entKnock[KNOCK_SIZE];
	uint64_t entBatch[ENTITY_BATCH];

	while (running.load(std::memory_order_acquire))
	{
		uintptr_t localPtr = GetLocalPlayerPtr();
		if (!localPtr)
		{
			backBuffer.load(std::memory_order_relaxed)->players.clear();
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			continue;
		}

		if (!drv::ReadRange(localPtr + BASE_OFF, localBase, BASE_SIZE)) continue;
		drv::ReadRange(localPtr + LOCAL_STATE_OFF, localState, LOCAL_STATE_SIZE);
		drv::ReadRange(localPtr + LOCAL_TIME_OFF, localTime, LOCAL_TIME_SIZE);

		Player local;
		local.Ptr = localPtr;
		local.Position = *reinterpret_cast<Vector3*>(localBase + (Offset::Player::VecAbsOrigin - BASE_OFF));
		local.TeamID = *reinterpret_cast<int*>(localBase + (Offset::Player::TeamNum - BASE_OFF));
		local.Zoomed = *reinterpret_cast<int*>(localState + (Offset::Player::Zooming - LOCAL_STATE_OFF));
		local.CameraPos = *reinterpret_cast<Vector3*>(localState + (Offset::Player::CameraPos - LOCAL_STATE_OFF));

		float localTimeBase = *reinterpret_cast<float*>(localTime);

		FrameData* working = backBuffer.load(std::memory_order_relaxed);
		working->players.clear();
		working->localPlayer = local;

		std::string levelName = GetLevelName();
		int loopSize = (levelName == "mp_rr_canyonlands_staging_mu1") ? 10000 : 64;

		for (int batchStart = 0; batchStart < loopSize; batchStart += ENTITY_BATCH)
		{
			int batchEnd = batchStart + ENTITY_BATCH;
			if (batchEnd > loopSize) batchEnd = loopSize;
			int batchCount = batchEnd - batchStart;

			uintptr_t listAddr = Global::GameBase + Offset::Misc::EntityList + ((uint64_t)batchStart << 5);
			if (!drv::ReadRange(listAddr, entBatch, batchCount * sizeof(uint64_t))) continue;

			for (int j = 0; j < batchCount; j++)
			{
				uintptr_t ent = entBatch[j];
				if (!ent || ent == localPtr || !drv::IsValidPointer(ent)) continue;

				if (!drv::ReadRange(ent + BASE_OFF, entBase, BASE_SIZE)) continue;
				drv::ReadRange(ent + VIS_OFF, entVis, VIS_SIZE);
				drv::ReadRange(ent + KNOCK_OFF, entKnock, KNOCK_SIZE);

				uint64_t signifierPtr = *reinterpret_cast<uint64_t*>(entBase + (Offset::Player::SignifierName - BASE_OFF));
				if (!signifierPtr) continue;
				StrBuf32 signifierBuf;
				if (!drv::ReadBulk(signifierPtr, signifierBuf)) continue;
				std::string signifier(signifierBuf.buf, strnlen(signifierBuf.buf, 32));
				if (signifier != "npc_dummie" && signifier != "player") continue;

				Vector3 entPos = *reinterpret_cast<Vector3*>(entBase + (Offset::Player::VecAbsOrigin - BASE_OFF));
				float dist = GameUnitsToMeters(GetDistance(local.Position, entPos));
				if (dist > cheatConfig::RenderDistance) continue;

				int entTeamID = *reinterpret_cast<int*>(entBase + (Offset::Player::TeamNum - BASE_OFF));
				if (entTeamID == local.TeamID) continue;

				int lifeState = *reinterpret_cast<int*>(entBase + (Offset::Player::LifeState - BASE_OFF));
				if (lifeState != 0) continue;

				float lastVisibleTime = *reinterpret_cast<float*>(entVis);
				bool isVisible = lastVisibleTime > localTimeBase - 0.25f;

				int bleedoutState = *reinterpret_cast<int*>(entKnock);

				Player p;
				p.Ptr = ent;
				p.Position = entPos;
				p.ToDistance = dist;
				p.IsVisible = isVisible;
				p.Health = *reinterpret_cast<int*>(entBase + (Offset::Player::Health - BASE_OFF));
				p.Shield = *reinterpret_cast<int*>(entBase + (Offset::Player::ShieldHealth - BASE_OFF));
				p.MaxShield = *reinterpret_cast<int*>(entBase + (Offset::Player::ShieldHealthMax - BASE_OFF));
				p.IsKnocked = bleedoutState;
				p.Velocity = *reinterpret_cast<Vector3*>(entBase + (Offset::Player::Velocity - BASE_OFF));

				working->players.push_back(p);
			}
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
