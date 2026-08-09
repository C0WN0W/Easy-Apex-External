#include "Triggerbot.hpp"
#include <chrono>
#include <thread>

void triggerbot::run(uint64_t localPtr, uint64_t EntPtr)
{
    if (!cheatConfig::Triggerbot)
        return;

    if (GetAsyncKeyState(cheatConfig::TriggerbotKey) & 0x8000)
    {
        bool IsAimed = IsInCrossHair(localPtr, EntPtr);
        if (!IsAimed)
            return;

		float distance = GetDistance(GetPosition(localPtr), GetPosition(EntPtr));
		if (distance > cheatConfig::TriggerbotDistance)
			return;

        if (cheatConfig::TriggerbotDelay > 0)
            std::this_thread::sleep_for(std::chrono::milliseconds(cheatConfig::TriggerbotDelay));

        drv::Write<int>(Global::GameBase + Offset::Buttons::Attack + 0x8, 5);
        Sleep(1);
        drv::Write<int>(Global::GameBase + Offset::Buttons::Attack + 0x8, 4);
    }
}
