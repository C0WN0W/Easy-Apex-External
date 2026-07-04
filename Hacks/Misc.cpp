#include "Misc.hpp"

void Misc::BunnyHop(uintptr_t localPlayer)
{
	if (!cheatConfig::BunnyHop) return;

	if (IsKeyDown(cheatConfig::BunnyHopKey) && IsKeyDown(VK_SPACE))
	{
		if (IsOnGround(localPlayer))
		{
			drv::Write<int>(Global::GameBase + Offset::Buttons::Jump + 0x8, 5);
			Sleep(5);
			drv::Write<int>(Global::GameBase + Offset::Buttons::Jump + 0x8, 4);
		}
	}
}

void Misc::AutoGrapple(uintptr_t localPlayer)
{
	if (!cheatConfig::AutoGrapple) return;

	bool Gn = drv::Read<bool>(localPlayer + Offset::Grapple::Grapple + Offset::Grapple::GrappleAttached);
	if (Gn)
	{
		drv::Write<int>(Global::GameBase + Offset::Buttons::Jump + 0x8, 6);
	}
}

void Misc::SuperGlideJump(uintptr_t localPlayer)
{
	int sleep_time = 8;
	try
	{
		if (Global::current_fps != 0)
		{
			sleep_time = 1200 / Global::current_fps;
		}
	}
	catch (...) {
	}

	drv::Write<int>(Global::GameBase + Offset::Buttons::Jump + 0x8, 4);
	drv::Write<int>(Global::GameBase + Offset::Buttons::Jump + 0x8, 5);
	Sleep(sleep_time);
	drv::Write<int>(Global::GameBase + Offset::Buttons::Duck + 0x8, 5);
	Sleep(220);
	drv::Write<int>(Global::GameBase + Offset::Buttons::Jump + 0x8, 4);
	drv::Write<int>(Global::GameBase + Offset::Buttons::Duck + 0x8, 4);
}

void Misc::SuperGlide(uintptr_t localPlayer)
{
	if (!cheatConfig::SuperGlideJump) return;

	static bool sg_pressed = false;
	float progress = drv::Read<float>(localPlayer + Offset::Player::TraversalProgress);

	if (progress > 0.94) {
		if (!sg_pressed) {
			if (IsKeyDown(cheatConfig::SuperGlideJumpKey))
			{
				SuperGlideJump(localPlayer);
			}
			sg_pressed = true;
		}
	}
	else {
		sg_pressed = false;
	}
}

void Misc::TapStrafe(uintptr_t localPlayer)
{
	return;
}