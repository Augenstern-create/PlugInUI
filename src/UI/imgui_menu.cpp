#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "imgui_menu.h"
#ifndef IMGUI_DISABLE
#include "imgui_internal.h"
#include <cstring>
#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <tuple>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace ImGui {
std::vector<std::tuple<std::string, std::string, bool>> chbox_list_;
std::vector<std::tuple<std::string, std::string, float>> slider_list_;
std::vector<std::tuple<std::string, std::string, ImVec4>> color_picker_list_;
static ImVec4 default_color_ = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
static int multilingualism_ = 1;
static unsigned int but_player_img_id_;
enum ToolBoxes { player, alternate };
static ToolBoxes toolboxes_ = ToolBoxes::player;
static ImU32 subform_color_ = IM_COL32(41, 43, 56, 128);
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
ImGui::Mat::Mat() : width_(0), height_(0), channels_(0), data_(nullptr), file_name_(nullptr) {}  // 修正构造函数定义
ImGui::Mat::Mat(const char* filename) {
    file_name_ = filename;
    width_ = 0;
    height_ = 0;
    channels_ = 0;
    data_ = stbi_load(file_name_, &width_, &height_, &channels_, 0);
}  // 修正构造函数定义
ImGui::Mat::~Mat() {
    // 在析构函数中释放内存等清理工作
    if (data_ != nullptr) {
        delete[] data_;
        data_ = nullptr;
    }
}
// 读取图片
ImGui::Mat ImGui::loadImage(const char* filename) {
    ImGui::Mat* img = new ImGui::Mat();
    img->file_name_ = filename;
    img->data_ = (stbi_load(filename, &img->width_, &img->height_, &img->channels_, 0));
    return *img;
}

unsigned int ImGui::LoadTexture(const char* path) {
    int width, height, channels;
    unsigned char* data = stbi_load(path, &width, &height, &channels, 0);

    if (data == nullptr) {
        // 处理加载失败的情况
        fprintf(stderr, "Failed to load image: %s\npath is: %s\n", stbi_failure_reason(), path);
        return 0;
    }
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    if (channels == 4) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }

    else if (channels == 3) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    } else {
        fprintf(stderr, "width is: %d,    height is: %d,    channels is: %d\n", width, height, channels);
    }
    glGenerateMipmap(GL_TEXTURE_2D);
    // 释放图像数据
    stbi_image_free(data);

    return textureID;
}

void ImGui::SetButPlayerImg(unsigned int id) { but_player_img_id_ = id; }

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

/// @brief 自定义Checkbox控件
/// @param label
/// @param value
/// @return
bool ImGui::RenderCustomCheckbox(const char* label, bool* value) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);

    ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
    const ImVec2 total_size(40, 20);
    // 在这里根据Checkbox的状态计算位置
    ImVec2 pos = window->DC.CursorPos;
    pos.y += style.FramePadding.y;
    ImGui::InvisibleButton(label, total_size);

    // 在这里根据Checkbox的状态绘制图形
    ImU32 col = ImGui::GetColorU32(*value ? ImVec4(0.4f, 0.8f, 0.2f, 1.0f) : ImVec4(0.5f, 0.5f, 0.5f, 1.0f));

    const float corner_radius = 10.0f;
    const float circle_radius = total_size.y * 0.5f;

    window->DrawList->AddRectFilled(pos, ImVec2(pos.x + total_size.x, pos.y + total_size.y), col, corner_radius);
    if (!*value) {
        window->DrawList->AddCircleFilled(ImVec2(pos.x + circle_radius, pos.y + circle_radius), circle_radius,
                                          ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)));
    } else {
        window->DrawList->AddCircleFilled(ImVec2(pos.x + total_size.x - circle_radius, pos.y + circle_radius),
                                          circle_radius, ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)));
    }

    // 在这里绘制Label
    ImGui::RenderText(ImVec2(pos.x + total_size.x + 2, pos.y), label);
    bool pressed = ImGui::IsItemClicked();
    if (pressed) *value = !*value;
    return pressed;
}

/// @brief 自定义float滑块
/// @param label
/// @param value
/// @param min
/// @param max
/// @param format
void ImGui::RenderCustomSlider(const char* label, float* value, float min, float max, const char* format) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) return;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);

    ImVec2 pos = window->DC.CursorPos;
    pos.y += style.FramePadding.y;
    ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
    const ImVec2 total_size(100.0f, 4.0f);
    const float corner_radius = 10.0f;
    const float circle_radius = 8.0f;
    const ImVec2 rect_pos(pos.x, pos.y + label_size.y + circle_radius);
    float t = (*value - min) / (max - min);
    // 绘制name txt
    ImGui::RenderText(ImVec2(pos.x, pos.y), label);
    // 绘制滑块条
    window->DrawList->AddRectFilled(rect_pos, ImVec2(rect_pos.x + total_size.x, rect_pos.y + total_size.y),
                                    ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)), 10.0f);

    // 处理拖拽滑块的输入
    bool is_dragging = false;
    std::string string_name = std::string(label) + "slider";
    ImGui::InvisibleButton(string_name.c_str(), ImVec2(total_size.x, total_size.y + label_size.y + circle_radius +
                                                                         style.FramePadding.y * 2));
    if (ImGui::IsItemActive()) {
        is_dragging = true;
        t = (g.IO.MousePos.x - rect_pos.x) / total_size.x;
        t = ImClamp(t, 0.0f, 1.0f);
        *value = min + (max - min) * t;
    }
    ImVec2 handle_pos(pos.x + t * total_size.x, pos.y + label_size.y + style.FramePadding.y + circle_radius);

    // 绘制指示器
    window->DrawList->AddCircleFilled(handle_pos, circle_radius, ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)));

    std::string value_c = std::to_string(*value);
    value_c = value_c.substr(0, value_c.find(".") + 3);
    // 绘制值txt
    ImGui::RenderText(ImVec2(rect_pos.x + total_size.x + style.FramePadding.x, rect_pos.y - circle_radius),
                      value_c.c_str());
    // 如果拖拽滑块结束，更新输入焦点状态
    if (!ImGui::IsItemActive()) {
        ImGui::SetItemAllowOverlap();
    }
}

/// @brief 自定义int滑块
/// @param label
/// @param value
/// @param min
/// @param max
/// @param format
void ImGui::RenderCustomSliderint(const char* label, int* value, int min, int max, const char* format) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) return;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);

    ImVec2 pos = window->DC.CursorPos;
    pos.y += style.FramePadding.y;
    ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
    const ImVec2 total_size(100.0f, 4.0f);
    const float corner_radius = 10.0f;
    const float circle_radius = 8.0f;
    const ImVec2 rect_pos(pos.x, pos.y + label_size.y + circle_radius);
    float t = (float)((float)(*value - min) / (float)(max - min));
    // 绘制name txt
    ImGui::RenderText(ImVec2(pos.x, pos.y), label);

    // 绘制滑块条
    window->DrawList->AddRectFilled(rect_pos, ImVec2(rect_pos.x + total_size.x, rect_pos.y + total_size.y),
                                    ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)), 10.0f);

    // 处理拖拽滑块的输入
    bool is_dragging = false;
    std::string string_name = std::string(label) + "slider";
    ImGui::InvisibleButton(string_name.c_str(), ImVec2(total_size.x, total_size.y + label_size.y + circle_radius +
                                                                         style.FramePadding.y * 2));
    if (ImGui::IsItemActive()) {
        is_dragging = true;
        t = (g.IO.MousePos.x - rect_pos.x) / total_size.x;
        t = ImClamp(t, 0.0f, 1.0f);
        *value = static_cast<int>(min + (int)((float)(max - min) * t));
    }
    ImVec2 handle_pos(pos.x + t * total_size.x, pos.y + label_size.y + style.FramePadding.y + circle_radius);

    // 绘制指示器
    window->DrawList->AddCircleFilled(handle_pos, circle_radius, ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)));

    std::string value_c = std::to_string(*value);
    // 绘制值txt
    ImGui::RenderText(ImVec2(rect_pos.x + total_size.x + style.FramePadding.x, rect_pos.y - circle_radius),
                      value_c.c_str());
    // 如果拖拽滑块结束，更新输入焦点状态
    if (!ImGui::IsItemActive()) {
        ImGui::SetItemAllowOverlap();
    }
}

ImVec2 ImGui::ImVec2ADD(const ImVec2 a, const ImVec2 b) { return ImVec2(a.x + b.x, a.y + b.y); }
ImVec2 ImGui::ImVec2Decrease(const ImVec2 a, const ImVec2 b) { return ImVec2(a.x - b.x, a.y - b.y); }
ImVec2 ImGui::ImVec2Multiply(const ImVec2 a, float b) { return ImVec2(a.x * b, a.y * b); }
bool ImGui::RenderCustomButton(const char* str_id, ImTextureID user_texture_id, const ImVec2& size,
                               const ImVec4& bg_col, const ImVec4& tint_col, const ImU32 col, const ImVec2& uv0,
                               const ImVec2& uv1, ImGuiButtonFlags flags) {
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    if (window->SkipItems) return false;

    ImGuiID id = window->GetID(str_id);

    const ImVec2 padding = g.Style.FramePadding;
    const ImVec2 cursor_pos = window->DC.CursorPos;
    const ImRect bb(cursor_pos, ImVec2ADD(cursor_pos, ImVec2ADD(size, ImVec2Multiply(padding, 2.0f))));
    ItemSize(bb);
    if (!ItemAdd(bb, id)) return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held, flags);
    RenderNavHighlight(bb, id);
    RenderFrame(bb.Min, bb.Max, col, true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, g.Style.FrameRounding));
    if (bg_col.w > 0.0f)
        window->DrawList->AddRectFilled(ImVec2ADD(bb.Min, padding), ImVec2Decrease(bb.Max, padding),
                                        GetColorU32(bg_col));
    window->DrawList->AddImage(user_texture_id, ImVec2ADD(bb.Min, padding), ImVec2Decrease(bb.Max, padding), uv0, uv1,
                               GetColorU32(tint_col));

    return pressed;
}

void ImGui::RenderRoundedRect(ImDrawList* drawList, ImVec2 pMin, ImVec2 pMax, float rounding, ImU32 borderColor,
                              float borderSize) {
    drawList->AddRectFilled(pMin, pMax, subform_color_, rounding);

    if (borderSize > 0) drawList->AddRect(pMin, pMax, borderColor, ImDrawFlags_RoundCornersAll, borderSize);
}

void ImGui::SubFormComfig1(const char* name, float width, float height,
                           std::vector<std::tuple<std::string, std::string, bool>>* chbox_list) {
    ImGui::BeginChild(name, ImVec2(width, height), ImGuiChildFlags_None, ImGuiWindowFlags_None);
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
    ImGui::BeginChild(name, ImVec2(width, height), ImGuiChildFlags_None, ImGuiWindowFlags_None);
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
    ImGui::BeginChild(name, ImVec2(width, height), ImGuiChildFlags_None, ImGuiWindowFlags_None);
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
    ImGui::BeginChild(name, ImVec2(width, height), ImGuiChildFlags_None, ImGuiWindowFlags_None);
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
    ImGui::BeginChild("MenuPanel", ImVec2(width, height), ImGuiChildFlags_None, ImGuiWindowFlags_None);
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
    ImGui::BeginChild("MenuPanel2", ImVec2(width, height), ImGuiChildFlags_None, ImGuiWindowFlags_None);
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
    ImGui::BeginChild("MenuPanel3", ImVec2(width, height), ImGuiChildFlags_None, ImGuiWindowFlags_None);
    ImGui::Columns(2, nullptr, false);
    ImGui::SetColumnWidth(0, MenuPanel4_size.x);
    ImGui::SetColumnWidth(1, MenuPanel4_size.x / 2);
    ImGui::BeginChild("MenuPanel4", ImVec2(MenuPanel4_size.x, MenuPanel4_size.y), ImGuiChildFlags_None,
                      ImGuiWindowFlags_None);
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

    ImGui::BeginChild("MenuPanel5", ImVec2(character_size.x, character_size.y), ImGuiChildFlags_None,
                      ImGuiWindowFlags_None);
    ImVec2 childPos = ImGui::GetWindowPos();
    ImVec2 childSize = ImGui::GetWindowSize();
    RenderRoundedRect(ImGui::GetWindowDrawList(), childPos, ImVec2(childPos.x + childSize.x, childPos.y + childSize.y),
                      10.0f);
    ImGui::EndChild();
    ImGui::EndChild();
    ImGui::Columns(1);
}

void ImGui::ShowMenuWindow(bool* p_open) {
    IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing Dear ImGui context. Refer to examples app!");
    ImGuiWindowFlags window_flags = 0;
    // We specify a default position/size in case there's no data in the .ini file.
    // We only do it to make the demo applications a little more welcoming, but typically this isn't required.
    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 650, main_viewport->WorkPos.y + 20),
                            ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);
    // ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f);
    // Main body of the Demo window starts here.
    if (!ImGui::Begin("Windows Menu", p_open, window_flags)) {
        // Early out if the window is collapsed, as an optimization.
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
}
#endif  // #ifndef IMGUI_DISABLE