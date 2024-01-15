#pragma once
#include "Include.h"
#include "IncludeImGui.h"
#include <unordered_map>
#include "game/Vector.h"

constexpr ImVec2 operator+(const ImVec2& rhs, const ImVec2& lhs) { return ImVec2(rhs.x + lhs.x, rhs.y + lhs.y); }
constexpr ImVec2 operator+(const ImVec2& rhs, float lhs) { return ImVec2(rhs.x + lhs, rhs.y + lhs); }
constexpr ImVec2 operator-(const ImVec2& rhs, const ImVec2& lhs) { return ImVec2(rhs.x - lhs.x, rhs.y - lhs.y); }
constexpr ImVec2 operator-(float rhs, const ImVec2& lhs) { return ImVec2(rhs - lhs.x, rhs - lhs.y); }
constexpr ImVec2 operator-(const ImVec2& rhs, float lhs) { return ImVec2(rhs.x - lhs, rhs.y - lhs); }
constexpr ImVec2 operator*(const ImVec2& lhs, float rhs) { return ImVec2(lhs.x * rhs, lhs.y * rhs); }

namespace ImGui {
IMGUI_API void RenderCustomSizedText(ImVec2 pos, const char* text, float FontSize, ImU32 Color = GetColorU32(ImGuiCol_Text), const char* text_end = NULL,
                                     bool hide_text_after_hash = true);
IMGUI_API ImTextureID CreateTextureFromImage(const char* imageFilePath, ID3D11Device* device);
IMGUI_API unsigned int CreateTextureFromImage(const char* imageFilePath);
IMGUI_API bool CustomizedButton(const char* label, ImVec2& size, ImU32 borderColor, ImU32 textColor);
IMGUI_API bool RenderCustomCheckbox(const char* label, bool* value, float fontSize);
IMGUI_API void RenderCustomSlider(const char* label, float* value, float min, float max, float fontSize, const char* format = "%.3f");
IMGUI_API void RenderCustomSliderint(const char* label, int* value, int min, int max, float fontSize, const char* format = "%.3f");
IMGUI_API bool RenderCustomButton(const char* str_id, ImTextureID user_texture_id, const ImVec2& size, const ImVec4& bg_col, const ImVec4& tint_col,
                                  const ImU32 col, const ImVec2& uv0 = ImVec2(0.0f, 0.0f), const ImVec2& uv1 = ImVec2(1.0f, 1.0f), ImGuiButtonFlags flags = 0);
IMGUI_API ImVec2 ImVec2ADD(const ImVec2 a, const ImVec2 b);
IMGUI_API ImVec2 ImVec2Decrease(const ImVec2 a, const ImVec2 b);
IMGUI_API ImVec2 ImVec2Multiply(const ImVec2 a, float b);
IMGUI_API ImVec2 ImVec2Except(const ImVec2 a, ImVec2 b);
IMGUI_API void DrawRotatedTriangle(ImDrawList* drawList, ImVec2 center, float size, float angleDegrees, ImU32 color);
IMGUI_API void RenderRoundedRect(ImDrawList* drawList, ImVec2 pMin, ImVec2 pMax, float rounding, ImU32 borderColor = 0, float borderSize = 0.0f);
IMGUI_API void PlayerDisplay(const char* label, float size, float direction, ImVec2 location, ImU32 color);

IMGUI_API void DrawPlayerHealthBar(const char* label, ImVec2 pos, float size, ImU32 color, float HP);

// 绘制玩家名称
IMGUI_API void DrawPlayerName(const char* txt, ImVec2 pos, float size, ImU32 color);
// 绘制距离
IMGUI_API void DrawPlayerDistance(const char* txt, ImVec2 pos, float size, ImU32 color);
// 绘制队伍id
IMGUI_API void DrawPlayerTeamID(const char* txt, ImVec2 pos, ImVec2 pos2, float size, ImU32 color);
// 绘制血量
IMGUI_API void DrawPlayerHitPoint(int HP, ImVec2 pos, float size, ImU32 color, ImU32 color2 = IM_COL32(255, 255, 255, 255));
// 绘制方框
IMGUI_API void DrawPlayerBox(ImVec2 startPos, ImVec2 stopPos, ImVec2 Screen, ImU32 color);
// 绘制骨骼
IMGUI_API void DrawPlayerBones(const char* label, float size, std::unordered_map<int, Vector3> location, ImVec2 Screen, ImU32 color);
// 绘制武器
IMGUI_API void DrawPlayerArms(ImTextureID textureID, ImVec2 pos, ImVec2 size);

}  // namespace ImGui