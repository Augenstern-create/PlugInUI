#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "imgui_menu.h"
#ifndef IMGUI_DISABLE
#include "imgui_internal.h"
#include <cstring>
#include <tuple>
#include "imgui_module.h"
#include "game/LanguageSwitch.h"

namespace ImGui {
std::vector<std::tuple<std::string, std::string, bool>> chbox_list_;
std::vector<std::tuple<std::string, std::string, float>> slider_list_;
std::vector<std::tuple<std::string, std::string, ImVec4>> color_picker_list_;
static ImVec4 default_color_ = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
static int multilingualism_ = 0;
static ImTextureID but_player_img_id_;
enum ToolBoxes { player, alternate };
static ToolBoxes toolboxes_ = ToolBoxes::player;

}  // namespace ImGui

void ImGui::SubFormComfig1(const char* name, float width, float height, std::vector<std::tuple<std::string, std::string, bool>>* chbox_list) {
    ImGui::BeginChild(name, ImVec2(width, height), false, ImGuiWindowFlags_None);
    ImVec2 childPos = ImGui::GetWindowPos();
    ImVec2 childSize = ImGui::GetWindowSize();
    RenderRoundedRect(ImGui::GetWindowDrawList(), childPos, ImVec2(childPos.x + childSize.x, childPos.y + childSize.y), 10.0f);
    ImGui::Columns(2, nullptr, false);
    ImGui::SetColumnWidth(0, width / 2);
    ImGui::SetColumnWidth(1, width / 2);
    for (auto parameter : ComponentsLists::ShboxLists) {
        std::string checkbox_name = gameData.Config.Font.multilingualism == 0 ? parameter.ChineseName : parameter.EnglishName;
        // ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
        ImGui::SetCursorPos({ImGui::GetCursorPos() + 10});
        ImGui::RenderCustomCheckbox(checkbox_name.c_str(), parameter.value, gameData.Config.Slider.fontSize);
        ImGui::NextColumn();
    }
    ImGui::Columns(1);
    ImGui::EndChild();
}
void ImGui::SubFormComfig2(const char* name, float width, float height, std::vector<std::tuple<std::string, std::string, float>>* slider_list) {
    ImGui::BeginChild(name, ImVec2(width, height), false, ImGuiWindowFlags_None);
    ImVec2 childPos = ImGui::GetWindowPos();
    ImVec2 childSize = ImGui::GetWindowSize();
    RenderRoundedRect(ImGui::GetWindowDrawList(), childPos, ImVec2(childPos.x + childSize.x, childPos.y + childSize.y), 10.0f);
    for (auto parameter : ComponentsLists::SliderLists) {
        std::string checkbox_name = gameData.Config.Font.multilingualism == 0 ? parameter.ChineseName : parameter.EnglishName;
        ImGui::SetCursorPos({ImGui::GetCursorPos() + 10});
        ImGui::RenderCustomSlider(checkbox_name.c_str(), parameter.value, parameter.min, parameter.max, gameData.Config.Slider.fontSize);
        ImGui::NextColumn();
    }
    ImGui::EndChild();
}
void ImGui::SubFormComfig3(const char* name, float width, float height, std::vector<std::tuple<std::string, std::string, int>>* slider_list) {
    ImGui::BeginChild(name, ImVec2(width, height), false, ImGuiWindowFlags_None);
    ImVec2 childPos = ImGui::GetWindowPos();
    ImVec2 childSize = ImGui::GetWindowSize();
    RenderRoundedRect(ImGui::GetWindowDrawList(), childPos, ImVec2(childPos.x + childSize.x, childPos.y + childSize.y), 10.0f);
    ImGui::Columns(2, nullptr, false);
    ImGui::SetColumnWidth(0, width / 2);

    for (auto parameter : ComponentsLists::SliderIntLists) {
        std::string checkbox_name = gameData.Config.Font.multilingualism == 0 ? parameter.ChineseName : parameter.EnglishName;
        ImGui::SetCursorPos({ImGui::GetCursorPos() + 10});
        ImGui::RenderCustomSliderint(checkbox_name.c_str(), parameter.value, parameter.min, parameter.max, gameData.Config.Slider.fontSize);
        ImGui::NextColumn();
    }
    ImGui::Columns(1);
    ImGui::EndChild();
}

void ImGui::SubFormComfig4(const char* name, float width, float height, std::vector<std::tuple<std::string, std::string, ImVec4>>* clore_list) {
    ImGui::BeginChild(name, ImVec2(width, height), false, ImGuiWindowFlags_None);
    ImVec2 childPos = ImGui::GetWindowPos();
    ImVec2 childSize = ImGui::GetWindowSize();
    RenderRoundedRect(ImGui::GetWindowDrawList(), childPos, ImVec2(childPos.x + childSize.x, childPos.y + childSize.y), 10.0f);
    ImGui::Columns(3, nullptr, false);
    ImGui::SetColumnWidth(0, width / 3);
    ImGui::SetColumnWidth(1, width / 3);
    ImGui::SetColumnWidth(2, width / 3);

    for (auto parameter : ComponentsLists::ColorList) {
        std::string checkbox_name = gameData.Config.Font.multilingualism == 0 ? parameter.ChineseName : parameter.EnglishName;
        ImGui::SetCursorPos({ImGui::GetCursorPos() + 10});
        ImGui::ColorEdit4(checkbox_name.c_str(), parameter.value, ImGuiColorEditFlags_NoInputs);
        ImGui::NextColumn();
    }
    ImGui::Columns(1);
    ImGui::EndChild();
}

void ImGui::SubFormMenuBar(float width, float height) {
    ImGui::BeginChild("MenuPanel", ImVec2(width, height), false, ImGuiWindowFlags_None);
    ImVec2 childPos = ImGui::GetWindowPos();
    ImVec2 childSize = ImGui::GetWindowSize();
    RenderRoundedRect(ImGui::GetWindowDrawList(), childPos, ImVec2(childPos.x + childSize.x, childPos.y + childSize.y), 10.0f);
    bool isButtonFocused = ImGui::IsItemFocused();
    float availWidth = ImGui::GetContentRegionAvail().x;
    float but_width = width * 0.33f;
    float buttonX = (availWidth - but_width) * 0.5f;

    ImGui::Dummy(ImVec2(0.0f, but_width));
    ImGui::SetCursorPosX(buttonX);

    const ImU32 col = GetColorU32(ImGuiCol_ButtonActive, (toolboxes_ == ToolBoxes::player) ? 0.0f : 0.0f);
    if (ImGui::RenderCustomButton("pr", but_player_img_id_, ImVec2(but_width, but_width), ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1.0f, 1.0f, 1.0f, 1.0f),
                                  col)) {
        toolboxes_ = ToolBoxes::player;
    }
    ImGui::Dummy(ImVec2(0.0f, but_width));
    ImGui::SetCursorPosX(buttonX);
    const ImU32 col2 = GetColorU32(ImGuiCol_ButtonActive, (toolboxes_ == ToolBoxes::alternate) ? 0.0f : 0.0f);
    if (ImGui::RenderCustomButton("prs", but_player_img_id_, ImVec2(but_width, but_width), ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1.0f, 1.0f, 1.0f, 1.0f),
                                  col2)) {
        toolboxes_ = ToolBoxes::alternate;
    }

    if (gameData.Config.Font.multilingualism == 0) {
        bool islanguage1 = true;
        bool islanguage2 = false;
        ImGui::SetCursorPos({0.0f, height - 120.0f});
        bool language1 = ImGui::RenderCustomCheckbox("中文", &islanguage1, gameData.Config.Slider.fontSize);
        ImGui::SetCursorPos({0.0f, height - 80.0f});
        bool language2 = ImGui::RenderCustomCheckbox("English", &islanguage2, gameData.Config.Slider.fontSize);
        if (islanguage1 != true || islanguage2 != false) {
            gameData.Config.Font.multilingualism = 1;
        }
    } else if (gameData.Config.Font.multilingualism == 1) {
        bool islanguage1 = false;
        bool islanguage2 = true;
        ImGui::SetCursorPos({0.0f, height - 120.0f});
        bool language1 = ImGui::RenderCustomCheckbox("中文", &islanguage1, gameData.Config.Slider.fontSize);
        ImGui::SetCursorPos({availWidth * 0.125f, height - 80.0f});
        bool language2 = ImGui::RenderCustomCheckbox("English", &islanguage2, gameData.Config.Slider.fontSize);
        if (islanguage1 != false || islanguage2 != true) {
            gameData.Config.Font.multilingualism = 0;
        }
    }

    std::string butName = gameData.Config.Font.multilingualism == 0 ? "保存设置" : "Save Settings";
    ImGui::SetCursorPos({availWidth * 0.125f, height - 40.0f});
    if (ImGui::Button(butName.c_str(), {availWidth * 0.75f, 24.0f})) {
        if (ComponentsLists::SaveSet()) {
            LOGI("Saved successfully!\n");
        } else {
            LOGE("Save failed!\n");
        }
    }
    ImGui::EndChild();
}
void ImGui::SubFormSecondaryMenu(float width, float height) {
    ImGui::BeginChild("MenuPanel2", ImVec2(width, height), false, ImGuiWindowFlags_None);
    ImVec2 childPos = ImGui::GetWindowPos();
    ImVec2 childSize = ImGui::GetWindowSize();
    RenderRoundedRect(ImGui::GetWindowDrawList(), childPos, ImVec2(childPos.x + childSize.x, childPos.y + childSize.y), 10.0f);
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
    RenderRoundedRect(ImGui::GetWindowDrawList(), childPos, ImVec2(childPos.x + childSize.x, childPos.y + childSize.y), 10.0f);
    ImGui::EndChild();
    ImGui::EndChild();
    ImGui::Columns(1);
}

void ImGui::ShowMenuWindow(bool* p_open, ImVec2 display_size) {
    IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing Dear ImGui context. Refer to examples app!");
    ImGuiWindowFlags window_flags = 0;
    ImGui::SetNextWindowPos({0, display_size.y / 6}, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(display_size.x, display_size.y * 2 / 3), ImGuiCond_FirstUseEver);

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