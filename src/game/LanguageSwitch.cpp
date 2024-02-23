#include "LanguageSwitch.h"

namespace ComponentsLists {

std::vector<Components<bool*>> radar_box_ = {
    {u8"绘制名字", "is_drawing_names", &(gameData.readConfig.is_drawing_names)},
    {u8"绘制血量", "is_drawing_hp", &(gameData.readConfig.is_drawing_hp)},
    {u8"绘制队伍ID", "is_drawing_teamID", &(gameData.readConfig.is_drawing_teamID)},
    {u8"绘制位置", "is_drawing_location", &(gameData.readConfig.is_drawing_location)},
    {u8"绘制方向线", "is_drawing_sight", &(gameData.readConfig.is_drawing_sight)},
};
std::vector<ComponentsSlider<float*, float>> radar_slider_ = {
    {u8"坐标大小", "location_size", &(gameData.readConfig.location_size), 2.0f, 20.0f},
};
std::vector<ComponentsSlider<int*, int>> radar_slider_int_ = {

};
std::vector<Components<float*>> radar_color_ = {
    {"敌人颜色", "foes_location_color", (float*)&(gameData.readConfig.foes_location_color)},
    {"队友颜色", "team_location_color", (float*)&(gameData.readConfig.team_location_color)},
    {"本人颜色", "my_location_color", (float*)&(gameData.readConfig.my_location_color)},
    {"血条颜色", "hp_color", (float*)&(gameData.readConfig.hp_color)},
};

std::vector<Components<bool*>> skeleton_box_ = {
    {u8"绘制方框", "is_drawing_box", &(gameData.skeletonConfig.is_drawing_box)},
    {u8"绘制骨骼", "is_drawing_skeleton", &(gameData.skeletonConfig.is_drawing_skeleton)},
    {u8"绘制血量", "is_drawing_hp", &(gameData.skeletonConfig.is_drawing_hp)},
    {u8"绘制距离", "is_drawing_distance", &(gameData.skeletonConfig.is_drawing_distance)},
    {u8"绘制玩家名称", "is_drawing_names", &(gameData.skeletonConfig.is_drawing_names)},
    {u8"绘制队伍名称", "is_drawing_team_name", &(gameData.skeletonConfig.is_drawing_team_name)},
    {u8"绘制队伍编号", "is_drawing_teamID", &(gameData.skeletonConfig.is_drawing_teamID)},
    {u8"绘制手持武器", "is_drawing_handheld_weapon", &(gameData.skeletonConfig.is_drawing_handheld_weapon)},
    {u8"绘制大地图雷达", "is_drawing_atlas_radar", &(gameData.skeletonConfig.is_drawing_atlas_radar)},
    {u8"绘制小地图雷达", "is_drawing_small_map_radar", &(gameData.skeletonConfig.is_drawing_small_map_radar)},

    {u8"绘制视野", "showFov", &(gameData.Config.Box.showFov)},
    {u8"全局开关", "isGlobal", &(gameData.Config.Box.isGlobal)},
    {u8"车辆健康", "vehicleHealth", &(gameData.Config.Box.vehicleHealth)},
    {u8"车辆燃料", "vehicleFuel", &(gameData.Config.Box.vehicleFuel)},
    {u8"绘制空投", "showAirdrop", &(gameData.Config.Box.showAirdrop)},
    {u8"空投物品", "airdropItems", &(gameData.Config.Box.airdropItems)},
    {u8"绘制箱子", "showBox2", &(gameData.Config.Box.showBox2)},
    {u8"箱子物品", "BoxItems", &(gameData.Config.Box.BoxItems)},
    {u8"投掷提醒", "throwReminder", &(gameData.Config.Box.throwReminder)},
    {u8"拉销计时器", "pullPinTimer", &(gameData.Config.Box.pullPinTimer)},
    {u8"绘制击杀数", "showKillCount", &(gameData.Config.Box.showKillCount)},
    {u8"绘制瞄准线", "showAimLine", &(gameData.Config.Box.showAimLine)},
    {u8"绘制生存等级", "showSurvivalLevel", &(gameData.Config.Box.showSurvivalLevel)},
    {u8"绘制旁观者数", "showSpectatorCount", &(gameData.Config.Box.showSpectatorCount)},
    {u8"绘制玩家排名", "showPlayerRank", &(gameData.Config.Box.showPlayerRank)},
    {u8"绘制平均伤害", "showAverageDamage", &(gameData.Config.Box.showAverageDamage)},
    {u8"绘制玩家KDA", "showPlayerKda", &(gameData.Config.Box.showPlayerKda)},
    {u8"绘制伤害统计", "showDamageStats", &(gameData.Config.Box.showDamageStats)},
    {u8"智能绘制", "showSmartDisplay", &(gameData.Config.Box.showSmartDisplay)},
    {u8"绘制附近敌人", "showNearbyEnemies", &(gameData.Config.Box.showNearbyEnemies)},

};

std::vector<ComponentsSlider<float*, float>> skeleton_slider_ = {
    {u8"透视距离", "lengthDistance", &(gameData.Config.Slider.lengthDistance), 0.0f, 1000.0f},
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
    {u8"补给字体大小", "supplyFontSize", &(gameData.Config.Slider.supplyFontSize), 8.0f, 58.0f},
};

std::vector<ComponentsSlider<int*, int>> skeleton_slider_int_ = {};

std::vector<Components<float*>> skeleton_color_ = {
    {"可视骨骼", "visualBone", (float*)&(gameData.Config.Color.visualBoneColor)},
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
    {"材料组 4", "materialsGroup4", (float*)&(gameData.Config.Color.materialsGroup4Color)},
};

std::vector<Components<bool*>> setting_box_ = {};
std::vector<ComponentsSlider<float*, float>> setting_slider_ = {
    {u8"默认字体大小", "default_font_size", &(gameData.setting.default_font_size), 8.0f, 58.0f},
    {u8"菜单字体大小", "menu_font_size", &(gameData.setting.menu_font_size), 8.0f, 58.0f},
};
std::vector<ComponentsSlider<int*, int>> setting_slider_int_ = {};
std::vector<Components<float*>> setting_color_ = {
    {"菜单背景颜色", "subform_color", (float*)&(gameData.setting.subform_color)},
};

void writeInfoToFile(std::ofstream& iniFile, std::vector<Components<bool*>> list) {
    for (auto info : list) {
        iniFile << info.EnglishName << " = " << *(info.value) << "\n";
    }
}
void writeInfoToFile(std::ofstream& iniFile, std::vector<ComponentsSlider<float*, float>> list) {
    for (auto info : list) {
        iniFile << info.EnglishName << " = " << *(info.value) << "\n";
    }
}
void writeInfoToFile(std::ofstream& iniFile, std::vector<ComponentsSlider<int*, int>> list) {
    for (auto info : list) {
        iniFile << info.EnglishName << " = " << *(info.value) << "\n";
    }
}
void writeInfoToFile(std::ofstream& iniFile, std::vector<Components<float*>> list) {
    for (auto info : list) {
        for (int i = 0; i < 4; i++) {
            std::string key = info.EnglishName + std::to_string(i + 1);
            iniFile << key << " = " << info.value[i] << "\n";
        }
    }
}

void RetrieveToMemory(INIReader* reader, std::vector<Components<bool*>>* list, std::string name) {
    for (auto info : *list) {
        std::string stringValue = reader->Get(name, info.EnglishName, "");
        if (!stringValue.empty()) {
            *(info.value) = (stringValue == "1");
        }
    }
}
void RetrieveToMemory(INIReader* reader, std::vector<ComponentsSlider<float*, float>>* list, std::string name) {
    for (auto info : *list) {
        *(info.value) = (float)reader->GetReal(name, info.EnglishName, *(info.value));
    }
}
void RetrieveToMemory(INIReader* reader, std::vector<ComponentsSlider<int*, int>>* list, std::string name) {
    for (auto info : *list) {
        *(info.value) = (float)reader->GetReal(name, info.EnglishName, *(info.value));
    }
}
void RetrieveToMemory(INIReader* reader, std::vector<Components<float*>>* list, std::string name) {
    for (auto& info : *list) {
        for (int i = 0; i < 4; ++i) {
            std::string key = info.EnglishName + std::to_string(i + 1);
            info.value[i] = (float)reader->GetReal(name, key, info.value[i]);
        }
    }
}

inline bool WriteToINI(const std::string& filename) {
    std::ofstream iniFile(filename);
    if (!iniFile.is_open()) {
        LOGE("Error creating file: %s", filename.c_str());
        return false;
    }

    for (int i = 0; i < nodalNumber; i++) {
        std::string nodalName = "[" + array1[i] + "." + array2[0] + "]";
        iniFile << "\n" + nodalName + "\n";

        writeInfoToFile(iniFile, *box[i]);

        nodalName = "[" + array1[i] + "." + array2[2] + "]";
        iniFile << "\n" + nodalName + "\n";
        writeInfoToFile(iniFile, *slider_int[i]);

        iniFile << std::fixed << std::setprecision(6);

        nodalName = "[" + array1[i] + "." + array2[1] + "]";
        iniFile << "\n" + nodalName + "\n";
        writeInfoToFile(iniFile, *slider[i]);

        nodalName = "[" + array1[i] + "." + array2[3] + "]";
        iniFile << "\n" + nodalName + "\n";
        writeInfoToFile(iniFile, *color[i]);
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
    for (int i = 0; i < nodalNumber; i++) {
        std::string nodalName = array1[i] + "." + array2[0];
        RetrieveToMemory(&reader, box[i], nodalName);
        nodalName = array1[i] + "." + array2[1];
        RetrieveToMemory(&reader, slider[i], nodalName);
        nodalName = array1[i] + "." + array2[2];
        RetrieveToMemory(&reader, slider_int[i], nodalName);
        nodalName = array1[i] + "." + array2[3];
        RetrieveToMemory(&reader, color[i], nodalName);
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