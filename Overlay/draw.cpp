#include "Draw.hpp"

void Draw::DrawBox(float x1, float y1, float x2, float y2, ULONG color, float Width, bool outline) {
    if (outline) {
        ImGui::GetBackgroundDrawList()->AddRect(
            ImVec2(x1, y1),
            ImVec2(x2, y2),
            IM_COL32(0, 0, 0, 255),
            0.0f,
            0,
            Width * 2
        );
    }
    ImGui::GetBackgroundDrawList()->AddRect(
        ImVec2(x1, y1),
        ImVec2(x2, y2),
        color,
        0.0f,
        0,
        Width
    );
}

void Draw::DrawLine(ImVec2 Start, ImVec2 End, ImColor color, float thickness, bool outline)
{
    if (outline) {
        ImGui::GetBackgroundDrawList()->AddLine(
            Start,
            End,
            IM_COL32(0, 0, 0, 255),
            thickness * 2
        );
    }
    ImGui::GetBackgroundDrawList()->AddLine(
        Start,
        End,
        color,
        thickness
    );
}

void Draw::DrawBox(ImVec2 Start, ImVec2 End, ImColor color, float thickness, float rounding)
{
    ImGui::GetBackgroundDrawList()->AddRect(
        Start, 
        ImVec2(Start.x + End.x, Start.y + End.y), 
        color, 
        rounding, 
        0, 
        thickness
    );
}

void Draw::DrawHealthBar(ImVec2 Start, ImVec2 End, int currentHealth, int MaxHealth, float width)
{
    if (MaxHealth <= 0) return;

    currentHealth = max(0, min(currentHealth, MaxHealth));

    float targetHealth = (float)currentHealth / (float)MaxHealth * 255.f;
    float col_r = 255.f - targetHealth;
    float col_g = targetHealth;
    float col_b = 0.f;

    float boxHeight = End.y - Start.y;

    ImVec2 barStart = ImVec2(Start.x - width - 1, Start.y);
    ImVec2 barEnd = ImVec2(Start.x - 1, End.y);

    float healthPercent = (float)currentHealth / (float)MaxHealth;
    float fillHeight = boxHeight * healthPercent;

    ImVec2 fillStart = ImVec2(barStart.x, barEnd.y - fillHeight);
    ImVec2 fillEnd = ImVec2(barEnd.x, barEnd.y);

    auto drawList = ImGui::GetBackgroundDrawList();

    drawList->AddRect(barStart, barEnd, IM_COL32(0, 0, 0, 255), 0, 0, 1.5f);
    drawList->AddRectFilled(barStart, barEnd, IM_COL32(10, 10, 10, 200));

    if (fillHeight > 0) {
        drawList->AddRectFilled(fillStart, fillEnd, IM_COL32((int)col_r, (int)col_g, (int)col_b, 255));
    }
}

void Draw::DrawArmorBar(ImVec2 Start, ImVec2 End, int currentArmor, int MaxArmor, float width)
{
    if (MaxArmor <= 0) return;

    currentArmor = max(0, min(currentArmor, MaxArmor));

    float boxWidth = End.x - Start.x;
    float boxHeight = End.y - Start.y;

    ImVec2 barStart = ImVec2(End.x + 1, Start.y);
    ImVec2 barEnd = ImVec2(End.x + width + 1, End.y);

    float armorPercent = (float)currentArmor / (float)MaxArmor;
    float fillHeight = boxHeight * armorPercent;

    ImVec2 fillStart = ImVec2(barStart.x, barEnd.y - fillHeight);
    ImVec2 fillEnd = ImVec2(barEnd.x, barEnd.y);

    ImColor colorLightGray = ImColor(247, 247, 247, 255);
    ImColor colorSkyBlue = ImColor(39, 178, 255, 255);
    ImColor colorPurple = ImColor(206, 59, 255, 255);
    ImColor colorRed = ImColor(219, 2, 2, 255);

    // If you have extra armor, this will be inaccurate,
    // You can read the m_armorType to judge.
    ImColor armorColor;
    if (MaxArmor >= 125) {
        armorColor = colorRed;
    }
    else if (MaxArmor >= 100) {
        armorColor = colorPurple;
    }
    else if (MaxArmor >= 75) {
        armorColor = colorSkyBlue;
    }
    else {
        armorColor = colorLightGray;
    }

    auto drawList = ImGui::GetBackgroundDrawList();

    drawList->AddRect(barStart, barEnd, IM_COL32(0, 0, 0, 255), 0, 0, 1.5f);
    drawList->AddRectFilled(barStart, barEnd, IM_COL32(30, 30, 30, 200));

    if (fillHeight > 0) {
        drawList->AddRectFilled(fillStart, fillEnd, armorColor);
    }
}