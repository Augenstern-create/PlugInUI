#include "Entitys.h"

EntityInfo findEntityInfoByID(int id) {
    for (const auto& pair : EntityLists) {
        const EntityInfo& entity = pair.second;
        if (entity.ID == id) {
            return entity;
        }
    }

    return EntityInfo{"Unknown", EntityType::Unknown, 0};
}

std::unordered_map<std::string, EntityInfo> EntityLists = {
    {"DroppedItem", {"DroppedItem", EntityType::DroppedItem, 0}},
    {"DroppedItemGroup", {"DroppedItemGroup", EntityType::DroppedItemGroup, 0}},

    // 玩家
    {"PlayerMale_A", {"玩家", EntityType::Player, 0}},
    {"PlayerMale_A_C", {"玩家", EntityType::Player, 0}},
    {"PlayerFemale_A", {"玩家", EntityType::Player, 0}},
    {"PlayerFemale_A_C", {"玩家", EntityType::Player, 0}},
    {"AIPawn_Base_C", {"AI", EntityType::AI, 0}},
    {"AIPawn_Base_Female_C", {"AI", EntityType::AI, 0}},
    {"AIPawn_Base_Male_C", {"AI", EntityType::AI, 0}},
    {"AIPawn_Base_Pillar_C", {"AI", EntityType::AI, 0}},
    {"AIPawn_Base_Female_Pillar_C", {"AI", EntityType::AI, 0}},
    {"AIPawn_Base_Male_Pillar_C", {"AI", EntityType::AI, 0}},
    {"UltAIPawn_Base_Female_C", {"AI", EntityType::AI, 0}},
    {"UltAIPawn_Base_Male_C", {"AI", EntityType::AI, 0}},
    {"ZDF2_NPC_Runner_C", {"AI", EntityType::AI, 0}},
    {"ZDF2_NPC_Burning_C", {"AI", EntityType::AI, 0}},
    {"ZDF2_NPC_Tanker_C", {"AI", EntityType::AI, 0}},
    {"ZDF2_NPC_Female_C", {"AI", EntityType::AI, 0}},
    {"ZombieNpcNewPawn_Tanker_C", {"AI", EntityType::AI, 0}},

    // 刷新物品
    {"Item_Weapon_Groza_C", {"Groza", EntityType::Item, 0}},
    {"Item_Weapon_BerylM762_C", {"Beryl M762", EntityType::Item, 0}},
    {"Item_Weapon_ACE32_C", {"ACE32", EntityType::Item, 0}},
    {"Item_Weapon_HK416_C", {"M416", EntityType::Item, 0}},
    {"Item_Weapon_Duncans_M416_C", {"M416", EntityType::Item, 0}},
    {"Item_Weapon_AUG_C", {"AUG", EntityType::Item, 0}},
    {"Item_Weapon_AK47_C", {"AKM", EntityType::Item, 0}},
    {"Item_Weapon_Lunchmeats_AK47_C", {"AKM", EntityType::Item, 0}},
    {"Item_Weapon_SCAR-L_C", {"SCAR-L", EntityType::Item, 0}},
    {"Item_Weapon_G36C_C", {"G36C", EntityType::Item, 0}},
    {"Item_Weapon_QBZ95_C", {"QBZ95", EntityType::Item, 0}},
    {"Item_Weapon_K2_C", {"K2", EntityType::Item, 0}},
    {"Item_Weapon_Mk47Mutant_C", {"Mk47", EntityType::Item, 0}},
    {"Item_Weapon_M16A4_C", {"M16A4", EntityType::Item, 0}},
    {"Item_Weapon_OriginS12_C", {"O12", EntityType::Item, 0}},
    {"Item_Weapon_MG3_C", {"MG3", EntityType::Item, 0}},
    {"Item_Weapon_DP28_C", {"DP28", EntityType::Item, 0}},
    {"Item_Weapon_M249_C", {"M249", EntityType::Item, 0}},
    {"Item_Weapon_L6_C", {"Barrett", EntityType::Item, 0}},
    {"Item_Weapon_AWM_C", {"AWM", EntityType::Item, 0}},
    {"Item_Weapon_M24_C", {"M24", EntityType::Item, 0}},
    {"Item_Weapon_Julies_Kar98k_C", {"Kar98k", EntityType::Item, 0}},
    {"Item_Weapon_Kar98k_C", {"Kar98k", EntityType::Item, 0}},
    {"Item_Weapon_Mosin_C", {"莫辛甘纳", EntityType::Item, 0}},
    {"Item_Weapon_Win1894_C", {"Win94", EntityType::Item, 0}},
    {"Item_Weapon_Mk14_C", {"Mk14", EntityType::Item, 0}},
    {"Item_Weapon_FNFal_C", {"SLR", EntityType::Item, 0}},
    {"Item_Weapon_Mk12_C", {"Mk12", EntityType::Item, 0}},
    {"Item_Weapon_SKS_C", {"SKS", EntityType::Item, 0}},
    {"Item_Weapon_Mads_QBU88_C", {"QBU", EntityType::Item, 0}},
    {"Item_Weapon_QBU88_C", {"QBU", EntityType::Item, 0}},
    {"Item_Weapon_Dragunov_C", {"德拉贡诺夫", EntityType::Item, 0}},
    {"Item_Weapon_Mini14_C", {"Mini14", EntityType::Item, 0}},
    {"Item_Weapon_VSS_C", {"VSS", EntityType::Item, 0}},
    {"Item_Weapon_DP12_C", {"DBS", EntityType::Item, 0}},
    {"Item_Weapon_Saiga12_C", {"S12K", EntityType::Item, 0}},
    {"Item_Weapon_Winchester_C", {"S1897", EntityType::Item, 0}},
    {"Item_Weapon_Berreta686_C", {"S686", EntityType::Item, 0}},
    {"Item_Weapon_P90_C", {"P90", EntityType::Item, 0}},
    {"Item_Weapon_Vector_C", {"Vector", EntityType::Item, 0}},
    {"Item_Weapon_UZI_C", {"UZI", EntityType::Item, 0}},
    {"Item_Weapon_UMP_C", {"UMP", EntityType::Item, 0}},
    {"Item_Weapon_Thompson_C", {"Thompson", EntityType::Item, 0}},
    {"Item_Weapon_BizonPP19_C", {"BizonPP19", EntityType::Item, 0}},
    {"Item_Weapon_MP5K_C", {"MP5K", EntityType::Item, 0}},

    // 投掷物
    {"ProjGrenade_C", {"Grenade", EntityType::Project, 0}},

    // 武器
    {"WeapFlashBang_C", {"闪光弹", EntityType::Weapon, 0}},
    {"WeapGrenade_C", {"手雷", EntityType::Weapon, 0}},
    {"WeapStickyGrenade_C", {"粘性炸弹", EntityType::Weapon, 0}},
    {"WeapMolotov_C", {"燃烧瓶", EntityType::Weapon, 0}},
    {"WeapSmokeBomb_C", {"烟雾弹", EntityType::Weapon, 0}},
    {"WeapC4_C", {"C4", EntityType::Weapon, 0}},
    {"WeapDecoyGrenade_C", {"诱饵手雷", EntityType::Weapon, 0}},
    {"WeapBluezoneGrenade_C", {"毒圈手雷", EntityType::Weapon, 0}},
    {"WeapSpikeTrap_C", {"尖刺陷阱", EntityType::Weapon, 0}},

    // Melee
    {"WeapCowbar_C", {"撬棍", EntityType::Weapon, 0}},
    {"WeapPan_C", {"平底锅", EntityType::Weapon, 0}},
    {"WeapSickle_C", {"镰刀", EntityType::Weapon, 0}},
    {"WeapMachete_C", {"砍刀", EntityType::Weapon, 0}},
    {"WeapCowbarProjectile_C", {"撬棍", EntityType::Weapon, 0}},
    {"WeapMacheteProjectile_C", {"Machete", EntityType::Weapon, 0}},
    {"WeapPanProjectile_C", {"平底锅", EntityType::Weapon, 0}},
    {"WeapSickleProjectile_C", {"镰刀", EntityType::Weapon, 0}},

    // AR
    {"WeapLunchmeatsAK47_C", {"AKM", EntityType::Weapon, 0}},
    {"WeapAK47_C", {"AKM", EntityType::Weapon, 0}},
    {"WeapGroza_C", {"Groza", EntityType::Weapon, 0}},
    {"WeapDuncansHK416_C", {"M416", EntityType::Weapon, 0}},
    {"WeapHK416_C", {"M416", EntityType::Weapon, 0}},
    {"WeapM16A4_C", {"M16A4", EntityType::Weapon, 0}},
    {"WeapSCAR-L_C", {"SCAR-L", EntityType::Weapon, 0}},
    {"WeapACE32_C", {"ACE", EntityType::Weapon, 0}},
    {"WeapAUG_C", {"AUG", EntityType::Weapon, 0}},
    {"WeapBerylM762_C", {"BerylM762", EntityType::Weapon, 0}},
    {"WeapG36C_C", {"G36C", EntityType::Weapon, 0}},
    {"WeapQBZ95_C", {"QBZ95", EntityType::Weapon, 0}},
    {"WeapK2_C", {"K2", EntityType::Weapon, 0}},
    {"WeapMk47Mutant_C", {"Mk47", EntityType::Weapon, 0}},
    {"Weapon_OriginS12_C", {"O12", EntityType::Weapon, 0}},

    // SR
    {"WeapAWM_C", {"AWM", EntityType::Weapon, 0}},
    {"WeapJuliesM24_C", {"M24", EntityType::Weapon, 0}},
    {"WeapM24_C", {"M24", EntityType::Weapon, 0}},
    {"WeapJuliesKar98k_C", {"Kar98k", EntityType::Weapon, 0}},
    {"WeapKar98k_C", {"Kar98k", EntityType::Weapon, 0}},
    {"WeapWin94_C", {"Win94", EntityType::Weapon, 0}},
    {"WeapL6_C", {"Lynx", EntityType::Weapon, 0}},
    {"WeapMosinNagant_C", {"莫辛甘纳", EntityType::Weapon, 0}},
    {"WeapCrossbow_1_C", {"弩", EntityType::Weapon, 0}},

    // SG
    {"WeapBerreta686_C", {"S686", EntityType::Weapon, 0}},
    {"WeapSaiga12_C", {"S12K", EntityType::Weapon, 0}},
    {"WeapWinchester_C", {"S1897", EntityType::Weapon, 0}},
    {"WeapDP12_C", {"DBS", EntityType::Weapon, 0}},
    {"WeapSawnoff_C", {"锯齿手枪", EntityType::Weapon, 0}},

    // PISTOL
    {"WeapG18_C", {"P18C", EntityType::Weapon, 0}},
    {"WeapM1911_C", {"P1911", EntityType::Weapon, 0}},
    {"WeapM9_C", {"P92", EntityType::Weapon, 0}},
    {"WeapNagantM1895_C", {"R1895", EntityType::Weapon, 0}},
    {"WeapRhino_C", {"R45", EntityType::Weapon, 0}},
    {"WeapDesertEagle_C", {"Deagle", EntityType::Weapon, 0}},
    {"Weapvz61Skorpion_C", {"Skorpion", EntityType::Weapon, 0}},

    // LMG
    {"WeapM249_C", {"M249", EntityType::Weapon, 0}},
    {"WeapMG3_C", {"MG3", EntityType::Weapon, 0}},
    {"WeapDP28_C", {"DP28", EntityType::Weapon, 0}},

    // DMR
    {"WeapDragunov_C", {"德拉贡诺夫", EntityType::Weapon, 0}},
    {"WeapMini14_C", {"Mini14", EntityType::Weapon, 0}},
    {"WeapMk14_C", {"Mk14", EntityType::Weapon, 0}},
    {"WeapSKS_C", {"SKS", EntityType::Weapon, 0}},
    {"WeapFNFal_C", {"SLR", EntityType::Weapon, 0}},
    {"WeapMadsFNFal_C", {"SLR", EntityType::Weapon, 0}},
    {"WeapMadsQBU88_C", {"QBU", EntityType::Weapon, 0}},
    {"WeapQBU88_C", {"QBU", EntityType::Weapon, 0}},
    {"WeapMk12_C", {"Mk12", EntityType::Weapon, 0}},
    {"WeapVSS_C", {"VSS", EntityType::Weapon, 0}},

    // SMG
    {"WeapThompson_C", {"汤姆逊", EntityType::Weapon, 0}},
    {"WeapUMP_C", {"UMP", EntityType::Weapon, 0}},
    {"WeapUZI_C", {"UZI", EntityType::Weapon, 0}},
    {"WeapUziPro_C", {"UZI", EntityType::Weapon, 0}},
    {"WeapVector_C", {"Vector", EntityType::Weapon, 0}},
    {"WeapBizonPP19_C", {"野牛PP19", EntityType::Weapon, 0}},
    {"WeapMP5K_C", {"MP5K", EntityType::Weapon, 0}},
    {"WeapP90_C", {"P90", EntityType::Weapon, 0}},

    // Special
    {"WeapMortar_C", {"Mortar", EntityType::Weapon, 0}},
    {"WeapFlareGun_C", {"信号枪", EntityType::Weapon, 0}},
    {"WeapPanzerFaust100M1_C", {"火箭筒", EntityType::Weapon, 0}},
    {"WeapJerryCan_C", {"汽油桶", EntityType::Weapon, 0}},
    {"WeapDrone_C", {"无人机", EntityType::Weapon, 0}},
    {"WeapTraumaBag_C", {"医疗套件", EntityType::Weapon, 0}},
    {"WeapSpotterScope_C", {"望远镜", EntityType::Weapon, 0}},
    {"WeapTacPack_C", {"战术背包", EntityType::Weapon, 0}},
    {"WeapM79_C", {"M79", EntityType::Weapon, 0}},
};