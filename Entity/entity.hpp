#pragma once
#include <unordered_set>
#include "structs.hpp"

struct EntityCache {
	const std::vector<Player>* snapCopy;
	const Player* localPlayer;
};

std::string GetLevelName();

int GetSquadByMap(uint64_t Addr);

uint64_t GetEntityPtr(int i);

uint64_t GetLocalPlayerPtr();

void GetWeaponEntity(uint64_t Addr);

bool Isvalid(uint64_t Addr);

Vector3 GetPosition(uint64_t Addr);

int GetKnocked(uint64_t Addr);

int GetLifeState(uint64_t Addr);

int GetHealth(uint64_t Addr);

int GetMaxHealth(uint64_t Addr);

int GetTeamID(uint64_t Addr);

int GetSquadID(uint64_t Addr);

float GetDistance(Vector3 LocalPlayer, Vector3 Entity);

bool GetVisible(uint64_t LocalPlayerPtr, uint64_t EntityPtr);

int getLevel(int m_xp);

std::string GetSignifier(uint64_t Addr);

int get_script_name(uint64_t EntityAddr);

int GetWeaponIndex(uint64_t EntityAddr);

bool IsInCrossHair(uint64_t LocalPlayerPtr, uint64_t EntityPtr);

bool IsOnGround(uint64_t Addr);

Vector3 GetViewOffset(uint64_t Addr);

Vector3 NewHitbox(uintptr_t ent, int HitBox);

float getBulletSpeed(uintptr_t current_weapon);

float GetBulletScale(uintptr_t current_weapon);

int CanAim(uint64_t local);