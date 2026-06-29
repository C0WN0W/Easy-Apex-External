#pragma once
#include <OS-ImGui.h>

namespace Draw
{
	void DrawBox(float x1, float y1, float x2, float y2, ULONG color, float strokeWidth, bool outline = false);
	void DrawBox(ImVec2 Start, ImVec2 End, ImColor color, float thickness = 1.2f, float rounding = 0.f);
	void DrawLine(ImVec2 Start, ImVec2 End, ImColor color, float thickness = 1.0f, bool outline = false);

	void DrawHealthBar(ImVec2 Start, ImVec2 End, int currentHealth, int MaxHealth, float width = 3.f);
	void DrawArmorBar(ImVec2 Start, ImVec2 End, int currentArmor, int MaxArmor, float width = 3.f);
}