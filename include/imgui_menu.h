#pragma once
#include "imgui.h"
#include <vector>
#include <iostream>
#include <list>
#include <string>
#include <tuple>

#ifndef IMGUI_DISABLE

namespace ImGui {
class Mat {
   public:
    int width_;
    int height_;  // 修正拼写错误
    int channels_;
    unsigned char* data_;
    const char* file_name_;
    Mat();                       // 修正构造函数声明
    Mat(const char* file_name);  // 修正构造函数声明
    ~Mat();
};
IMGUI_API Mat loadImage(const char* filename);
IMGUI_API unsigned int LoadTexture(const char* path);
IMGUI_API void SetButPlayerImg(unsigned int id);
IMGUI_API void ShowMenuWindow(bool* p_open = NULL);
IMGUI_API void ShowExampleAppMenu();
IMGUI_API void ShowExampleAppPlayerInstall();
IMGUI_API void ShowExampleAppAlternateInstall();
IMGUI_API void ShowExampleAppMenuInitializelist();
IMGUI_API bool CustomizedButton(const char* label, ImVec2& size, ImU32 borderColor, ImU32 textColor);
IMGUI_API bool RoundedCheckbox(const char* label, bool* v);
IMGUI_API bool RenderCustomCheckbox(const char* label, bool* v);
IMGUI_API void RenderCustomSlider(const char* label, float* value, float min, float max, const char* format = "%.3f");
IMGUI_API void RenderCustomSliderint(const char* label, int* value, int min, int max, const char* format = "%.3f");
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
IMGUI_API ImVec2 ImVec2ADD(const ImVec2 a, const ImVec2 b);
IMGUI_API ImVec2 ImVec2Decrease(const ImVec2 a, const ImVec2 b);
IMGUI_API ImVec2 ImVec2Multiply(const ImVec2 a, float b);
IMGUI_API bool RenderCustomButton(const char* str_id, ImTextureID user_texture_id, const ImVec2& size,
                                  const ImVec4& bg_col, const ImVec4& tint_col, const ImU32 col,
                                  const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1),
                                  ImGuiButtonFlags flags = 0);
IMGUI_API void RenderRoundedRect(ImDrawList* drawList, ImVec2 pMin, ImVec2 pMax, float rounding, ImU32 borderColor = 0,
                                 float borderSize = 0);

}  // namespace ImGui

#endif  // #ifndef IMGUI_DISABLE
