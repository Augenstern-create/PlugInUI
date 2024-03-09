#pragma once
#include "common/util.h"
#include "utils/Utils.h"

struct tZeroingInfo {
    float BaseIron = 100.0f;
    float BaseScope = 100.0f;
    float Increment = 100.0f;
    float BaseHolo = BaseScope;
};

enum class tWeaponType {
    None,    // 无
    Throw,   // 投掷武器
    Melee,   // 近战武器
    AR,      // 突击步枪
    SR,      // 狙击步枪
    SG,      // 霰弹枪
    LMG,     // 轻机枪
    SMG,     // 冲锋枪
    BOW,     // 弓箭
    PISTOL,  // 手枪
    DMR      // 狙击步枪
};

struct tWeaponInfo {
    fixstr::basic_fixed_string<char, 0x100> WeaponName;
    tWeaponType WeaponType = tWeaponType::None;
    tZeroingInfo ZeroingInfo;
};

static tWeaponInfo GetWeaponInfo(unsigned Hash) {
    constexpr static tZeroingInfo ZeroingSG = {50.0f, 100.0f, 0.0f};
    constexpr static tZeroingInfo ZeroingCBow = {25.0f, 25.0f, 0.0f};
    constexpr static tZeroingInfo ZeroingVSS = {100.0f, 100.0f, 25.0f};
    constexpr static tZeroingInfo ZeroingPistol = {50.0f, 50.0f, 0.0f, 100.0f};
    constexpr static tZeroingInfo ZeroingDeagle = {50.0f, 100.0f, 0.0f, 100.0f};
    constexpr static tZeroingInfo ZeroingSkorpion = {100.0f, 50.0f, 0.0, 100.0f};

    switch (Hash) {
        // Throw
        HASH_CASE("WeapFlashBang_C"h, {"Flash", tWeaponType::Throw});
        HASH_CASE("WeapGrenade_C"h, {"Grenade", tWeaponType::Throw});
        HASH_CASE("WeapStickyGrenade_C"h, {"Bomb", tWeaponType::Throw});
        HASH_CASE("WeapMolotov_C"h, {"Molotov", tWeaponType::Throw});
        HASH_CASE("WeapSmokeBomb_C"h, {"Smoke", tWeaponType::Throw});
        HASH_CASE("WeapC4_C"h, {"C4", tWeaponType::Throw});
        HASH_CASE("WeapDecoyGrenade_C"h, {"Decoy", tWeaponType::Throw});
        HASH_CASE("WeapBluezoneGrenade_C"h, {"BlueBomb", tWeaponType::Throw});
        HASH_CASE("WeapSpikeTrap_C"h, {"Trap", tWeaponType::Throw});

        // Melee
        HASH_CASE("WeapCowbar_C"h, {"Crawbar", tWeaponType::Melee});
        HASH_CASE("WeapPan_C"h, {"Pan", tWeaponType::Melee});
        HASH_CASE("WeapSickle_C"h, {"Sickle", tWeaponType::Melee});
        HASH_CASE("WeapMachete_C"h, {"Machete", tWeaponType::Melee});
        HASH_CASE("WeapCowbarProjectile_C"h, {"Crawbar", tWeaponType::Throw});
        HASH_CASE("WeapMacheteProjectile_C"h, {"Machete", tWeaponType::Throw});
        HASH_CASE("WeapPanProjectile_C"h, {"Pan", tWeaponType::Throw});
        HASH_CASE("WeapSickleProjectile_C"h, {"Sickle", tWeaponType::Throw});

        // AR
        HASH_CASE("WeapLunchmeatsAK47_C"h, {"AK", tWeaponType::AR});
        HASH_CASE("WeapAK47_C"h, {"AK", tWeaponType::AR});
        HASH_CASE("WeapGroza_C"h, {"Groza", tWeaponType::AR});
        HASH_CASE("WeapDuncansHK416_C"h, {"M4", tWeaponType::AR});
        HASH_CASE("WeapHK416_C"h, {"M4", tWeaponType::AR});
        HASH_CASE("WeapM16A4_C"h, {"M16", tWeaponType::AR});
        HASH_CASE("WeapSCAR-L_C"h, {"SCAR", tWeaponType::AR});
        HASH_CASE("WeapACE32_C"h, {"ACE", tWeaponType::AR});
        HASH_CASE("WeapAUG_C"h, {"AUG", tWeaponType::AR});
        HASH_CASE("WeapBerylM762_C"h, {"Beryl", tWeaponType::AR});
        HASH_CASE("WeapG36C_C"h, {"G36C", tWeaponType::AR});
        HASH_CASE("WeapQBZ95_C"h, {"QBZ", tWeaponType::AR});
        HASH_CASE("WeapK2_C"h, {"K2", tWeaponType::AR});
        HASH_CASE("WeapMk47Mutant_C"h, {"Mutant", tWeaponType::AR});

        // SR
        HASH_CASE("WeapAWM_C"h, {"AWM", tWeaponType::SR});
        HASH_CASE("WeapJuliesM24_C"h, {"M24", tWeaponType::SR});
        HASH_CASE("WeapM24_C"h, {"M24", tWeaponType::SR});
        HASH_CASE("WeapJuliesKar98k_C"h, {"K98", tWeaponType::SR});
        HASH_CASE("WeapKar98k_C"h, {"K98", tWeaponType::SR});
        HASH_CASE("WeapWin94_C"h, {"Win94", tWeaponType::SR});
        HASH_CASE("WeapL6_C"h, {"Lynx", tWeaponType::SR});
        HASH_CASE("WeapMosinNagant_C"h, {"Mosin", tWeaponType::SR});
        HASH_CASE("WeapCrossbow_1_C"h, {"CBow", tWeaponType::SR, ZeroingCBow});

        // SG
        HASH_CASE("WeapBerreta686_C"h, {"S686", tWeaponType::SG, ZeroingSG});
        HASH_CASE("WeapSaiga12_C"h, {"S12K", tWeaponType::SG, ZeroingSG});
        HASH_CASE("WeapWinchester_C"h, {"S1897", tWeaponType::SG, ZeroingSG});
        HASH_CASE("WeapDP12_C"h, {"DBS", tWeaponType::SG, ZeroingSG});
        HASH_CASE("WeapSawnoff_C"h, {"Sawnoff", tWeaponType::SG, ZeroingSG});

        // PISTOL
        HASH_CASE("WeapG18_C"h, {"P18C", tWeaponType::PISTOL, ZeroingPistol});
        HASH_CASE("WeapM1911_C"h, {"P1911", tWeaponType::PISTOL, ZeroingPistol});
        HASH_CASE("WeapM9_C"h, {"P92", tWeaponType::PISTOL, ZeroingPistol});
        HASH_CASE("WeapNagantM1895_C"h, {"R1895", tWeaponType::PISTOL, ZeroingPistol});
        HASH_CASE("WeapRhino_C"h, {"R45", tWeaponType::PISTOL, ZeroingPistol});
        HASH_CASE("WeapDesertEagle_C"h, {"Deagle", tWeaponType::PISTOL, ZeroingDeagle});
        HASH_CASE("Weapvz61Skorpion_C"h, {"Skorpion", tWeaponType::PISTOL, ZeroingSkorpion});

        // LMG
        HASH_CASE("WeapM249_C"h, {"M249", tWeaponType::LMG});
        HASH_CASE("WeapMG3_C"h, {"MG3", tWeaponType::LMG});
        HASH_CASE("WeapDP28_C"h, {"DP28", tWeaponType::LMG});

        // DMR
        HASH_CASE("WeapMini14_C"h, {"Mini", tWeaponType::DMR});
        HASH_CASE("WeapMk14_C"h, {"Mk14", tWeaponType::DMR});
        HASH_CASE("WeapSKS_C"h, {"SKS", tWeaponType::DMR});
        HASH_CASE("WeapFNFal_C"h, {"SLR", tWeaponType::DMR});
        HASH_CASE("WeapMadsFNFal_C"h, {"SLR", tWeaponType::DMR});
        HASH_CASE("WeapMadsQBU88_C"h, {"QBU", tWeaponType::DMR});
        HASH_CASE("WeapQBU88_C"h, {"QBU", tWeaponType::DMR});
        HASH_CASE("WeapMk12_C"h, {"Mk12", tWeaponType::DMR});
        HASH_CASE("WeapVSS_C"h, {"VSS", tWeaponType::DMR, ZeroingVSS});

        // SMG
        HASH_CASE("WeapThompson_C"h, {"Tom", tWeaponType::SMG});
        HASH_CASE("WeapUMP_C"h, {"UMP", tWeaponType::SMG});
        HASH_CASE("WeapUZI_C"h, {"UZI", tWeaponType::SMG});
        HASH_CASE("WeapUziPro_C"h, {"UZI", tWeaponType::SMG});
        HASH_CASE("WeapVector_C"h, {"Vec", tWeaponType::SMG});
        HASH_CASE("WeapBizonPP19_C"h, {"Bizon", tWeaponType::SMG});
        HASH_CASE("WeapMP5K_C"h, {"MP5K", tWeaponType::SMG});
        HASH_CASE("WeapP90_C"h, {"P90", tWeaponType::SMG});

        // Special
        HASH_CASE("WeapMortar_C"h, {"Mortar"});
        HASH_CASE("WeapFlareGun_C"h, {"FlareGun"});
        HASH_CASE("WeapPanzerFaust100M1_C"h, {"Rocket"});
        HASH_CASE("WeapJerryCan_C"h, {"Fuel"});
        HASH_CASE("WeapDrone_C"h, {"Drone"});
        HASH_CASE("WeapTraumaBag_C"h, {"TraumaBag"});
        HASH_CASE("WeapSpotterScope_C"h, {"Spotter"});
        HASH_CASE("WeapTacPack_C"h, {"TacPack"});
        HASH_CASE("WeapM79_C"h, {"M79"});

        HASH_DEFAULT({});
    }
}