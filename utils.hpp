#pragma once
#include "offsets.hpp"
#include "settings.hpp"

// Pos to Box data
inline bool W2S(const Vector3& entityPos, RectData& box, float& scale,
    const Matrix& mat)
{
    float ViewW = mat.matrix[3 * 4 + 0] * entityPos.x +
        mat.matrix[3 * 4 + 1] * entityPos.y +
        mat.matrix[3 * 4 + 2] * entityPos.z +
        mat.matrix[3 * 4 + 3];

    if (ViewW < 0.01f)
        return false;

    scale = 1.0f / ViewW;

    box.X = Global::SightCenter.x + (mat.matrix[0 * 4 + 0] * entityPos.x +
        mat.matrix[0 * 4 + 1] * entityPos.y +
        mat.matrix[0 * 4 + 2] * entityPos.z +
        mat.matrix[0 * 4 + 3]) * scale * Global::SightCenter.x;

    box.Y = Global::SightCenter.y - (mat.matrix[1 * 4 + 0] * entityPos.x +
        mat.matrix[1 * 4 + 1] * entityPos.y +
        mat.matrix[1 * 4 + 2] * (entityPos.z + 75.0f) +
        mat.matrix[1 * 4 + 3]) * scale * Global::SightCenter.y;

    float Y1 = Global::SightCenter.y - (mat.matrix[1 * 4 + 0] * entityPos.x +
        mat.matrix[1 * 4 + 1] * entityPos.y +
        mat.matrix[1 * 4 + 2] * entityPos.z +
        mat.matrix[1 * 4 + 3]) * scale * Global::SightCenter.y;

    box.H = Y1 - box.Y;

    return true;
}

// 3D to 2D, return true if valid
inline bool world_to_screen(Vector3 from, Matrix m, int targetWidth, int targetHeight, Vector3& to)
{
    float w = m.matrix[12] * from.x + m.matrix[13] * from.y + m.matrix[14] * from.z + m.matrix[15];

    if (w < 0.01f) {
        return false;
    }

    float x = m.matrix[0] * from.x + m.matrix[1] * from.y + m.matrix[2] * from.z + m.matrix[3];
    float y = m.matrix[4] * from.x + m.matrix[5] * from.y + m.matrix[6] * from.z + m.matrix[7];

    float invw = 1.0f / w;
    x *= invw;
    y *= invw;

    to.x = (targetWidth / 2.0f) + (targetWidth / 2.0f) * x;
    to.y = (targetHeight / 2.0f) - (targetHeight / 2.0f) * y;
    to.z = 0;
    return true;
}

// Return 2D
inline Vector2 WorldToScreen(const Vector3& pos, const Matrix& m, float width, float height) {
    Vector2 screen;

    float w = m.matrix[12] * pos.x + m.matrix[13] * pos.y + m.matrix[14] * pos.z + m.matrix[15];

    if (w < 0.01f) {
        return Vector2(-1, -1);
    }

    float x = m.matrix[0] * pos.x + m.matrix[1] * pos.y + m.matrix[2] * pos.z + m.matrix[3];
    float y = m.matrix[4] * pos.x + m.matrix[5] * pos.y + m.matrix[6] * pos.z + m.matrix[7];

    screen.x = (width / 2.0f) + (width / 2.0f) * x / w;
    screen.y = (height / 2.0f) - (height / 2.0f) * y / w;

    return screen;
}

inline float GameUnitsToMeters(float units)
{
    return units / 39.37007874;
}

inline float Distance2(float x1, float y1, float x2, float y2)
{
    float dx = x1 - x2;
    float dy = y1 - y2;
    return std::sqrt(dx * dx + dy * dy);
}


inline std::string ReadStr32(uint64_t src)
{
    StrBuf32 ret = drv::Read<StrBuf32>(src);
    return std::string::basic_string(ret.buf);
}

inline int GetMonitorRefreshRate()
{
    DEVMODE dm = { 0 };
    dm.dmSize = sizeof(dm);
    if (EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm))
    {
        return dm.dmDisplayFrequency;
    }
    return 120; // default in 120Hz
}

inline bool DelayPlus(DWORD& lastTick, DWORD delayMs)
{
    DWORD currentTick = GetTickCount();
    if (currentTick - lastTick >= delayMs)
    {
        lastTick = currentTick;
        return true;
    }
    return false;
}

inline void updateFPS()
{
    CGlobalVars vars = drv::Read<CGlobalVars>(Global::GameBase + Offset::Misc::GlobalVars);
    static float framerate = 0;
    framerate = 0.9 * framerate + (1.0 - 0.9) * vars.absoluteframetime;
    Global::current_fps = int(1.f / framerate);
}

inline bool IsKeyDown(int VK)
{
    return (GetAsyncKeyState(VK) & 0x8000) != 0;
}

namespace Math
{
#define XM_PI 3.141592654f
#define XM_2PI 6.283185307f
#define XM_1DIVPI 0.318309886f
#define XM_1DIV2PI 0.159154943f
#define XM_PIDIV2 1.570796327f
#define XM_PIDIV4 0.785398163f
#define DegToRad(val1) ((val1) * (XM_PI / 180))

    inline Vector2 CalcAngle(Vector3 src, Vector3 dst)
    {
        Vector2 angle = {};
        Vector3 delta = (src - dst);
        double hyp = sqrt(delta.x * delta.x + delta.y * delta.y);
        angle.x = atan(delta.z / hyp) * (180.0f / PI);
        angle.y = atan(delta.y / delta.x) * (180.0f / PI);
        if (delta.x >= 0.0) angle.y += 180.0f;
        return angle;
    }

    inline void NormalizeAngles(Vector2& angle) {
        int maxiter = 1000;
        while (angle.x > 89.0f && maxiter > 0) {
            angle.x -= 180.f;
            maxiter--;
        }
        maxiter = 1000;
        while (angle.x < -89.0f && maxiter > 0) {
            angle.x += 180.f;
            maxiter--;
        }
        maxiter = 1000;
        while (angle.y > 180.f && maxiter > 0) {
            angle.y -= 360.f;
            maxiter--;
        }
        maxiter = 1000;
        while (angle.y < -180.f && maxiter > 0) {
            angle.y += 360.f;
            maxiter--;
        }
    }

    inline void Smooth(Vector2* Delta, float smooth_multiplier) {
        Delta->x /= (smooth_multiplier);
        Delta->y /= (smooth_multiplier);
    }
}