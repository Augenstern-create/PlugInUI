#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "imgui_menu.h"
#ifndef IMGUI_DISABLE
#include "imgui_internal.h"
#include <cstring>
#include <tuple>
#include "imgui_module.h"

namespace ImGui {
std::vector<std::tuple<std::string, std::string, bool>> chbox_list_;
std::vector<std::tuple<std::string, std::string, float>> slider_list_;
std::vector<std::tuple<std::string, std::string, ImVec4>> color_picker_list_;
static ImVec4 default_color_ = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
static int multilingualism_ = 1;
static unsigned int but_player_img_id_;
enum ToolBoxes { player, alternate };
static ToolBoxes toolboxes_ = ToolBoxes::player;

}  // namespace ImGui

typedef void (*ImGuiMenuMarkerCallback)(const char* file, int line, const char* section, void* user_data);
extern ImGuiMenuMarkerCallback GImGuiMenuMarkerCallback;
extern void* GImGuiMenuMarkerCallbackUserData;
ImGuiMenuMarkerCallback GImGuiMenuMarkerCallback = NULL;
void* GImGuiMenuMarkerCallbackUserData = NULL;
#define IMGUI_MENU_MARKER(section)                                                                   \
    do {                                                                                             \
        if (GImGuiMenuMarkerCallback != NULL)                                                        \
            GImGuiMenuMarkerCallback(__FILE__, __LINE__, section, GImGuiMenuMarkerCallbackUserData); \
    } while (0)

void ImGui::ShowExampleAppMenuInitializelist() {
    chbox_list_.push_back(std::tuple<std::string, std::string, bool>(u8"显示方框", "show_box", false));
    chbox_list_.push_back(std::tuple<std::string, std::string, bool>(u8"显示骨骼", "show_skeleton", false));
    chbox_list_.push_back(std::tuple<std::string, std::string, bool>(u8"显示血迹", "show_bloodstain", false));
    chbox_list_.push_back(std::tuple<std::string, std::string, bool>(u8"显示长度", "show_length", false));
    chbox_list_.push_back(std::tuple<std::string, std::string, bool>(u8"显示视野", "show fov", false));
    chbox_list_.push_back(std::tuple<std::string, std::string, bool>(u8"全局开关", "is global", false));
    chbox_list_.push_back(std::tuple<std::string, std::string, bool>(u8"车辆健康", "Vehicle Health", false));
    chbox_list_.push_back(std::tuple<std::string, std::string, bool>(u8"车辆燃料", "Vehicle Fuel", false));
    chbox_list_.push_back(std::tuple<std::string, std::string, bool>(u8"显示空投", "Show Airdrop", false));
    chbox_list_.push_back(std::tuple<std::string, std::string, bool>(u8"空投物品", "Airdrop Items", false));
    chbox_list_.push_back(std::tuple<std::string, std::string, bool>(u8"显示箱子", "Show Box", false));
    chbox_list_.push_back(std::tuple<std::string, std::string, bool>(u8"箱子物品", "Box Items", false));
    chbox_list_.push_back(std::tuple<std::string, std::string, bool>(u8"投掷提醒", "Throw Reminder", false));
    chbox_list_.push_back(std::tuple<std::string, std::string, bool>(u8"拉销计时器", "Pull Pin Timer", false));
    chbox_list_.push_back(std::tuple<std::string, std::string, bool>(u8"显示击杀数", "show_kill_count", false));
    chbox_list_.push_back(std::tuple<std::string, std::string, bool>(u8"显示瞄准线", "show aim line", false));
    chbox_list_.push_back(std::tuple<std::string, std::string, bool>(u8"显示玩家名称", "show_player_name", false));
    chbox_list_.push_back(std::tuple<std::string, std::string, bool>(u8"显示队伍名称", "show_team_name", false));
    chbox_list_.push_back(std::tuple<std::string, std::string, bool>(u8"显示手持武器", "show_handheld_weapon", false));
    chbox_list_.push_back(std::tuple<std::string, std::string, bool>(u8"显示生存等级", "show_survival_level", false));
    chbox_list_.push_back(std::tuple<std::string, std::string, bool>(u8"显示旁观者数", "show_spectator_count", false));
    chbox_list_.push_back(std::tuple<std::string, std::string, bool>(u8"显示队伍编号", "show_team_number", false));
    chbox_list_.push_back(std::tuple<std::string, std::string, bool>(u8"显示玩家排名", "show_player_rank", false));
    chbox_list_.push_back(std::tuple<std::string, std::string, bool>(u8"显示平均伤害", "show_average_damage", false));
    chbox_list_.push_back(std::tuple<std::string, std::string, bool>(u8"显示玩家KDA", "show_player_kda", false));
    chbox_list_.push_back(std::tuple<std::string, std::string, bool>(u8"显示伤害统计", "show_damage_stats", false));
    chbox_list_.push_back(std::tuple<std::string, std::string, bool>(u8"显示智能显示", "show smart display", false));
    chbox_list_.push_back(std::tuple<std::string, std::string, bool>(u8"显示附近敌人", "show nearby enemies", false));
    chbox_list_.push_back(std::tuple<std::string, std::string, bool>(u8"显示大地图雷达", "show_big_map_radar", false));
    chbox_list_.push_back(
        std::tuple<std::string, std::string, bool>(u8"显示小地图雷达", "show_small_map_radar", false));

    slider_list_.push_back(std::tuple<std::string, std::string, float>(u8"透视距离", "length_distance", 0.0f));
    slider_list_.push_back(std::tuple<std::string, std::string, float>(u8"骨骼距离", "skeleton_distance", 0.0f));
    slider_list_.push_back(std::tuple<std::string, std::string, float>(u8"名称距离", "name_distance", 0.0f));
    slider_list_.push_back(std::tuple<std::string, std::string, float>(u8"手持物品距离", "handheld_distance", 0.0f));
    slider_list_.push_back(std::tuple<std::string, std::string, float>(u8"信息距离", "info_distance", 0.0f));
    slider_list_.push_back(std::tuple<std::string, std::string, float>(u8"骨骼厚度", "skeleton_thickness", 0.0f));
    slider_list_.push_back(std::tuple<std::string, std::string, float>(u8"车辆距离", "Vehicle Distance", 0.0f));
    slider_list_.push_back(std::tuple<std::string, std::string, float>(u8"车辆字体大小", "Vehicle Font Size", 0.0f));
    slider_list_.push_back(std::tuple<std::string, std::string, float>(u8"空投距离", "Airdrop Distance", 0.0f));
    slider_list_.push_back(std::tuple<std::string, std::string, float>(u8"空投字体大小", "Airdrop Font Size", 0.0f));
    slider_list_.push_back(std::tuple<std::string, std::string, float>(u8"箱子距离", "Box Distance", 0.0f));
    slider_list_.push_back(std::tuple<std::string, std::string, float>(u8"箱子字体大小", "Box Font Size", 0.0f));
    slider_list_.push_back(std::tuple<std::string, std::string, float>(u8"方框厚度", "box_thickness", 0.0f));
    slider_list_.push_back(std::tuple<std::string, std::string, float>(u8"字体大小", "font_size", 0.0f));
    slider_list_.push_back(std::tuple<std::string, std::string, float>(u8"危险击杀数", "danger_kill_count", 0.0f));
    slider_list_.push_back(std::tuple<std::string, std::string, float>(u8"附近距离", "nearby_distance", 0.0f));
    slider_list_.push_back(std::tuple<std::string, std::string, float>(u8"锁定透明度", "lock_opacity", 0.0f));
    slider_list_.push_back(std::tuple<std::string, std::string, float>(u8"投掷距离", "Throw Distance", 0.0f));
    slider_list_.push_back(std::tuple<std::string, std::string, float>(u8"投掷字体大小", "Throw Font Size", 0.0f));
    slider_list_.push_back(std::tuple<std::string, std::string, float>(u8"补给距离", "Supply Distance", 0.0f));
    slider_list_.push_back(std::tuple<std::string, std::string, float>(u8"补给字体大小", "Supply Font Size", 0.0f));

    color_picker_list_.push_back(
        std::tuple<std::string, std::string, ImVec4>(u8"可视骨骼", "visual bone", default_color_));
    color_picker_list_.push_back(
        std::tuple<std::string, std::string, ImVec4>(u8"不可视骨骼", "invisible bone", default_color_));
    color_picker_list_.push_back(
        std::tuple<std::string, std::string, ImVec4>(u8"倒地骨骼", "fallen skeleton", default_color_));
    color_picker_list_.push_back(
        std::tuple<std::string, std::string, ImVec4>(u8"倒地血迹", "down Blood", default_color_));
    color_picker_list_.push_back(
        std::tuple<std::string, std::string, ImVec4>(u8"机器人骨骼", "bot skeleton", default_color_));
    color_picker_list_.push_back(
        std::tuple<std::string, std::string, ImVec4>(u8"射线颜色", "ray color", default_color_));
    color_picker_list_.push_back(
        std::tuple<std::string, std::string, ImVec4>(u8"协作者骨骼", "collaborator skeleton", default_color_));
    color_picker_list_.push_back(
        std::tuple<std::string, std::string, ImVec4>(u8"协作者信息", "collaborator info", default_color_));
    color_picker_list_.push_back(
        std::tuple<std::string, std::string, ImVec4>(u8"玩家名称", "player name", default_color_));
    color_picker_list_.push_back(
        std::tuple<std::string, std::string, ImVec4>(u8"玩家信息", "player info", default_color_));
    color_picker_list_.push_back(
        std::tuple<std::string, std::string, ImVec4>(u8"危险信息", "danger info", default_color_));
    color_picker_list_.push_back(
        std::tuple<std::string, std::string, ImVec4>(u8"手持武器", "handheld weapon", default_color_));
    color_picker_list_.push_back(
        std::tuple<std::string, std::string, ImVec4>(u8"车辆颜色", "Vehicle Color", default_color_));
    color_picker_list_.push_back(
        std::tuple<std::string, std::string, ImVec4>(u8"空投颜色", "Airdrop Color", default_color_));
    color_picker_list_.push_back(
        std::tuple<std::string, std::string, ImVec4>(u8"箱子颜色", "Box Color", default_color_));
    color_picker_list_.push_back(
        std::tuple<std::string, std::string, ImVec4>(u8"投掷颜色", "Throw Color", default_color_));
    color_picker_list_.push_back(
        std::tuple<std::string, std::string, ImVec4>(u8"材料组 1", "Materials Group 1", default_color_));
    color_picker_list_.push_back(
        std::tuple<std::string, std::string, ImVec4>(u8"材料组 2", "Materials Group 2", default_color_));
    color_picker_list_.push_back(
        std::tuple<std::string, std::string, ImVec4>(u8"材料组 3", "Materials Group 3", default_color_));
    color_picker_list_.push_back(
        std::tuple<std::string, std::string, ImVec4>(u8"材料组 4", "Materials Group 4", default_color_));
}

void ImGui::SubFormComfig1(const char* name, float width, float height,
                           std::vector<std::tuple<std::string, std::string, bool>>* chbox_list) {
    ImGui::BeginChild(name, ImVec2(width, height), false, ImGuiWindowFlags_None);
    ImVec2 childPos = ImGui::GetWindowPos();
    ImVec2 childSize = ImGui::GetWindowSize();
    RenderRoundedRect(ImGui::GetWindowDrawList(), childPos, ImVec2(childPos.x + childSize.x, childPos.y + childSize.y),
                      10.0f);
    ImGui::Columns(2, nullptr, false);
    ImGui::SetColumnWidth(0, width / 2);
    ImGui::SetColumnWidth(1, width / 2);
    for (size_t i = 0; i < chbox_list->size(); i++) {
        bool& value_slider = std::get<2>((*chbox_list)[i]);
        std::string checkbox_name =
            (multilingualism_ == 1) ? std::get<1>((*chbox_list)[i]) : std::get<0>((*chbox_list)[i]);
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
        ImGui::RenderCustomCheckbox(checkbox_name.c_str(), &value_slider);
        ImGui::NextColumn();
    }
    ImGui::Columns(1);
    ImGui::EndChild();
}
void ImGui::SubFormComfig2(const char* name, float width, float height,
                           std::vector<std::tuple<std::string, std::string, float>>* slider_list) {
    ImGui::BeginChild(name, ImVec2(width, height), false, ImGuiWindowFlags_None);
    ImVec2 childPos = ImGui::GetWindowPos();
    ImVec2 childSize = ImGui::GetWindowSize();
    RenderRoundedRect(ImGui::GetWindowDrawList(), childPos, ImVec2(childPos.x + childSize.x, childPos.y + childSize.y),
                      10.0f);
    for (size_t i = 0; i < slider_list->size(); i++) {
        float& value_slider = std::get<2>((*slider_list)[i]);
        std::string checkbox_name =
            (multilingualism_ == 1) ? std::get<1>((*slider_list)[i]) : std::get<0>((*slider_list)[i]);
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
        ImGui::RenderCustomSlider(checkbox_name.c_str(), &value_slider, 0.0f, 100.0f);
        ImGui::NextColumn();
    }

    ImGui::EndChild();
}
void ImGui::SubFormComfig3(const char* name, float width, float height,
                           std::vector<std::tuple<std::string, std::string, int>>* slider_list) {
    ImGui::BeginChild(name, ImVec2(width, height), false, ImGuiWindowFlags_None);
    ImVec2 childPos = ImGui::GetWindowPos();
    ImVec2 childSize = ImGui::GetWindowSize();
    RenderRoundedRect(ImGui::GetWindowDrawList(), childPos, ImVec2(childPos.x + childSize.x, childPos.y + childSize.y),
                      10.0f);
    ImGui::Columns(2, nullptr, false);
    ImGui::SetColumnWidth(0, width / 2);
    for (size_t i = 0; i < slider_list->size(); i++) {
        int& value_slider = std::get<2>((*slider_list)[i]);
        std::string checkbox_name =
            (multilingualism_ == 1) ? std::get<1>((*slider_list)[i]) : std::get<0>((*slider_list)[i]);
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
        ImGui::RenderCustomSliderint(checkbox_name.c_str(), &value_slider, 0, 100);
        ImGui::NextColumn();
    }
    ImGui::Columns(1);
    ImGui::EndChild();
}

void ImGui::SubFormComfig4(const char* name, float width, float height,
                           std::vector<std::tuple<std::string, std::string, ImVec4>>* clore_list) {
    ImGui::BeginChild(name, ImVec2(width, height), false, ImGuiWindowFlags_None);
    ImVec2 childPos = ImGui::GetWindowPos();
    ImVec2 childSize = ImGui::GetWindowSize();
    RenderRoundedRect(ImGui::GetWindowDrawList(), childPos, ImVec2(childPos.x + childSize.x, childPos.y + childSize.y),
                      10.0f);
    ImGui::Columns(3, nullptr, false);
    ImGui::SetColumnWidth(0, width / 3);
    ImGui::SetColumnWidth(1, width / 3);
    ImGui::SetColumnWidth(2, width / 3);
    for (size_t i = 0; i < clore_list->size(); i++) {
        ImVec4& value_clore = std::get<2>((*clore_list)[i]);
        std::string checkbox_name =
            (multilingualism_ == 1) ? std::get<1>((*clore_list)[i]) : std::get<0>((*clore_list)[i]);
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
        ImGui::ColorEdit3(checkbox_name.c_str(), (float*)&value_clore, ImGuiColorEditFlags_NoInputs);
        ImGui::NextColumn();
    }
    ImGui::Columns(1);
    ImGui::EndChild();
}

void ImGui::SubFormMenuBar(float width, float height) {
    ImGui::BeginChild("MenuPanel", ImVec2(width, height), false, ImGuiWindowFlags_None);
    ImVec2 childPos = ImGui::GetWindowPos();
    ImVec2 childSize = ImGui::GetWindowSize();
    RenderRoundedRect(ImGui::GetWindowDrawList(), childPos, ImVec2(childPos.x + childSize.x, childPos.y + childSize.y),
                      10.0f);
    bool isButtonFocused = ImGui::IsItemFocused();
    float availWidth = ImGui::GetContentRegionAvail().x;
    float but_width = width * 0.33f;
    float buttonX = (availWidth - but_width) * 0.5f;

    ImGui::Dummy(ImVec2(0.0f, but_width));
    ImGui::SetCursorPosX(buttonX);

    const ImU32 col = GetColorU32(ImGuiCol_ButtonActive, (toolboxes_ == ToolBoxes::player) ? 0.0f : 0.0f);
    if (ImGui::RenderCustomButton("pr", (ImTextureID)but_player_img_id_, ImVec2(but_width, but_width),
                                  ImVec4(0, 0, 0, 0), ImVec4(1, 1, 1, 1), col)) {
        toolboxes_ = ToolBoxes::player;
    }
    ImGui::Dummy(ImVec2(0.0f, but_width));
    ImGui::SetCursorPosX(buttonX);
    const ImU32 col2 = GetColorU32(ImGuiCol_ButtonActive, (toolboxes_ == ToolBoxes::alternate) ? 0.0f : 0.0f);
    if (ImGui::RenderCustomButton("prs", (ImTextureID)but_player_img_id_, ImVec2(but_width, but_width),
                                  ImVec4(0, 0, 0, 0), ImVec4(1, 1, 1, 1), col2)) {
        toolboxes_ = ToolBoxes::alternate;
    }

    ImGui::EndChild();
}
void ImGui::SubFormSecondaryMenu(float width, float height) {
    ImGui::BeginChild("MenuPanel2", ImVec2(width, height), false, ImGuiWindowFlags_None);
    ImVec2 childPos = ImGui::GetWindowPos();
    ImVec2 childSize = ImGui::GetWindowSize();
    RenderRoundedRect(ImGui::GetWindowDrawList(), childPos, ImVec2(childPos.x + childSize.x, childPos.y + childSize.y),
                      10.0f);
    ImGui::EndChild();
}

void ImGui::SubFormFunctionBlock(float width, float height) {
    ImVec2 MenuPanel4_size = ImVec2(width * 2 / 3, height);
    ImVec2 cbox_size = ImVec2(MenuPanel4_size.x * 2 / 3 - 10, height * 2 / 3);
    ImVec2 slider_size = ImVec2(MenuPanel4_size.x / 3 - 30, height * 2 / 3);
    ImVec2 color_selection_size = ImVec2(cbox_size.x + slider_size.x + 9.0f, (height / 3) - 10);
    ImVec2 character_size = ImVec2(MenuPanel4_size.x / 2 - 15, MenuPanel4_size.y);
    ImGui::BeginChild("MenuPanel3", ImVec2(width, height), false, ImGuiWindowFlags_None);
    ImGui::Columns(2, nullptr, false);
    ImGui::SetColumnWidth(0, MenuPanel4_size.x);
    ImGui::SetColumnWidth(1, MenuPanel4_size.x / 2);
    ImGui::BeginChild("MenuPanel4", ImVec2(MenuPanel4_size.x, MenuPanel4_size.y), false, ImGuiWindowFlags_None);
    ImGui::Columns(2, nullptr, false);
    ImGui::SetColumnWidth(0, MenuPanel4_size.x * 2 / 3);
    ImGui::SetColumnWidth(1, MenuPanel4_size.x / 3);
    SubFormComfig1("config1", cbox_size.x, cbox_size.y, &chbox_list_);
    ImGui::NextColumn();
    SubFormComfig2("config2", slider_size.x, slider_size.y, &slider_list_);
    ImGui::Columns(1);
    ImGui::SetCursorPosX(8.0f);
    SubFormComfig4("config3", color_selection_size.x, color_selection_size.y, &color_picker_list_);
    ImGui::NextColumn();
    ImGui::EndChild();
    ImGui::NextColumn();

    ImGui::BeginChild("MenuPanel5", ImVec2(character_size.x, character_size.y), false, ImGuiWindowFlags_None);
    ImVec2 childPos = ImGui::GetWindowPos();
    ImVec2 childSize = ImGui::GetWindowSize();
    RenderRoundedRect(ImGui::GetWindowDrawList(), childPos, ImVec2(childPos.x + childSize.x, childPos.y + childSize.y),
                      10.0f);
    ImGui::EndChild();
    ImGui::EndChild();
    ImGui::Columns(1);
}

void ImGui::ShowMenuWindow(bool* p_open, ImVec2 display_size) {
    IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing Dear ImGui context. Refer to examples app!");
    ImGuiWindowFlags window_flags = 0;
    ImGui::SetNextWindowPos({display_size.x / 6, display_size.y / 6}, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(display_size.x * 2 / 3, display_size.y * 2 / 3), ImGuiCond_FirstUseEver);

    if (!ImGui::Begin("Windows Menu", p_open, window_flags)) {
        ImGui::End();
        return;
    }
    float from_width = 100;
    float from_height = 40;
    ImVec2 from_size = ImGui::GetContentRegionAvail();
    ImVec2 from_menu = ImVec2(from_width - 10, from_size.y);
    ImVec2 from_menu2 = ImVec2(from_size.x - from_width - 10, from_height);
    ImVec2 from_menu3 = ImVec2(from_size.x - from_width - 10, from_size.y - from_height - 10);
    ImGui::Columns(2, nullptr, false);
    ImGui::SetColumnWidth(0, from_width);
    ImGui::SetColumnWidth(1, from_size.x - from_width);

    SubFormMenuBar(from_menu.x, from_menu.y);
    ImGui::NextColumn();
    SubFormSecondaryMenu(from_menu2.x, from_menu2.y);
    SubFormFunctionBlock(from_menu3.x, from_menu3.y);
    ImGui::Columns(1);
    ImGui::End();
}

#endif  // #ifndef IMGUI_DISABLE