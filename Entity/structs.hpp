#pragma once
#include "../utils.hpp"

struct Player {
	bool NPC;
	uint64_t Ptr;
	uint8_t buffer[0x4820];
	Vector3 Position;
	Vector3 Velocity;
	Vector3 CameraPos;
	std::string Name;
	std::string SignifierName;
	int armorType;
	int Shield;
	int MaxShield;
	int Health;
	int TeamID;
	int SquadID;
	int XP;
	int Level;
	int DuckState;
	int Zoomed;
	bool IsSpectator;
	bool IsValid;
	bool IsKnocked;
	bool IsVisible;
	int LifeState;
	float LastInCrosshairTime;
	float ToDistance;
	int WeaponIndex;

	int IsInCrosshair;

	uint64_t WorldTimeBase;

	Vector3 CachedBones[19];
	Matrix3x4 BoneMatrices[128];
};