#include "menu.hpp"
#include "../settings.hpp"

DWORD picker_flags = ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaPreview;

char field[45] = { "" };

static void GUI::menuBarItem(const char* name, bool& enabled) noexcept
{
    if (ImGui::MenuItem(name)) {
        enabled = true;
        ImGui::SetWindowFocus(name);
        ImGui::SetWindowPos(name, { 100.0f, 100.0f });
    }
}

void GUI::DrawMenu()
{
    if (ImGui::BeginMainMenuBar())
    {
        menuBarItem("ESP", window.esp);
        menuBarItem("Glow", window.glow);
        menuBarItem("Aimbot", window.aimbot);
        menuBarItem("Triggerbot", window.triggerbot);
        menuBarItem("Recoil Control", window.rcs);
        menuBarItem("Miscs", window.misc);
        menuBarItem("Dumper", window.dumper);
        ImGui::EndMainMenuBar();
    }
}