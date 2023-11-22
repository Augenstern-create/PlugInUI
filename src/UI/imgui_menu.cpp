#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "imgui_menu.h"
#ifndef IMGUI_DISABLE
#include "imgui_internal.h"

namespace ImGui {
std::vector<std::pair<std::string, bool>> chbox_list;
std::vector<std::pair<std::string, int>> slid_list;
std::vector<std::pair<std::string, bool>> bottoms_chbox_list;

std::vector<std::pair<std::string, bool>> alternate_chbox_list;
std::vector<std::pair<std::string, int>> alternate_slid_list;
enum ToolBoxes { player, alternate };
static ToolBoxes toolboxes = ToolBoxes::player;
}  // namespace ImGui

// Helper to display a little (?) mark which shows a tooltip when hovered.
// In your own code you may want to display an actual icon if you are using a merged icon fonts (see docs/FONTS.md)
static void HelpMarker(const char* desc) {
    ImGui::TextDisabled("(?)");
    if (ImGui::BeginItemTooltip()) {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

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

void ImGui::ShowMenuWindow(bool* p_open) {
    IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing Dear ImGui context. Refer to examples app!");
    ImGuiWindowFlags window_flags = 0;
    // We specify a default position/size in case there's no data in the .ini file.
    // We only do it to make the demo applications a little more welcoming, but typically this isn't required.
    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 650, main_viewport->WorkPos.y + 20),
                            ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);

    // Main body of the Demo window starts here.
    if (!ImGui::Begin("Windows Menu", p_open, window_flags)) {
        // Early out if the window is collapsed, as an optimization.
        ImGui::End();
        return;
    }
    ShowExampleAppMenu();
    switch (toolboxes) {
        case ToolBoxes::player:
            ShowExampleAppPlayerInstall();
            break;  // 可选的
        case ToolBoxes::alternate:
            ShowExampleAppAlternateInstall();
            break;  // 可选的
        default:    // 可选的
            break;
    }
}

void ImGui::ShowExampleAppMenu() {
    static bool show_app_fuser_mode;
    static int show_app_language_selection;
    ImVec2 contentSize = ImGui::GetContentRegionAvail();
    float rectWidth = contentSize.x / 4;
    float rectHeight = contentSize.y;
    ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(41, 43, 56, 255));
    ImGui::BeginChild("CustomPanel", ImVec2(rectWidth, rectHeight), true);
    ImGui::Spacing();                   // 添加间隔
    ImGui::Dummy(ImVec2(0.0f, 20.0f));  // 添加更大的间隔
    float centerX = (rectWidth - rectWidth / 2) * 0.5f;
    ImGui::SetCursorPosX(centerX);
    // ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(50, 50, 50, 255));
    if (ImGui::Button("player", ImVec2(rectWidth / 2, 40))) {
        toolboxes = ToolBoxes::player;
    }
    ImGui::SetCursorPosX(centerX);
    if (ImGui::Button("alternate", ImVec2(rectWidth / 2, 40))) {
        toolboxes = ToolBoxes::alternate;
    }
    // ImGui::PopStyleColor();
    ImGui::SetCursorPosX(centerX);
    ImGui::RoundedCheckbox("Fuser modes", &show_app_fuser_mode, 0.5f);
    ImGui::SetCursorPosX(centerX);
    ImGui::RadioButton("Chinese writing", &show_app_language_selection, 1);
    ImGui::SameLine();
    ImGui::NewLine();
    ImGui::SetCursorPosX(centerX);
    ImGui::RadioButton("English", &show_app_language_selection, 0);
    ImGui::EndChild();
    ImGui::PopStyleColor();
}

void ImGui::ShowExampleAppMenuInitializelist() {
    chbox_list.push_back(std::make_pair("show_box", false));
    chbox_list.push_back(std::make_pair("show_skeleton", false));
    chbox_list.push_back(std::make_pair("show_bloodstain", false));
    chbox_list.push_back(std::make_pair("show_length", false));
    chbox_list.push_back(std::make_pair("show_player_name", false));
    chbox_list.push_back(std::make_pair("show_team_name", false));
    chbox_list.push_back(std::make_pair("show_handheld_weapon", false));
    chbox_list.push_back(std::make_pair("show_survival_level", false));
    chbox_list.push_back(std::make_pair("show_kill_count", false));
    chbox_list.push_back(std::make_pair("show_spectator_count", false));
    chbox_list.push_back(std::make_pair("show_team_number", false));
    chbox_list.push_back(std::make_pair("show_player_rank", false));
    chbox_list.push_back(std::make_pair("show_average_damage", false));
    chbox_list.push_back(std::make_pair("show_player_kda", false));
    chbox_list.push_back(std::make_pair("show_big_map_radar", false));
    chbox_list.push_back(std::make_pair("show_small_map_radar", false));
    chbox_list.push_back(std::make_pair("show_damage_stats", false));
    chbox_list.push_back(std::make_pair("show_smart_display", false));
    chbox_list.push_back(std::make_pair("show_aim_line", false));
    chbox_list.push_back(std::make_pair("show_nearby_enemies", false));
    chbox_list.push_back(std::make_pair("show_fov", false));
    chbox_list.push_back(std::make_pair("is_global", false));

    slid_list.push_back(std::make_pair("length_distance", 0));
    slid_list.push_back(std::make_pair("skeleton_distance", 0));
    slid_list.push_back(std::make_pair("name_distance", 0));
    slid_list.push_back(std::make_pair("handheld_distance", 0));
    slid_list.push_back(std::make_pair("info_distance", 0));
    slid_list.push_back(std::make_pair("skeleton_thickness", 0));
    slid_list.push_back(std::make_pair("box_thickness", 0));
    slid_list.push_back(std::make_pair("font_size", 0));
    slid_list.push_back(std::make_pair("danger_kill_count", 0));
    slid_list.push_back(std::make_pair("nearby_distance", 0));
    slid_list.push_back(std::make_pair("lock_opacity", 0));

    bottoms_chbox_list.push_back(std::make_pair("visual bone", false));
    bottoms_chbox_list.push_back(std::make_pair("invisible bone", false));
    bottoms_chbox_list.push_back(std::make_pair("fallen skeleton", false));
    bottoms_chbox_list.push_back(std::make_pair("down Blood", false));
    bottoms_chbox_list.push_back(std::make_pair("bot skeleton", false));
    bottoms_chbox_list.push_back(std::make_pair("ray color", false));
    bottoms_chbox_list.push_back(std::make_pair("collaborator skeleton", false));
    bottoms_chbox_list.push_back(std::make_pair("collaborator info", false));
    bottoms_chbox_list.push_back(std::make_pair("player name", false));
    bottoms_chbox_list.push_back(std::make_pair("player info", false));
    bottoms_chbox_list.push_back(std::make_pair("danger info", false));
    bottoms_chbox_list.push_back(std::make_pair("handheld weapon", false));

    alternate_chbox_list.push_back(std::make_pair("Vehicle Health", false));
    alternate_chbox_list.push_back(std::make_pair("Vehicle Fuel", false));
    alternate_chbox_list.push_back(std::make_pair("Vehicle Color", false));
    alternate_chbox_list.push_back(std::make_pair("Show Airdrop", false));
    alternate_chbox_list.push_back(std::make_pair("Airdrop Items", false));
    alternate_chbox_list.push_back(std::make_pair("Airdrop Color", false));
    alternate_chbox_list.push_back(std::make_pair("Show Box", false));
    alternate_chbox_list.push_back(std::make_pair("Box Items", false));
    alternate_chbox_list.push_back(std::make_pair("Box Color", false));
    alternate_chbox_list.push_back(std::make_pair("Throw Reminder", false));
    alternate_chbox_list.push_back(std::make_pair("Pull Pin Timer", false));
    alternate_chbox_list.push_back(std::make_pair("Throw Color", false));

    alternate_chbox_list.push_back(std::make_pair("Materials Group 1", false));
    alternate_chbox_list.push_back(std::make_pair("Materials Group 2", false));
    alternate_chbox_list.push_back(std::make_pair("Materials Group 3", false));
    alternate_chbox_list.push_back(std::make_pair("Materials Group 4", false));

    alternate_slid_list.push_back(std::make_pair("Vehicle Distance", 0));
    alternate_slid_list.push_back(std::make_pair("Vehicle Font Size", 0));
    alternate_slid_list.push_back(std::make_pair("Airdrop Distance", 0));
    alternate_slid_list.push_back(std::make_pair("Airdrop Font Size", 0));
    alternate_slid_list.push_back(std::make_pair("Box Distance", 0));
    alternate_slid_list.push_back(std::make_pair("Box Font Size", 0));
    alternate_slid_list.push_back(std::make_pair("Throw Distance", 0));
    alternate_slid_list.push_back(std::make_pair("Throw Font Size", 0));
    alternate_slid_list.push_back(std::make_pair("Supply Distance", 0));
    alternate_slid_list.push_back(std::make_pair("Supply Font Size", 0));
}

static void ImGui::ShowExampleAppPlayerInstall() {
    ImVec2 contentSize = ImGui::GetContentRegionAvail();
    float rectWidth = (contentSize.x / 4) * 3;
    float rectHeight = contentSize.y;
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(41, 43, 56, 255));
    ImGui::BeginChild("CustomPanel2", ImVec2(rectWidth, rectHeight), true);
    ImGui::Columns(3, nullptr, false);
    float adjustedColumnWidth = rectWidth / 4.0f;
    ImGui::SetColumnWidth(0, adjustedColumnWidth);
    ImGui::SetColumnWidth(1, adjustedColumnWidth);
    ImGui::SetColumnWidth(2, adjustedColumnWidth * 2.0f);

    size_t boolSettings = 0;
    size_t intSettings = 0;
    for (size_t i = 0; i < chbox_list.size() + slid_list.size(); i++) {
        if (i % 3 != 2) {
            bool value_box = chbox_list[boolSettings].second;
            ImGui::Checkbox(chbox_list[boolSettings].first.c_str(), &value_box);
            ImGui::NextColumn();
            chbox_list[boolSettings].second = value_box;
            boolSettings++;
        } else {
            int& value_slider = slid_list[intSettings].second;
            ImGui::SliderInt(slid_list[intSettings].first.c_str(), &value_slider, 0, 100);
            ImGui::NextColumn();
            intSettings++;
        }
    }

    ImGui::Columns(1);
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::NextColumn();
    ImGui::Separator();  // 添加一条横线

    ImGui::Columns(4, nullptr, false);
    ImGui::SetColumnWidth(0, adjustedColumnWidth);
    ImGui::SetColumnWidth(1, adjustedColumnWidth);
    ImGui::SetColumnWidth(2, adjustedColumnWidth);
    ImGui::SetColumnWidth(3, adjustedColumnWidth);
    ImGui::Spacing();
    ImGui::Spacing();
    for (size_t i = 0; i < bottoms_chbox_list.size(); i++) {
        bool value_box = bottoms_chbox_list[i].second;
        ImGui::Checkbox(bottoms_chbox_list[i].first.c_str(), &value_box);
        ImGui::NextColumn();
        bottoms_chbox_list[i].second = value_box;
    }
    ImGui::Columns(1);
    ImGui::NextColumn();
    if (ImGui::Button("Save Settings", ImVec2(rectWidth / 4, 40))) {
    }
    ImGui::EndChild();
}

static ImVec2 ImVector2Add(const ImVec2& a, const ImVec2& b) { return ImVec2(a.x + b.x, a.y + b.y); }

void ImGui::ShowExampleAppAlternateInstall() {
    ImVec2 contentSize = ImGui::GetContentRegionAvail();
    float rectWidth = (contentSize.x / 4) * 3;
    float rectHeight = contentSize.y;
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(41, 43, 56, 255));
    ImGui::BeginChild("CustomPanel2", ImVec2(rectWidth, rectHeight), true);

    size_t boolSettings = 0;
    size_t intSettings = 0;
    int currentColumn = 0;
    for (size_t i = 0; i < (alternate_chbox_list.size() - 4) / 3; i++) {
        for (size_t j = 0; j < 3; j++) {
            if (currentColumn != 3) {
                ImGui::Columns(3, nullptr, false);
                float adjustedColumnWidth = rectWidth / 3.0f;
                ImGui::SetColumnWidth(0, adjustedColumnWidth);
                ImGui::SetColumnWidth(1, adjustedColumnWidth);
                ImGui::SetColumnWidth(2, adjustedColumnWidth);
                currentColumn = 3;
            }
            bool value_box = alternate_chbox_list[boolSettings].second;
            ImGui::Checkbox(alternate_chbox_list[boolSettings].first.c_str(), &value_box);
            ImGui::NextColumn();
            alternate_chbox_list[boolSettings].second = value_box;
            boolSettings++;
        }
        for (size_t j = 0; j < 2; j++) {
            if (currentColumn != 2) {
                ImGui::Columns(2, nullptr, false);
                float adjustedColumnWidth = rectWidth / 2.0f;
                ImGui::SetColumnWidth(0, adjustedColumnWidth);
                ImGui::SetColumnWidth(1, adjustedColumnWidth);
                currentColumn = 2;
            }
            int& value_slider = alternate_slid_list[intSettings].second;
            ImGui::SliderInt(alternate_slid_list[intSettings].first.c_str(), &value_slider, 0, 100);
            ImGui::NextColumn();
            intSettings++;
        }
    }
    for (size_t i = 0; i < 4; i++) {
        if (currentColumn != 4) {
            ImGui::Columns(4, nullptr, false);
            float adjustedColumnWidth = rectWidth / 4.0f;
            ImGui::SetColumnWidth(0, adjustedColumnWidth);
            ImGui::SetColumnWidth(1, adjustedColumnWidth);
            ImGui::SetColumnWidth(2, adjustedColumnWidth);
            ImGui::SetColumnWidth(2, adjustedColumnWidth);
            currentColumn = 4;
        }
        bool value_box = alternate_chbox_list[boolSettings].second;
        ImGui::Checkbox(alternate_chbox_list[boolSettings].first.c_str(), &value_box);
        ImGui::NextColumn();
        alternate_chbox_list[boolSettings].second = value_box;
        boolSettings++;
    }
    for (size_t j = 0; j < 2; j++) {
        if (currentColumn != 2) {
            ImGui::Columns(2, nullptr, false);
            float adjustedColumnWidth = rectWidth / 2.0f;
            ImGui::SetColumnWidth(0, adjustedColumnWidth);
            ImGui::SetColumnWidth(1, adjustedColumnWidth);
            currentColumn = 2;
        }
        int& value_slider = alternate_slid_list[intSettings].second;
        ImGui::SliderInt(alternate_slid_list[intSettings].first.c_str(), &value_slider, 0, 100);
        ImGui::NextColumn();
        intSettings++;
    }

    ImGui::Columns(1);
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::NextColumn();
    ImGui::Separator();  // 添加一条横线
    ImGui::NextColumn();
    if (ImGui::Button("Save Settings", ImVec2(rectWidth / 4, 40))) {
    }
    ImGui::EndChild();
}

#endif  // #ifndef IMGUI_DISABLE