#include "LanguageSwitch.h"

namespace ComponentsLists {

std::vector<Components<bool*>> ShboxLists = {{u8"显示方框", "showBox", &(gameData.Config.Box.showBox)},
                                             {u8"显示骨骼", "showSkeleton", &(gameData.Config.Box.showSkeleton)},
                                             {u8"显示血量", "showBloodstain", &(gameData.Config.Box.showBloodstain)},
                                             {u8"显示距离", "showLength", &(gameData.Config.Box.showLength)},
                                             {u8"显示视野", "showFov", &(gameData.Config.Box.showFov)},
                                             {u8"全局开关", "isGlobal", &(gameData.Config.Box.isGlobal)},
                                             {u8"车辆健康", "vehicleHealth", &(gameData.Config.Box.vehicleHealth)},
                                             {u8"车辆燃料", "vehicleFuel", &(gameData.Config.Box.vehicleFuel)},
                                             {u8"显示空投", "showAirdrop", &(gameData.Config.Box.showAirdrop)},
                                             {u8"空投物品", "airdropItems", &(gameData.Config.Box.airdropItems)},
                                             {u8"显示箱子", "showBox2", &(gameData.Config.Box.showBox2)},
                                             {u8"箱子物品", "BoxItems", &(gameData.Config.Box.BoxItems)},
                                             {u8"投掷提醒", "throwReminder", &(gameData.Config.Box.throwReminder)},
                                             {u8"拉销计时器", "pullPinTimer", &(gameData.Config.Box.pullPinTimer)},
                                             {u8"显示击杀数", "showKillCount", &(gameData.Config.Box.showKillCount)},
                                             {u8"显示瞄准线", "showAimLine", &(gameData.Config.Box.showAimLine)},
                                             {u8"显示玩家名称", "showPlayerName", &(gameData.Config.Box.showPlayerName)},
                                             {u8"显示队伍名称", "showTeamName", &(gameData.Config.Box.showTeamName)},
                                             {u8"显示手持武器", "showHandheldWeapon", &(gameData.Config.Box.showHandheldWeapon)},
                                             {u8"显示生存等级", "showSurvivalLevel", &(gameData.Config.Box.showSurvivalLevel)},
                                             {u8"显示旁观者数", "showSpectatorCount", &(gameData.Config.Box.showSpectatorCount)},
                                             {u8"显示队伍编号", "showTeamNumber", &(gameData.Config.Box.showTeamNumber)},
                                             {u8"显示玩家排名", "showPlayerRank", &(gameData.Config.Box.showPlayerRank)},
                                             {u8"显示平均伤害", "showAverageDamage", &(gameData.Config.Box.showAverageDamage)},
                                             {u8"显示玩家KDA", "showPlayerKda", &(gameData.Config.Box.showPlayerKda)},
                                             {u8"显示伤害统计", "showDamageStats", &(gameData.Config.Box.showDamageStats)},
                                             {u8"显示智能显示", "showSmartDisplay", &(gameData.Config.Box.showSmartDisplay)},
                                             {u8"显示附近敌人", "showNearbyEnemies", &(gameData.Config.Box.showNearbyEnemies)},
                                             {u8"显示大地图雷达", "showBigMapRadar", &(gameData.Config.Box.showBigMapRadar)},
                                             {u8"显示小地图雷达", "showSmallMapRadar", &(gameData.Config.Box.showSmallMapRadar)}};

std::vector<ComponentsSlider<float*, float>> SliderLists = {{u8"透视距离", "lengthDistance", &(gameData.Config.Slider.lengthDistance), 0.0f, 1000.0f},
                                                            {u8"骨骼距离", "skeletonDistance", &(gameData.Config.Slider.skeletonDistance), 0.0f, 1000.0f},
                                                            {u8"名称距离", "nameDistance", &(gameData.Config.Slider.nameDistance), 0.0f, 1000.0f},
                                                            {u8"手持物品距离", "handheldDistance", &(gameData.Config.Slider.handheldDistance), 0.0f, 1000.0f},
                                                            {u8"信息距离", "infoDistance", &(gameData.Config.Slider.infoDistance), 0.0f, 1000.0f},
                                                            {u8"骨骼厚度", "skeletonThickness", &(gameData.Config.Slider.skeletonThickness), 0.0f, 1000.0f},
                                                            {u8"车辆距离", "vehicleDistance", &(gameData.Config.Slider.vehicleDistance), 0.0f, 1000.0f},
                                                            {u8"空投距离", "airdropDistance", &(gameData.Config.Slider.airdropDistance), 0.0f, 1000.0f},
                                                            {u8"箱子距离", "boxDistance", &(gameData.Config.Slider.boxDistance), 0.0f, 1000.0f},
                                                            {u8"方框厚度", "boxThickness", &(gameData.Config.Slider.boxThickness), 0.0f, 1000.0f},
                                                            {u8"附近距离", "nearbyDistance", &(gameData.Config.Slider.nearbyDistance), 0.0f, 1000.0f},
                                                            {u8"锁定透明度", "lockOpacity", &(gameData.Config.Slider.lockOpacity), 0.0f, 1000.0f},
                                                            {u8"投掷距离", "throwDistance", &(gameData.Config.Slider.throwDistance), 0.0f, 1000.0f},
                                                            {u8"补给距离", "supplyDistance", &(gameData.Config.Slider.supplyDistance), 0.0f, 1000.0f},
                                                            {u8"危险击杀数", "dangerKillCount", &(gameData.Config.Slider.dangerKillCount), 0.0f, 1000.0f},
                                                            {u8"字体大小", "fontSize", &(gameData.Config.Slider.fontSize), 8.0f, 58.0f},
                                                            {u8"骨骼字体大小", "skeletonfontSize", &(gameData.Config.Slider.skeletonfontSize), 8.0f, 58.0f},
                                                            {u8"车辆字体大小", "vehicleFontSize", &(gameData.Config.Slider.vehicleFontSize), 8.0f, 58.0f},
                                                            {u8"空投字体大小", "airdropFontSize", &(gameData.Config.Slider.airdropFontSize), 8.0f, 58.0f},
                                                            {u8"箱子字体大小", "boxFontSize", &(gameData.Config.Slider.boxFontSize), 8.0f, 58.0f},
                                                            {u8"投掷字体大小", "throwFontSize", &(gameData.Config.Slider.throwFontSize), 8.0f, 58.0f},
                                                            {u8"补给字体大小", "supplyFontSize", &(gameData.Config.Slider.supplyFontSize), 8.0f, 58.0f}};

std::vector<ComponentsSlider<int*, int>> SliderIntLists = {};

std::vector<Components<float*>> ColorList = {{"可视骨骼", "visualBone", (float*)&(gameData.Config.Color.visualBoneColor)},
                                             {"不可视骨骼", "invisibleBone", (float*)&(gameData.Config.Color.invisibleBoneColor)},
                                             {"倒地骨骼", "fallenSkeleton", (float*)&(gameData.Config.Color.fallenSkeletonColor)},
                                             {"倒地血量", "downBlood", (float*)&(gameData.Config.Color.downBloodColor)},
                                             {"机器人骨骼", "botSkeleton", (float*)&(gameData.Config.Color.botSkeletonColor)},
                                             {"射线颜色", "rayColor", (float*)&(gameData.Config.Color.rayColor)},
                                             {"队友骨骼", "collaboratorSkeleton", (float*)&(gameData.Config.Color.collaboratorSkeletonColor)},
                                             {"队友信息", "collaboratorInfo", (float*)&(gameData.Config.Color.collaboratorInfoColor)},
                                             {"玩家名称", "playerName", (float*)&(gameData.Config.Color.playerNameColor)},
                                             {"玩家信息", "playerInfo", (float*)&(gameData.Config.Color.playerInfoColor)},
                                             {"敌人信息", "dangerInfo", (float*)&(gameData.Config.Color.dangerInfoColor)},
                                             {"手持武器", "handheldWeapon", (float*)&(gameData.Config.Color.handheldWeaponColor)},
                                             {"车辆颜色", "vehicleColor", (float*)&(gameData.Config.Color.vehicleColor)},
                                             {"空投颜色", "airdropColor", (float*)&(gameData.Config.Color.airdropColor)},
                                             {"箱子颜色", "boxColor", (float*)&(gameData.Config.Color.boxColor)},
                                             {"投掷颜色", "throwColor", (float*)&(gameData.Config.Color.throwColor)},
                                             {"材料组 1", "materialsGroup1", (float*)&(gameData.Config.Color.materialsGroup1Color)},
                                             {"材料组 2", "materialsGroup2", (float*)&(gameData.Config.Color.materialsGroup2Color)},
                                             {"材料组 3", "materialsGroup3", (float*)&(gameData.Config.Color.materialsGroup3Color)},
                                             {"材料组 4", "materialsGroup4", (float*)&(gameData.Config.Color.materialsGroup4Color)}};

inline bool WriteToINI(const std::string& filename) {
    std::ofstream iniFile(filename);

    if (!iniFile.is_open()) {
        LOGE("Error creating file: %s", filename.c_str());
        return false;
    }
    iniFile << "[Shbox]\n";
    for (auto info : ShboxLists) {
        iniFile << info.EnglishName << " = " << *(info.value) << "\n";
    }
    iniFile << "\n[Slider]\n";
    for (auto info : SliderLists) {
        iniFile << info.EnglishName << " = " << *(info.value) << "\n";
    }

    iniFile << "\n[Color]\n";
    iniFile << std::fixed << std::setprecision(6);
    for (auto info : ColorList) {
        for (int i = 0; i < 4; i++) {
            std::string key = info.EnglishName + std::to_string(i + 1);
            iniFile << key << " = " << info.value[i] << "\n";
        }
    }

    iniFile.close();
    return true;
}
inline bool LoadSet() {
    std::string filename = "config.ini";
    INIReader reader(filename);
    if (reader.ParseError() < 0) {
        std::ifstream fileStream(filename);
        if (fileStream.good()) {
            fileStream.close();  // Close the file stream before removal
            if (std::remove(filename.c_str()) == 0) {
            }
        }
        if (!WriteToINI(filename)) return false;
    }
    for (auto& info : ShboxLists) {
        std::string stringValue = reader.Get("Shbox", info.EnglishName, "");
        if (!stringValue.empty()) {
            *(info.value) = (stringValue == "1");
        }
    }
    for (auto info : SliderLists) {
        *(info.value) = reader.GetReal("Slider", info.EnglishName, *(info.value));
    }
    for (auto& info : ColorList) {
        for (int i = 0; i < 4; ++i) {
            std::string key = info.EnglishName + std::to_string(i + 1);
            info.value[i] = (float)reader.GetReal("Color", key, info.value[i]);
        }
    }

    return true;
}
inline bool SaveSet() {
    std::string filename = "config.ini";
    std::ifstream fileStream(filename);
    if (fileStream.good()) {
        fileStream.close();
        if (std::remove(filename.c_str()) == 0) {
        }
    }
    if (!WriteToINI(filename)) return false;
    return true;
}

}  // namespace ComponentsLists