#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "imgui_module.h"
#ifndef IMGUI_DISABLE
#include "imgui_internal.h"
#include <cstring>
#include <tuple>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <d3d11.h>
#include <cmath>
#include "imgui_radar.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"
#include "game/Bone.h"
#include "game/Data.h"
#include "imgui.h"

namespace ImGui {
static ImU32 subform_color_ = IM_COL32(41, 43, 56, 128);

ImU32 InverseColor(ImU32 color) {
    return IM_COL32(255 - ((color >> IM_COL32_R_SHIFT) & 0xFF), 255 - ((color >> IM_COL32_G_SHIFT) & 0xFF), 255 - ((color >> IM_COL32_B_SHIFT) & 0xFF), 255);
}

TextureID LoadingTexturesUsingImage(const char* path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrChannels;
    uint8_t* data = (uint8_t*)stbi_load(path, &width, &height, &nrChannels, 0);
    if (data == NULL) {
        return {nullptr, {0.0f, 0.0f}};
    }

    GLenum format;
    if (nrChannels == 1)
        format = GL_RED;
    else if (nrChannels == 3)
        format = GL_RGB;
    else if (nrChannels == 4)
        format = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    TextureID t;
    t.texture = reinterpret_cast<ImTextureID>(static_cast<intptr_t>(textureID));
    t.size = {(float)width, (float)height};
    t.channels = nrChannels;
    stbi_image_free(data);

    return t;
}
}  // namespace ImGui

// 带有圆角矩形的子窗体
void ImGui::RoundedRectangleSubform(const char* text, ImVec2 size, ImVec2 pos, float rounding, ImU32 Color) {
    ImGui::SetNextWindowPos(pos);
    ImGui::BeginChild(text, size, ImGuiWindowFlags_None);
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 p0 = pos;
    ImVec2 p1 = ImVec2(pos.x + size.x, pos.y + size.y);
    drawList->AddRectFilled(p0, p1, Color, rounding);
}

void ImGui::RenderCustomSizedText(ImVec2 pos, const char* text, float FontSize, ImU32 Color, const char* text_end, bool hide_text_after_hash) {
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;

    const char* text_display_end;
    if (hide_text_after_hash) {
        text_display_end = FindRenderedTextEnd(text, text_end);
    } else {
        if (!text_end) text_end = text + strlen(text);
        text_display_end = text_end;
    }
    if (text != text_display_end) {
        window->DrawList->AddText(g.Font, FontSize, pos, Color, text, text_display_end);
        if (g.LogEnabled) LogRenderedText(&pos, text, text_display_end);
    }
}

ImTextureID ImGui::CreateTextureFromImage(const char* imageFilePath, ID3D11Device* device) {
    int width, height, channels;
    stbi_uc* pixels = stbi_load(imageFilePath, &width, &height, &channels, STBI_rgb_alpha);

    if (pixels) {
        // 创建DirectX 11纹理
        D3D11_TEXTURE2D_DESC desc;
        ZeroMemory(&desc, sizeof(desc));
        desc.Width = width;
        desc.Height = height;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // STBI_rgb_alpha 使用4通道（R, G, B, A）
        desc.SampleDesc.Count = 1;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

        D3D11_SUBRESOURCE_DATA initData;
        ZeroMemory(&initData, sizeof(initData));
        initData.pSysMem = pixels;
        initData.SysMemPitch = static_cast<UINT>(width * 4);

        ID3D11Texture2D* texture = nullptr;
        HRESULT hr = device->CreateTexture2D(&desc, &initData, &texture);

        stbi_image_free(pixels);

        if (SUCCEEDED(hr)) {
            // 创建Shader Resource View
            ID3D11ShaderResourceView* texture_srv = nullptr;
            device->CreateShaderResourceView(texture, nullptr, &texture_srv);
            texture->Release();

            if (texture_srv != nullptr) {
                std::cout << "Texture loaded successfully" << std::endl;
                return reinterpret_cast<ImTextureID>(texture_srv);

            } else {
                printf("Failed to create Shader Resource View.\n");
            }
        } else {
            printf("Failed to create Direct3D 11 texture.\n");
        }
    } else {
        printf("Failed to load image from file.\n");
    }

    return 0;
}

// 用于加载图像的函数
unsigned int ImGui::CreateTextureFromImage(const char* path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    } else {
        // 处理加载失败的情况
        std::cout << "Failed to load texture" << std::endl;
    }

    return textureID;
}

/// @brief 自定义Checkbox控件
/// @param label
/// @param value
/// @return
bool ImGui::RenderCustomCheckbox(const char* label, bool* value, float fontSize) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);

    float defaultFontSize = ImGui::GetIO().FontDefault ? ImGui::GetIO().FontDefault->FontSize : 16.0f;
    float font_zoom_size = fontSize / defaultFontSize;
    ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
    label_size = label_size * font_zoom_size;
    const ImVec2 total_size(40.0f, 20.0f);
    // 在这里根据Checkbox的状态计算位置
    ImVec2 pos = window->DC.CursorPos;
    pos.y += style.FramePadding.y;
    // ImGui::InvisibleButton(label, ImVec2(label_size.x + total_size.x, total_size.y));
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
        window->DrawList->AddCircleFilled(ImVec2(pos.x + total_size.x - circle_radius, pos.y + circle_radius), circle_radius,
                                          ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)));
    }
    ImGui::RenderCustomSizedText(ImVec2(pos.x + total_size.x + 2, pos.y), label, fontSize);

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
void ImGui::RenderCustomSlider(const char* label, float* value, float min, float max, float fontSize, const char* format) {
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
    ImGui::RenderCustomSizedText(ImVec2(pos.x, pos.y), label, fontSize);
    // 绘制滑块条
    window->DrawList->AddRectFilled(rect_pos, ImVec2(rect_pos.x + total_size.x, rect_pos.y + total_size.y), ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)),
                                    10.0f);
    bool is_dragging = false;
    std::string string_name = std::string(label) + "slider";
    ImGui::InvisibleButton(string_name.c_str(), ImVec2(total_size.x, total_size.y + label_size.y + circle_radius + style.FramePadding.y * 2));
    if (ImGui::IsItemActive()) {
        is_dragging = true;
        t = (g.IO.MousePos.x - rect_pos.x) / total_size.x;
        t = ImClamp(t, 0.0f, 1.0f);
        *value = min + (max - min) * t;
    }
    ImVec2 handle_pos(pos.x + t * total_size.x, pos.y + label_size.y + style.FramePadding.y + circle_radius);
    window->DrawList->AddCircleFilled(handle_pos, circle_radius, ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)));
    std::string value_c = std::to_string(*value);
    value_c = value_c.substr(0, value_c.find(".") + 3);
    ImGui::RenderCustomSizedText(ImVec2(rect_pos.x + total_size.x + style.FramePadding.x, rect_pos.y - circle_radius), value_c.c_str(), fontSize);
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
void ImGui::RenderCustomSliderint(const char* label, int* value, int min, int max, float fontSize, const char* format) {
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
    ImGui::RenderCustomSizedText(ImVec2(pos.x, pos.y), label, fontSize);

    // 绘制滑块条
    window->DrawList->AddRectFilled(rect_pos, ImVec2(rect_pos.x + total_size.x, rect_pos.y + total_size.y), ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)),
                                    10.0f);

    // 处理拖拽滑块的输入
    bool is_dragging = false;
    std::string string_name = std::string(label) + "slider";
    ImGui::InvisibleButton(string_name.c_str(), ImVec2(total_size.x, total_size.y + label_size.y + circle_radius + style.FramePadding.y * 2));
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
    ImGui::RenderCustomSizedText(ImVec2(rect_pos.x + total_size.x + style.FramePadding.x, rect_pos.y - circle_radius), value_c.c_str(), fontSize);
    // 如果拖拽滑块结束，更新输入焦点状态
    if (!ImGui::IsItemActive()) {
        ImGui::SetItemAllowOverlap();
    }
}

ImVec2 ImGui::ImVec2ADD(const ImVec2 a, const ImVec2 b) { return ImVec2(a.x + b.x, a.y + b.y); }
ImVec2 ImGui::ImVec2Decrease(const ImVec2 a, const ImVec2 b) { return ImVec2(a.x - b.x, a.y - b.y); }
ImVec2 ImGui::ImVec2Multiply(const ImVec2 a, float b) { return ImVec2(a.x * b, a.y * b); }
ImVec2 ImGui::ImVec2Except(const ImVec2 a, ImVec2 b) { return ImVec2(a.x / b.x, a.y / b.y); }
bool ImGui::RenderCustomButton(const char* str_id, ImTextureID user_texture_id, const ImVec2& size, const ImVec4& bg_col, const ImVec4& tint_col,
                               const ImU32 col, const ImVec2& uv0, const ImVec2& uv1, ImGuiButtonFlags flags) {
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
    if (bg_col.w > 0.0f) window->DrawList->AddRectFilled(ImVec2ADD(bb.Min, padding), ImVec2Decrease(bb.Max, padding), GetColorU32(bg_col));
    window->DrawList->AddImage(user_texture_id, ImVec2ADD(bb.Min, padding), ImVec2Decrease(bb.Max, padding), uv0, uv1, GetColorU32(tint_col));

    return pressed;
}

void ImGui::RenderRoundedRect(ImDrawList* drawList, ImVec2 pMin, ImVec2 pMax, float rounding, ImU32 borderColor, float borderSize) {
    drawList->AddRectFilled(pMin, pMax, subform_color_, rounding);
    if (borderSize > 0) drawList->AddRect(pMin, pMax, borderColor, ImDrawFlags_RoundCornersAll, borderSize);
}

float DegToRad(float degrees) { return degrees * 3.1415926535f / 180.0f; }

void ImGui::DrawRotatedTriangle(ImDrawList* drawList, ImVec2 center, float side, float rotationAngle, ImU32 color) {
    // 计算三角形的高度
    float height = side * 0.6f * sqrt(3.0f);

    // 计算底部的中心点坐标
    ImVec2 baseCenter = ImVec2(center.x, center.y + height / 2);

    // 计算三角形的顶点坐标
    ImVec2 p0(center.x, center.y - height / 2);
    ImVec2 p1(center.x - side / 2, center.y + height / 2);
    ImVec2 p2(center.x + side / 2, center.y + height / 2);

    // 将角度转换为弧度值
    rotationAngle += 90.0f;  // 设置右侧为顶点
    float rotationAngleRad = rotationAngle * 3.14159265358979323846f / 180.0f;

    // 计算旋转后的三角形顶点坐标
    ImVec2 rotatedP0(center.x + (p0.x - baseCenter.x) * cos(rotationAngleRad) - (p0.y - baseCenter.y) * sin(rotationAngleRad),
                     center.y + (p0.x - baseCenter.x) * sin(rotationAngleRad) + (p0.y - baseCenter.y) * cos(rotationAngleRad));
    ImVec2 rotatedP1(center.x + (p1.x - baseCenter.x) * cos(rotationAngleRad) - (p1.y - baseCenter.y) * sin(rotationAngleRad),
                     center.y + (p1.x - baseCenter.x) * sin(rotationAngleRad) + (p1.y - baseCenter.y) * cos(rotationAngleRad));
    ImVec2 rotatedP2(center.x + (p2.x - baseCenter.x) * cos(rotationAngleRad) - (p2.y - baseCenter.y) * sin(rotationAngleRad),
                     center.y + (p2.x - baseCenter.x) * sin(rotationAngleRad) + (p2.y - baseCenter.y) * cos(rotationAngleRad));

    // 在给定的ImDrawList中绘制旋转后的三角形
    drawList->AddTriangleFilled(rotatedP0, rotatedP1, rotatedP2, color);
}

void ImGui::PlayerDisplay(const char* label, float size, float direction, ImVec2 location, ImU32 color) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) return;
    ImGui::DrawRotatedTriangle(window->DrawList, location, size * 2.0f, direction, color);
    window->DrawList->AddCircleFilled(location, size, color);
    ImGui::RenderText(ImVec2(location.x, location.y), label);
}

// 绘制玩家名称
void ImGui::DrawPlayerName(const char* txt, ImVec2 pos, float size, ImU32 color) { ImGui::RenderCustomSizedText(pos, txt, size, color); }

// 绘制距离
void ImGui::DrawPlayerDistance(const char* txt, ImVec2 pos, float size, ImU32 color) { ImGui::RenderCustomSizedText(pos, txt, size, color); }

// // 绘制队伍id
// void ImGui::DrawPlayerTeamID(const char* txt, ImVec2 pos, ImVec2 pos2, float size, ImU32 color) {
//     ImGuiWindow* window = ImGui::GetCurrentWindow();
//     if (window->SkipItems) return;
//     window->DrawList->AddCircleFilled(pos2, size * 0.75, color);
//     ImGui::RenderCustomSizedText(pos, txt, size, IM_COL32(255, 255, 255, 255));
// }
// 绘制队伍id
void ImGui::DrawPlayerTeamID(const char* txt, ImVec2 pos, float size, float zoom, ImU32 color) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) return;
    ImVec2 txtSize = ImGui::CalcTextSize(txt);
    txtSize = txtSize * zoom;
    ImU32 contexts_color = InverseColor(color);
    ImVec2 pos2 = pos + txtSize * 0.25f;
    window->DrawList->AddCircleFilled(pos2, size * 0.5f, color);
    ImGui::RenderCustomSizedText(pos, txt, size, contexts_color);
}

// 绘制血量
void ImGui::DrawPlayerHitPoint(int HP, ImVec2 pos, float size, ImU32 color, ImU32 color2) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) return;
    HP = HP > 100.0f ? 100.0f : HP;
    HP = HP < 0.0f ? 0.0f : HP;
    float posx = 50.0f * size;
    float posy = (8.0f - 8.0f * size) + 8.0f;
    ImVec2 HPPos1 = {pos.x - posx, pos.y - 20.0f};
    ImVec2 HPPos2 = {pos.x + posx, pos.y - posy};
    ImVec2 HPPos3 = {HPPos1.x + (HP * size), pos.y - posy};
    window->DrawList->AddRectFilled(HPPos1, HPPos2, color2);
    window->DrawList->AddRectFilled(HPPos1, HPPos3, color);
}

// 绘制武器
void ImGui::DrawPlayerArms(ImTextureID textureID, ImVec2 pos, ImVec2 size) {
    ImGui::SetCursorPos(pos);
    ImGui::Image(textureID, size);
}

// 方框
void ImGui::DrawPlayerBox(ImVec2 startPos, ImVec2 stopPos, ImVec2 Screen, ImU32 color) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) return;
    if (startPos.x < 0 || startPos.y < 0 || stopPos.x > Screen.x || stopPos.y > Screen.y) return;
    window->DrawList->AddRectFilled(ImVec2(startPos.x, startPos.y), ImVec2(stopPos.x, startPos.y), color, 2.0f);
    window->DrawList->AddRectFilled(ImVec2(startPos.x, startPos.y), ImVec2(startPos.x, stopPos.y), color, 2.0f);
    window->DrawList->AddRectFilled(ImVec2(startPos.x, stopPos.y), ImVec2(stopPos.x, stopPos.y), color, 2.0f);
    window->DrawList->AddRectFilled(ImVec2(stopPos.x, startPos.y), ImVec2(stopPos.x, stopPos.y), color, 2.0f);
}

void ImGui::DrawPlayerBones(const char* label, float size, std::unordered_map<int, Vector3> location, ImVec2 Screen, ImU32 color) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) return;

    for (const auto& pair : location) {
        const Vector3& vector = pair.second;
        if (vector.x < 0 || vector.x > Screen.x || vector.y < 0 || vector.y > Screen.y) {
            return;
        }
    }

    // 头
    window->DrawList->AddLine(ImVec2(location[EBoneIndex::forehead].x, location[EBoneIndex::forehead].y),
                              ImVec2(location[EBoneIndex::head].x, location[EBoneIndex::head].y), color, size);
    window->DrawList->AddLine(ImVec2(location[EBoneIndex::head].x, location[EBoneIndex::head].y),
                              ImVec2(location[EBoneIndex::neck_01].x, location[EBoneIndex::neck_01].y), color, size);
    // 右臂
    window->DrawList->AddLine(ImVec2(location[EBoneIndex::neck_01].x, location[EBoneIndex::neck_01].y),
                              ImVec2(location[EBoneIndex::upperarm_r].x, location[EBoneIndex::upperarm_r].y), color, size);
    window->DrawList->AddLine(ImVec2(location[EBoneIndex::upperarm_r].x, location[EBoneIndex::upperarm_r].y),
                              ImVec2(location[EBoneIndex::lowerarm_r].x, location[EBoneIndex::lowerarm_r].y), color, size);
    window->DrawList->AddLine(ImVec2(location[EBoneIndex::lowerarm_r].x, location[EBoneIndex::lowerarm_r].y),
                              ImVec2(location[EBoneIndex::hand_r].x, location[EBoneIndex::hand_r].y), color, size);
    // 左臂
    window->DrawList->AddLine(ImVec2(location[EBoneIndex::neck_01].x, location[EBoneIndex::neck_01].y),
                              ImVec2(location[EBoneIndex::upperarm_l].x, location[EBoneIndex::upperarm_l].y), color, size);
    window->DrawList->AddLine(ImVec2(location[EBoneIndex::upperarm_l].x, location[EBoneIndex::upperarm_l].y),
                              ImVec2(location[EBoneIndex::lowerarm_l].x, location[EBoneIndex::lowerarm_l].y), color, size);
    window->DrawList->AddLine(ImVec2(location[EBoneIndex::lowerarm_l].x, location[EBoneIndex::lowerarm_l].y),
                              ImVec2(location[EBoneIndex::hand_l].x, location[EBoneIndex::hand_l].y), color, size);
    // 脊椎
    window->DrawList->AddLine(ImVec2(location[EBoneIndex::neck_01].x, location[EBoneIndex::neck_01].y),
                              ImVec2(location[EBoneIndex::spine_01].x, location[EBoneIndex::spine_01].y), color, size);
    window->DrawList->AddLine(ImVec2(location[EBoneIndex::spine_01].x, location[EBoneIndex::spine_01].y),
                              ImVec2(location[EBoneIndex::pelvis].x, location[EBoneIndex::pelvis].y), color, size);
    // 右腿
    window->DrawList->AddLine(ImVec2(location[EBoneIndex::pelvis].x, location[EBoneIndex::pelvis].y),
                              ImVec2(location[EBoneIndex::thigh_r].x, location[EBoneIndex::thigh_r].y), color, size);
    window->DrawList->AddLine(ImVec2(location[EBoneIndex::thigh_r].x, location[EBoneIndex::thigh_r].y),
                              ImVec2(location[EBoneIndex::calf_r].x, location[EBoneIndex::calf_r].y), color, size);
    window->DrawList->AddLine(ImVec2(location[EBoneIndex::calf_r].x, location[EBoneIndex::calf_r].y),
                              ImVec2(location[EBoneIndex::foot_r].x, location[EBoneIndex::foot_r].y), color, size);
    // 左腿
    window->DrawList->AddLine(ImVec2(location[EBoneIndex::pelvis].x, location[EBoneIndex::pelvis].y),
                              ImVec2(location[EBoneIndex::thigh_l].x, location[EBoneIndex::thigh_l].y), color, size);
    window->DrawList->AddLine(ImVec2(location[EBoneIndex::thigh_l].x, location[EBoneIndex::thigh_l].y),
                              ImVec2(location[EBoneIndex::calf_l].x, location[EBoneIndex::calf_l].y), color, size);
    window->DrawList->AddLine(ImVec2(location[EBoneIndex::calf_l].x, location[EBoneIndex::calf_l].y),
                              ImVec2(location[EBoneIndex::foot_l].x, location[EBoneIndex::foot_l].y), color, size);
}
#endif  // #ifndef IMGUI_DISABLE
