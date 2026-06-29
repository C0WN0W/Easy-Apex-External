#pragma once
#include <OS-ImGui.h>

namespace GUI
{
	static void menuBarItem(const char* name, bool& enabled) noexcept;
	void DrawMenu();

	struct {
		bool esp = false;
		bool glow = false;
		bool aimbot = false;
		bool triggerbot = false;
		bool rcs = false;
		bool misc = false;
		bool dumper = false;
	} window;
}
