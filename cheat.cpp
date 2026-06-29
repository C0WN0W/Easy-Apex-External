#define IMGUI_DEFINE_MATH_OPERATORS
#include "cheat.hpp"

std::mutex Cheat::taskMutex;
std::condition_variable Cheat::taskCV;
std::atomic<bool> Cheat::running{ true };

namespace GlobalBuffer {
	std::vector<Player> poolA;
	std::vector<Player> poolB;

	std::atomic<std::vector<Player>*> frontBuffer{ &poolA };

	std::atomic<std::vector<Player>*> backBuffer{ &poolB };

	Player localPlayerSnapshot;
}

void Cheat::MatrixUpdater()
{
	uint64_t viewRender = drv::Read<uint64_t>(Global::GameBase + Offset::Misc::ViewRender);
	uint64_t viewMatrixPtr = drv::Read<uint64_t>(viewRender + Offset::Misc::ViewMatrix);

	while (running)
	{
		Global::ViewMatrix = drv::Read<Matrix>(viewMatrixPtr);
		Global::MatrixReady = true;

		std::this_thread::yield();
	}
}

void Cheat::WorkerThread()
{
	while (running)
	{
		uintptr_t localPtr = GetLocalPlayerPtr();
		if (!localPtr)
		{
			GlobalBuffer::frontBuffer.load()->clear();
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			continue;
		}

		Player local;
		local.Ptr = localPtr;
		local.Position = GetPosition(localPtr);
		local.TeamID = GetTeamID(localPtr);
		local.Zoomed = drv::Read<int>(localPtr + Offset::Player::Zooming);
		local.CameraPos = drv::Read<Vector3>(localPtr + Offset::Player::CameraPos);
		GlobalBuffer::localPlayerSnapshot = local;

		std::vector<Player>* workingList = GlobalBuffer::backBuffer.load();
		workingList->clear();

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

			workingList->push_back(p);
		}
		GlobalBuffer::backBuffer.store(GlobalBuffer::frontBuffer.exchange(workingList));

		std::this_thread::sleep_for(std::chrono::nanoseconds(1));
	}
}