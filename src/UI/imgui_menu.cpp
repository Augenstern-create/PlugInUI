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
#include "game/config.h"

void MenuSubform();
void SubmenuSubform();
void OptionSubform();
void SliderSubform();
void ColorSubform();
void DraftOption(std::vector<ComponentsLists::Components<bool*>> Lists);
void DraftSlider(std::vector<ComponentsLists::ComponentsSlider<float*, float>> Lists, std::vector<ComponentsLists::ComponentsSlider<int*, int>> IntLists);
void DraftColor(std::vector<ComponentsLists::Components<float*>> Lists);
void DraftCheckBox();
enum SetButtion { RadarBut, SkeletonBut, SetBut };
static SetButtion set_but_ = SetButtion::RadarBut;
static ImVec2 g_display_size = {0, 0};
void ImGui::ShowMenuWindow(bool* p_open, ImVec2 display_size) {
    IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing Dear ImGui context. Refer to examples app!");
    ImGuiWindowFlags window_flags = 0;
    ImGui::SetNextWindowPos({display_size.x / 6, display_size.y / 6}, ImGuiCond_FirstUseEver);
    if (g_display_size != display_size) {
        ImGui::SetNextWindowSize(ImVec2(display_size.x * 2 / 3, display_size.y * 2 / 3), ImGuiCond_Always);
        g_display_size = display_size;
    }

    if (!ImGui::Begin("Windows Menu", p_open, window_flags)) {
        ImGui::End();
        return;
    }
    float rounding = 10.0f;
    float intervals = 5.0f;
    ImVec2 from_size = ImGui::GetContentRegionAvail();
    ImVec2 pos_menu = ImGui::GetCursorScreenPos();
    ImVec2 from_menu = ImVec2(from_size.x * 0.1f, from_size.y);

    ImVec2 pos_submenu = ImVec2(pos_menu.x + from_menu.x + intervals, pos_menu.y);
    ImVec2 from_submenu = ImVec2(from_size.x - from_menu.x - intervals, from_size.y * 0.1f);

    ImVec2 pos_option = ImVec2(pos_submenu.x, pos_submenu.y + from_submenu.y + intervals);
    ImVec2 from_option = ImVec2((from_size.x - from_menu.x) / 2.0f, (from_size.y - from_submenu.y) * 3.0f / 4.0f - intervals);

    ImVec2 pos_slider = ImVec2(pos_option.x + from_option.x + intervals, pos_option.y);
    ImVec2 from_slider = ImVec2((from_size.x - from_menu.x) / 4.0f - intervals, from_option.y);

    ImVec2 pos_color = ImVec2(pos_option.x, pos_option.y + from_option.y + intervals);
    ImVec2 from_color = ImVec2((from_size.x - from_menu.x) * 3.0f / 4.0f, from_size.y - intervals - from_option.y - intervals - from_submenu.y);

    ImVec2 pos_player = ImVec2(pos_slider.x + from_slider.x + intervals, pos_slider.y);
    ImVec2 from_player = ImVec2((from_size.x - from_menu.x) * 1.0f / 4.0f - intervals - intervals, from_size.y - from_submenu.y - intervals);

    ImGui::RoundedRectangleSubform("MenuSubform", from_menu, pos_menu, rounding, gameData.setting.subform_color);
    MenuSubform();
    ImGui::EndChild();
    ImGui::RoundedRectangleSubform("SubmenuSubform", from_submenu, pos_submenu, rounding, gameData.setting.subform_color);
    ImGui::EndChild();
    ImGui::RoundedRectangleSubform("OptionSubform", from_option, pos_option, rounding, gameData.setting.subform_color);
    OptionSubform();
    ImGui::EndChild();
    ImGui::RoundedRectangleSubform("SliderSubform", from_slider, pos_slider, rounding, gameData.setting.subform_color);
    SliderSubform();
    ImGui::EndChild();
    ImGui::RoundedRectangleSubform("ColorSubform", from_color, pos_color, rounding, gameData.setting.subform_color);
    ColorSubform();
    ImGui::EndChild();
    ImGui::RoundedRectangleSubform("PlayerSubform", from_player, pos_player, rounding, gameData.setting.subform_color);
    ImGui::EndChild();
    ImGui::End();
}

void MenuSubform() {
    ImVec2 size = ImGui::GetContentRegionAvail();
    ImVec2 buttonSize = {size.x * 0.9f, size.x * 0.25f};

    ImVec2 buttonPos((size.x - buttonSize.x) * 0.5f, size.y / 10.0f);
    ImGui::SetCursorPos(buttonPos);
    if (ImGui::Button("Radar", buttonSize)) {
        set_but_ = SetButtion::RadarBut;
    }
    buttonPos = {(size.x - buttonSize.x) * 0.5f, size.y * 2 / 10.0f};
    ImGui::SetCursorPos(buttonPos);
    if (ImGui::Button("Skeleton", buttonSize)) {
        set_but_ = SetButtion::SkeletonBut;
    }
    buttonPos = {(size.x - buttonSize.x) * 0.5f, size.y * 3 / 10.0f};
    ImGui::SetCursorPos(buttonPos);
    if (ImGui::Button("Seting", buttonSize)) {
        set_but_ = SetButtion::SetBut;
    }

    std::string butName = (int)gameData.setting.language == 0 ? "保存设置" : "Save Settings";
    ImGui::SetCursorPos({(size.x - buttonSize.x) * 0.5f, size.y * 9.0f / 10.0f});
    if (ImGui::Button(butName.c_str(), buttonSize)) {
        if (ComponentsLists::SaveSet()) {
            LOGI("Saved successfully!\n");
        } else {
            LOGE("Save failed!\n");
        }
    }
}

void OptionSubform() {
    switch (set_but_) {
        case SetButtion::RadarBut:
            DraftOption(ComponentsLists::radar_box_);
            break;
        case SetButtion::SkeletonBut:
            DraftOption(ComponentsLists::skeleton_box_);
            break;
        case SetButtion::SetBut:
            DraftCheckBox();
            DraftOption(ComponentsLists::setting_box_);
            break;

        default:
            break;
    }
}

void SliderSubform() {
    switch (set_but_) {
        case SetButtion::RadarBut:
            DraftSlider(ComponentsLists::radar_slider_, ComponentsLists::radar_slider_int_);
            break;
        case SetButtion::SkeletonBut:
            DraftSlider(ComponentsLists::skeleton_slider_, ComponentsLists::skeleton_slider_int_);
            break;
        case SetButtion::SetBut:
            DraftSlider(ComponentsLists::setting_slider_, ComponentsLists::setting_slider_int_);
            break;

        default:
            break;
    }
}
void ColorSubform() {
    switch (set_but_) {
        case SetButtion::RadarBut:
            DraftColor(ComponentsLists::radar_color_);
            break;
        case SetButtion::SkeletonBut:
            DraftColor(ComponentsLists::skeleton_color_);
            break;
        case SetButtion::SetBut:
            DraftColor(ComponentsLists::setting_color_);
            break;

        default:
            break;
    }
}

void DraftOption(std::vector<ComponentsLists::Components<bool*>> Lists) {
    ImVec2 size = ImGui::GetContentRegionAvail();
    ImGui::Columns(2, nullptr, false);
    for (auto parameter : Lists) {
        std::string name = gameData.setting.language == 0 ? parameter.ChineseName : parameter.EnglishName;
        ImGui::SetCursorPos({ImGui::GetCursorPos() + 10});
        ImGui::RenderCustomCheckbox(name.c_str(), parameter.value, gameData.setting.menu_font_size);
        ImGui::NextColumn();
    }
    ImGui::Columns(1);
}
void DraftCheckBox() {
    ImVec2 size = ImGui::GetContentRegionAvail();

    bool radio_button_magic_box_selected = (gameData.setting.key_type == KeyType::MagicBox);
    bool radio_button_not_have_selected = (gameData.setting.key_type == KeyType::NotHave);
    bool radio_button_Chinese_selected = (gameData.setting.language == Language::Chinese);
    bool radio_button_English_selected = (gameData.setting.language == Language::English);
    ImGui::Columns(2, nullptr, false);

    if (ImGui::RenderCustomCheckbox("键鼠魔盒", &radio_button_magic_box_selected, gameData.setting.menu_font_size)) {
        gameData.setting.key_type = KeyType::MagicBox;
    }
    if (ImGui::RenderCustomCheckbox("NotHave", &radio_button_not_have_selected, gameData.setting.menu_font_size)) {
        gameData.setting.key_type = KeyType::NotHave;
    }
    if (ImGui::RenderCustomCheckbox("中文", &radio_button_Chinese_selected, gameData.setting.menu_font_size)) {
        gameData.setting.language = Language::Chinese;
    }
    if (ImGui::RenderCustomCheckbox("English", &radio_button_English_selected, gameData.setting.menu_font_size)) {
        gameData.setting.language = Language::English;
    }

    ImGui::NextColumn();
    static char buffer[256] = "";
    std::strncpy(buffer, gameData.setting.COMID.c_str(), sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    if (ImGui::InputText("COMID", buffer, IM_ARRAYSIZE(buffer))) {
        gameData.setting.COMID = buffer;
    }
    ImGui::SameLine();
    ImGui::Columns(1);
}

void DraftSlider(std::vector<ComponentsLists::ComponentsSlider<float*, float>> Lists, std::vector<ComponentsLists::ComponentsSlider<int*, int>> IntLists) {
    for (auto parameter : Lists) {
        std::string name = gameData.setting.language == 0 ? parameter.ChineseName : parameter.EnglishName;
        ImGui::SetCursorPos({ImGui::GetCursorPos() + 10});
        ImGui::RenderCustomSlider(name.c_str(), parameter.value, parameter.min, parameter.max, gameData.setting.menu_font_size);
        ImGui::NextColumn();
    }
    for (auto parameter : IntLists) {
        std::string name = gameData.setting.language == 0 ? parameter.ChineseName : parameter.EnglishName;
        ImGui::SetCursorPos({ImGui::GetCursorPos() + 10});
        ImGui::RenderCustomSliderint(name.c_str(), parameter.value, parameter.min, parameter.max, gameData.setting.menu_font_size);
        ImGui::NextColumn();
    }
}
void DraftColor(std::vector<ComponentsLists::Components<float*>> Lists) {
    ImGui::Columns(3, nullptr, false);
    for (auto parameter : Lists) {
        std::string name = gameData.setting.language == 0 ? parameter.ChineseName : parameter.EnglishName;
        ImGui::SetCursorPos({ImGui::GetCursorPos() + 10});
        ImGui::ColorEdit4(name.c_str(), parameter.value, ImGuiColorEditFlags_NoInputs);
        ImGui::NextColumn();
    }
    ImGui::Columns(1);
}

#endif  // #ifndef IMGUI_DISABLE