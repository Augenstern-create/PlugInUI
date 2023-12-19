#pragma once
#include "Include.h"
#include "IncludeImGui.h"

constexpr ImVec2 operator+(const ImVec2& rhs, const ImVec2& lhs) { return ImVec2(rhs.x + lhs.x, rhs.y + lhs.y); }
constexpr ImVec2 operator-(const ImVec2& rhs, const ImVec2& lhs) { return ImVec2(rhs.x - lhs.x, rhs.y - lhs.y); }
constexpr ImVec2 operator-(float rhs, const ImVec2& lhs) { return ImVec2(rhs - lhs.x, rhs - lhs.y); }
constexpr ImVec2 operator*(const ImVec2& lhs, float rhs) { return ImVec2(lhs.x * rhs, lhs.y * rhs); }

namespace ImGui {
IMGUI_API void ShowMenuWindow(bool* p_open = NULL, ImVec2 display_size = ImVec2(100.0f, 100.0f));
IMGUI_API void ShowExampleAppMenu();
IMGUI_API void ShowExampleAppPlayerInstall();
IMGUI_API void ShowExampleAppAlternateInstall();
IMGUI_API void ShowExampleAppMenuInitializelist();

IMGUI_API void SubFormMenuBar(float width, float height);
IMGUI_API void SubFormSecondaryMenu(float width, float height);
IMGUI_API void SubFormFunctionBlock(float width, float height);
IMGUI_API void SubFormComfig1(const char* name, float width, float height,
                              std::vector<std::tuple<std::string, std::string, bool>>* chbox_list);
IMGUI_API void SubFormComfig2(const char* name, float width, float height,
                              std::vector<std::tuple<std::string, std::string, float>>* slider_list);
IMGUI_API void SubFormComfig3(const char* name, float width, float height,
                              std::vector<std::tuple<std::string, std::string, int>>* slider_list);
IMGUI_API void SubFormComfig4(const char* name, float width, float height,
                              std::vector<std::tuple<std::string, std::string, ImVec4>>* clore_list);

}  // namespace ImGui
