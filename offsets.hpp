#pragma once
#include "structs.hpp"
#include "Memory/driver.h"

using off = const unsigned long long;

namespace Offset
{
	namespace Misc
	{
		constexpr off GlobalVars = 0x1E3CCF0;
		constexpr off LevelName = 0x1e3d174;
		constexpr off EntityList = 0x6268c28;
		constexpr off LocalPlayer = 0x2691c08;
		constexpr off ViewMatrix = 0x11a350;
		constexpr off ViewRender = 0x3d9a068;
		constexpr off HighlightSettings = 0x69b2900;
		constexpr off StudioHdr = 0x1000;
		constexpr off ObserverList = 0x626ac48;
	}

	namespace Buttons
	{
		constexpr off Attack = 0x3d9c940;
		constexpr off Duck = 0x3d9d2e8;
		constexpr off Jump = 0x3d9d1f8;
		constexpr off Reload = 0x3d9d158;
	}

	namespace Player
	{
		constexpr off TimeBase = 0x2178;
		constexpr off Health = 0x324;
		constexpr off MaxHealth = 0x468;
		constexpr off ShieldHealth = 0x1a0;
		constexpr off ShieldHealthMax = 0x1a4;
		constexpr off BleedoutState = 0x27E0;
		constexpr off LifeState = 0x690;
		constexpr off DuckState = 0x2AE4;
		constexpr off VecAbsOrigin = 0x17c;
		constexpr off TeamNum = 0x334;
		constexpr off SquadID = 0x340;
		constexpr off Flags = 0xc8;
		constexpr off SignifierName = 0x470;
		constexpr off ScriptNameIndex = 0x580;
		constexpr off LastVisibleTime = 0x1A74;
		constexpr off CrosshairTime = LastVisibleTime + 0x4;
		constexpr off LastCrosshairTime = LastVisibleTime + 0x8;
		constexpr off Zooming = 0x1CC1;
		constexpr off Velocity = 0x170;	// m_vecAbsVelocity
		constexpr off ViewAngle = 0x2610; // m_ammoPoolCapacity - 0x14
		constexpr off AimPunchAngle = 0x2528; // m_currentFrameLocalPlayer.m_vecPunchWeapon_Angle
		constexpr off TraversalProgress = 0x2BF4;
		constexpr off ViewOffset = 0xe8; // m_currentFrame.viewOffset
		constexpr off CameraPos = 0x1FD4; // camera_origin
		constexpr off BreathAngle = ViewAngle - 0x10; // camera_angles

		constexpr off m_inventory = 0x1968;
		constexpr off m_latestPrimaryWeapons = 0x19D4;

		constexpr off Bones = 0xDB8 + 0x48; // m_nForceBone + 0x48
	}

	namespace Weapon
	{
		constexpr off WeaponHandle = 0x1970;
		constexpr off WeaponIndex = 0x1888;
		constexpr off WeaponSettingsMeta = 0x1B20;
		constexpr off NextReadyTime = 0x15E8;
		constexpr off AmmoInClip = 0x15E0;
		constexpr off IsSemiAuto = WeaponSettingsMeta + 0x3DC; // WeaponSettings.is_semi_auto
		constexpr off BulletSpeed = WeaponSettingsMeta + 0xDA8; // WeaponSettingsMeta + projectile_launch_speed
		constexpr off BulletScale = WeaponSettingsMeta + 0xDB0; // WeaponSettingsMeta + projectile_gravity_scale
		constexpr off BulletDrag = WeaponSettingsMeta + 0xDB8; // projectile_drag_coefficient
	}

	namespace HighlightSettings
	{
		constexpr off GlowEnable = 0x28c;	// m_highlightServerActiveStates
		constexpr off GlowThroughWall = 0x26c;	// m_highlightVisibilityType
		constexpr off GlowFix = 0x278;
		constexpr off GlowHighlightID = 0x298;	// m_highlightServerActiveStates
		constexpr off GlowDistance = 0x294;		// Highlight_SetFarFadeDist
	}

	namespace ThirdPerson
	{
		constexpr off thirdperson_override = 0x2524940;
	}

	namespace Grapple
	{
		constexpr off Grapple = 0x2D48;
		constexpr off GrappleAttached = 0x48;
	}

	namespace Minimap
	{
		constexpr off MINIMAP_ZOOM_SCALE = 0x4868;	// m_minimapTargetZoomScale
		constexpr off MINIMAP_ZOOM_LERP_TIME = 0x486C;	// m_minimapTargetLerpTime
		constexpr off HUD_MINIMAP_ROTATE = 0x02565070;	// hud_setting_minimapRotate
	}
}
