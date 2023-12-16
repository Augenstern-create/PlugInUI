#pragma once
#include "Include.h"
#include "IncludeImGui.h"

namespace ImGui {
IMGUI_API ImTextureID CreateTextureFromImage(const char* imageFilePath, ID3D11Device* device);
IMGUI_API bool CustomizedButton(const char* label, ImVec2& size, ImU32 borderColor, ImU32 textColor);
IMGUI_API bool RoundedCheckbox(const char* label, bool* v);
IMGUI_API bool RenderCustomCheckbox(const char* label, bool* v);
IMGUI_API void RenderCustomSlider(const char* label, float* value, float min, float max, const char* format = "%.3f");
IMGUI_API void RenderCustomSliderint(const char* label, int* value, int min, int max, const char* format = "%.3f");
IMGUI_API bool RenderCustomButton(const char* str_id, ImTextureID user_texture_id, const ImVec2& size,
                                  const ImVec4& bg_col, const ImVec4& tint_col, const ImU32 col,
                                  const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1),
                                  ImGuiButtonFlags flags = 0);
IMGUI_API ImVec2 ImVec2ADD(const ImVec2 a, const ImVec2 b);
IMGUI_API ImVec2 ImVec2Decrease(const ImVec2 a, const ImVec2 b);
IMGUI_API ImVec2 ImVec2Multiply(const ImVec2 a, float b);
IMGUI_API ImVec2 ImVec2Except(const ImVec2 a, ImVec2 b);
IMGUI_API void DrawRotatedTriangle(ImDrawList* drawList, ImVec2 center, float size, float angleDegrees, ImU32 color);
IMGUI_API void RenderRoundedRect(ImDrawList* drawList, ImVec2 pMin, ImVec2 pMax, float rounding, ImU32 borderColor = 0,
                                 float borderSize = 0);
IMGUI_API void PlayerDisplay(const char* label, float size, float direction, ImVec2 location, ImU32 color);
}  // namespace ImGui