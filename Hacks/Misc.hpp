#pragma once
#include "../Entity/Entity.hpp"

namespace Misc
{
    enum eWeaponId : int {
        WEAPON_R301 = 0,    // mp_weapon_rspn101
        WEAPON_SENTINEL = 2,    // mp_weapon_sentinel
        WEAPON_BOCEK = 3,    // mp_weapon_bow
        WEAPON_RAMPAGE = 7,    // mp_weapon_dragon_lmg
        WEAPON_ALTERNATOR = 89,   // mp_weapon_alternator_smg
        WEAPON_P2020 = 90,   // mp_weapon_autopistol
        WEAPON_CHARGE_RIFLE = 93,   // mp_weapon_defender
        WEAPON_DEVOTION = 94,   // mp_weapon_esaw
        WEAPON_LONGBOW = 96,   // mp_weapon_dmr
        WEAPON_HAVOC = 97,   // mp_weapon_energy_ar
        WEAPON_EVA8 = 99,   // mp_weapon_shotgun
        WEAPON_FLATLINE = 101,  // mp_weapon_vinson
        WEAPON_G7_SCOUT = 102,  // mp_weapon_g2
        WEAPON_HEMLOK = 104,  // mp_weapon_hemlok
        WEAPON_KRABER = 107,  // mp_weapon_sniper
        WEAPON_BUSTER_RIFLE = 108,  // mp_weapon_buster_rifle
        WEAPON_LSTAR = 109,  // mp_weapon_lstar
        WEAPON_MASTIFF = 111,  // mp_weapon_mastiff
        WEAPON_MOZAMBIQUE = 113,  // mp_weapon_shotgun_pistol
        WEAPON_PROWLER = 119,  // mp_weapon_pdw
        WEAPON_PEACEKEEPER = 121,  // mp_weapon_energy_shotgun
        WEAPON_R99 = 123,  // mp_weapon_r97
        WEAPON_RE45 = 125,  // mp_weapon_semipistol
        WEAPON_SPITFIRE = 127,  // mp_weapon_lmg
        WEAPON_TRIPLE_TAKE = 128,  // mp_weapon_doubletake
        WEAPON_WINGMAN = 130,  // mp_weapon_wingman
        WEAPON_VOLT = 132,  // mp_weapon_volt_smg
        WEAPON_3030_REPEATER = 133,  // mp_weapon_3030
        WEAPON_CAR = 136,  // mp_weapon_car
        WEAPON_NEMESIS = 138,  // mp_weapon_nemesis
        WEAPON_FRAG_GRENADE = 139,  // ordnance
        WEAPON_ARC_STAR = 140,  // ordnance
        WEAPON_THERMITE = 141,  // ordnance
        WEAPON_MELEE_SURVIVAL = 142,  // mp_weapon_melee_survival
        WEAPON_MELEE = 143,  // melee_pilot_emptyhanded
        WEAPON_THROWING_KNIFE = 223,  // mp_weapon_throwingknife
        WEAPON_EVAC_TOWER = 226,
        WEAPON_HEAL = 242,
    };

    void ThirdPerson(uintptr_t localPlayer);

    void BunnyHop(uintptr_t localPlayer);
    void AutoGrapple(uintptr_t localPlayer);
    void SuperGlide(uintptr_t localPlayer);
    void SuperGlideJump(uintptr_t localPlayer);
    void TapStrafe(uintptr_t localPlayer);

}