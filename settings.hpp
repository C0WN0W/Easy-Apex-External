#pragma once
#include "structs.hpp"
#include "OS-ImGui/imgui/imgui.h"
#include <atomic>

namespace Global
{
	inline uintptr_t GameBase = 0;
	inline Vector2 ScreenSize = { 0,0 };
	inline Vector2 SightCenter = { 0,0 };
	inline Matrix ViewMatrix;
	inline std::atomic<bool> MatrixReady{ false };

	inline int GlowID = 0;
	inline int current_fps = 0;

	inline bool MenuOpen = true;

	inline bool deviceConnected = false;
	inline int device_select = 0;
	inline char Km_IP[50] = { "" };
	inline char Km_port[50] = { "" };
	inline char Km_mac[50] = { "" };
}

namespace Game
{
	inline uint64_t localPlayerPtr = 0;
	inline uint64_t pInventory = 0;
	inline uint64_t pWeapon = 0;
	inline uint64_t WeaponHandle = 0;
	inline uint64_t WeaponEntity = 0;
}

namespace cheatConfig
{
	inline bool Outline = true;
	inline bool BoxESP = true;
	inline bool LineESP = true;
	inline bool HealthESP = false;
	inline bool ArmorESP = false;
	inline bool KnockESP = true;
	inline bool DistESP = false;
	inline bool TeamESP = false;
	inline bool BoneESP = false;
	inline float RenderDistance = 300.f;
	inline bool GlowESP = false;

	inline bool AimbotEnabled = false;
	inline int AimposSelect = 0;
	inline int AimbotKey = 2;
	inline int AimbotKeyMode = 0;
	inline int AimbotKey2 = 0;
	inline float AimbotFOV = 100.f;
	inline float AimbotFOV_ADS = 200.f;
	inline float AimbotSmooth = 1.f;
	inline float AimbotSmooth_zoomed = 1.f;
	inline float AimDistance = 300.f;
	inline bool DrawAimFOV = true;
	inline bool NotAimAtKnocked = false;
	inline bool NotAimAtWall = true;
	// 0=Head, 1=Neck, 2=Chest, 3=Stomach, 4=Arms, 5=Legs
	inline bool AimBoneSelect[6] = { true, true, true, true, false, false };

	inline bool RecoilControl = false;
	inline float HorizontalRecoilControl = 1.f;
	inline float VerticalRecoilControl = 1.f;

	inline bool Triggerbot = false;
	inline int TriggerbotKey = 0;
	inline int TriggerbotKeyMode = 0;
	inline int TriggerbotDelay = 0;
	inline int TriggerbotDistance = 100;

	inline bool RapidFire = false;
	inline bool AutoReload = false;
	inline bool BunnyHop = false;
	inline bool AutoGrapple = false;
	inline bool SuperGlideJump = false;
	inline bool TapStrafe = false;
	inline bool MapRadar = false;
	inline bool SafeCircle = false;
	inline bool ThirdPerson = false;
	inline bool ThirdPersonSwitch = false;
	inline bool SkyNade = false;
	inline int ReloadAtBullets = 1;
	inline int BunnyHopKey = VK_LCONTROL;
	inline int BunnyHopKeyMode = 0;
	inline int SuperGlideJumpKey = VK_LSHIFT;
	inline int SuperGlideJumpKeyMode = 0;
	inline int TapStrafeKey = 0;
	inline int TapStrafeKeyMode = 0;
	inline int MapRadarKey = VK_F2;
	inline int MapRadarKeyMode = 0;
	inline int SafeCircleKey = VK_F3;
	inline int SafeCircleKeyMode = 0;
	inline int ThirdPersonDist = 20;
	inline int ThirdPersonKey = VK_MBUTTON;


	inline ImColor BoxCol_vis = ImColor(255, 255, 255, 255);
	inline ImColor BoxCol_unvis = ImColor(0, 255, 0, 255);
	inline ImColor LineCol = ImColor(255, 255, 255, 255);
	inline ImColor BoneCol_Knock = ImColor(255, 125, 0, 255);
	inline ImColor BoneCol = ImColor(255, 255, 255, 255);
	inline ImColor BoneCol_Unvis = ImColor(0, 255, 0, 255);
	inline ImColor AimFOVCol = ImColor(255, 255, 255, 255);

	inline int GlowStyleVisSelect = 11;
	inline int GlowStyleUnvisSelect = 4;
	inline int GlowStyleVis = 10;
	inline int GlowStyleUnvis = 52;
}