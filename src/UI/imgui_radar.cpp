
#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "imgui_radar.h"
#include "imgui_module.h"

#ifndef IMGUI_DISABLE
#include "imgui_internal.h"
#include <cstring>
#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <tuple>
#include <cfloat>
#include "game/Data.h"
#include <d3d11.h>
#include "glad/glad.h"
#include <cmath>

static const float g_epsilon = 1e-6;  // 设置一个足够小的阈值

namespace ImGui {
extern unsigned int Map;
}  // namespace ImGui

// void ImGui::ShowRadarWindow(bool* p_open, ImVec2 display_size) {
//     IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing Dear ImGui context. Refer to examples app!");

//     ImGuiWindowFlags window_flags = 0;
//     window_flags |= ImGuiWindowFlags_None;                   // 0
//     window_flags |= ImGuiWindowFlags_NoTitleBar;             // 禁用标题栏
//     window_flags |= ImGuiWindowFlags_NoScrollbar;            // 禁用滚动条
//     window_flags |= ImGuiWindowFlags_NoMove;                 // 禁止用户拖动
//     window_flags |= ImGuiWindowFlags_NoResize;               // 禁止用户使用鼠标调整大小并禁用窗口边框
//     window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;  // 聚焦时禁止将窗口置为上层
//     ImGui::SetNextWindowSize({display_size.x - 20, display_size.x - 20}, ImGuiCond_FirstUseEver);
//     ImGui::SetNextWindowPos({10, 10}, ImGuiCond_FirstUseEver);
//     if (!ImGui::Begin("Windows Radar", p_open, window_flags)) {
//         ImGui::End();
//         if (ImGui::Map != 0) {
//             glDeleteTextures(1, &Map);
//             ImGui::Map = 0;
//         }
//         return;
//     }

//     ImVec2 from_size = ImGui::GetContentRegionAvail();
//     ImVec2 childPos = ImGui::GetWindowPos();
//     ImVec2 zoomedSize = ImVec2(from_size.x, from_size.x);
//     ImGuiIO& io = ImGui::GetIO();
//     ImVec2 uv0 = {0.0f + (gameData.Radar.ZoomFactor * 0.5f), 0.0f + (gameData.Radar.ZoomFactor * 0.5f)};
//     ImVec2 uv1 = {1.0f - (gameData.Radar.ZoomFactor * 0.5f), 1.0f - (gameData.Radar.ZoomFactor * 0.5f)};
//     if (io.MouseWheel != 0.0f && ImGui::IsWindowHovered()) {
//         ImVec2 MousePos = io.MousePos;
//         float zoomSpeed = 0.05f;
//         float zoomDelta = io.MouseWheel * zoomSpeed;
//         float newZoomFactor = gameData.Radar.ZoomFactor + zoomDelta;
//         newZoomFactor = newZoomFactor < 0.0f ? 0.0f : newZoomFactor;
//         newZoomFactor = newZoomFactor > 0.95f ? 0.95f : newZoomFactor;

//         // // 缩放前图片大小
//         // float imageWidth = 1.0f + (uv1.x - uv0.x) * 2.0f;
//         // float imageHeight = 1.0f + (uv1.y - uv0.y) * 2.0f;
//         // ImVec2 ImageSize = {zoomedSize.x * imageWidth, zoomedSize.y * imageHeight};
//         // ImVec2 MousePos = {(gameData.Radar.ScreenCenter.X + 0.5f) * zoomedSize.x, (gameData.Radar.ScreenCenter.Y + 0.5f) * zoomedSize.y};
//         // 缩放后图片大小
//         uv0 = {0.0f + (newZoomFactor * 0.5f), 0.0f + (newZoomFactor * 0.5f)};
//         uv1 = {1.0f - (newZoomFactor * 0.5f), 1.0f - (newZoomFactor * 0.5f)};
//         float imageWidth = 1.0f + (uv1.x - uv0.x) * 2.0f;
//         float imageHeight = 1.0f + (uv1.y - uv0.y) * 2.0f;
//         ImVec2 newImageSize = {zoomedSize.x * imageWidth, zoomedSize.y * imageHeight};

//         // 鼠标在屏幕中位置
//         ImVec2 fromMousePos = ImVec2((MousePos.x - childPos.x - 8.0f), (MousePos.y - childPos.y - 8.0f));
//         // 鼠标在图片中位置
//         ImVec2 imageMousePos = ImVec2((fromMousePos.x / from_size.x - (0.5f)), (fromMousePos.y / from_size.y - (0.5f)));
//         // 计算中心偏移
//         ImVec2 deltaOffset = ImVec2(imageMousePos.x * newZoomFactor, imageMousePos.y * newZoomFactor);
//         gameData.Radar.ScreenCenter = {deltaOffset.x, deltaOffset.y};
//         gameData.Radar.ZoomFactor = newZoomFactor;
//     }
//     if (io.MouseDown[ImGuiMouseButton_Left] && ImGui::IsWindowHovered()) {
//         float deltaX = io.MouseDelta.x * 0.2f;
//         float deltaY = io.MouseDelta.y * 0.2f;
//         if (deltaX != 0.0f || deltaY != 0.0f) {
//             float imageWidth = 1.0f + (uv1.x - uv0.x) * 2.0f;
//             float imageHeight = 1.0f + (uv1.y - uv0.y) * 2.0f;
//             ImVec2 newImageSize = {zoomedSize.x * imageWidth, zoomedSize.y * imageHeight};
//             gameData.Radar.ScreenCenter.X -= deltaX / newImageSize.x;
//             gameData.Radar.ScreenCenter.Y -= deltaY / newImageSize.y;
//         }
//     }
//     if (gameData.Radar.ScreenCenter.X != 0.0f || gameData.Radar.ScreenCenter.Y != 0.0f) {
//         uv0 = {uv0.x + gameData.Radar.ScreenCenter.X, uv0.y + gameData.Radar.ScreenCenter.Y};
//         uv1 = {uv1.x + gameData.Radar.ScreenCenter.X, uv1.y + gameData.Radar.ScreenCenter.Y};
//         if (uv0.x < 0.0f) {
//             uv1 = {uv1.x - uv0.x, uv1.y};
//             uv0 = {0.0f, uv0.y};
//         }

//         if (uv0.y < 0.0f) {
//             uv1 = {uv1.x, uv1.y - uv0.y};
//             uv0 = {uv0.x, 0.0f};
//         }

//         if (uv1.x > 1.0f) {
//             uv0 = {uv0.x + (1.0f - uv1.x), uv0.y};
//             uv1 = {1.0f, uv1.y};
//         }

//         if (uv1.y > 1.0f) {
//             uv0 = {uv0.x, uv0.y + (1.0f - uv1.y)};
//             uv1 = {uv1.x, 1.0f};
//         }
//         gameData.Radar.CenterPoint = {(uv1.x - uv0.x) * 0.5f + uv0.x, (uv1.y - uv0.y) * 0.5f + uv0.y};
//     }
//     // zoomedSize图片大小(1080*1080),uv0,起始位置,uv1,结束位置(0,0)(1,1),更改uv0与uv1完成缩放
//     ImGui::Image(reinterpret_cast<ImTextureID>(static_cast<intptr_t>(ImGui::Map)), zoomedSize, uv0, uv1);
//     // uv0-uv1是图像显示的部分(归一化之后)
//     float imageWidth = uv1.x - uv0.x;
//     float imageHeight = uv1.y - uv0.y;
//     auto Playerlist = Data::GetPlayers();
//     auto PlayerPos = gameData.Radar.Players;
//     for (auto player : Playerlist) {
//         auto it = PlayerPos.find(player.Entity);
//         if (it != PlayerPos.end()) {
//             // pos 图中某一点的坐标,需要跟随图片缩放
//             auto pos = it->second;
//             ImVec2 zoomedPoint = ImVec2((pos.x - uv0.x) * zoomedSize.x / imageWidth, (pos.y - uv0.y) * zoomedSize.y / imageHeight);
//             // 窗体起始坐标(childPos.x + 8,childPos.y + 28 )
//             ImVec2 screenPoint = ImVec2(childPos.x + 8 + zoomedPoint.x, childPos.y + 8 + zoomedPoint.y);
//             ImU32 PlayerColor = player.IsMyTeam ? gameData.Config.Color.PlayerTeamCol : gameData.Config.Color.EnemyTeamCol;
//             ImGui::PlayerDisplay(player.Name.c_str(), 8.0f, player.AimOffsets.y, ImVec2(screenPoint.x, screenPoint.y), PlayerColor);
//         } else {
//             continue;
//         }
//     }

//     ImGui::End();
// }

static int g_width = 0;
static int g_height = 0;
static float g_center_w = 0.0f;
static float g_center_h = 0.0f;
static unsigned int g_map;          // 假设这是你的图像纹理 ID
static float g_zoom_factor = 0.0f;  // 初始缩放因子
static ImVec2 g_screen_center = {0.0f, 0.0f};

void ImGui::ShowRadarWindow(bool* p_open, ImVec2 display_size) {
    IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing Dear ImGui context. Refer to examples app!");

    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_None;                   // 0
    window_flags |= ImGuiWindowFlags_NoTitleBar;             // 禁用标题栏
    window_flags |= ImGuiWindowFlags_NoScrollbar;            // 禁用滚动条
    window_flags |= ImGuiWindowFlags_NoMove;                 // 禁止用户拖动
    window_flags |= ImGuiWindowFlags_NoResize;               // 禁止用户使用鼠标调整大小并禁用窗口边框
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;  // 聚焦时禁止将窗口置为上层
    ImGui::SetNextWindowSize({display_size.x - 20, display_size.x - 20}, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos({10, 10}, ImGuiCond_FirstUseEver);
    if (!ImGui::Begin("Windows Radar", p_open, window_flags)) {
        ImGui::End();
        if (ImGui::Map != 0) {
            glDeleteTextures(1, &Map);
            ImGui::Map = 0;
        }
        return;
    }
    // 可用显示大小,超出部分不显示
    ImVec2 from_size = ImGui::GetContentRegionAvail();
    // 窗体开始位置
    ImVec2 childPos = ImGui::GetWindowPos();
    // 图片大小
    ImVec2 zoomedSize = ImVec2(from_size.x, from_size.x);
    ImGuiIO& io = ImGui::GetIO();
    // 显示起始位置(归一化)
    ImVec2 uv0 = {0.0f + (gameData.Radar.ZoomFactor * 0.5f), 0.0f + (gameData.Radar.ZoomFactor * 0.5f)};
    // 显示结束位置(归一化)
    ImVec2 uv1 = {1.0f - (gameData.Radar.ZoomFactor * 0.5f), 1.0f - (gameData.Radar.ZoomFactor * 0.5f)};
    // 鼠标滑轮滚动
    if (io.MouseWheel != 0.0f && ImGui::IsWindowHovered()) {
        // 计算缩放系数
        ImVec2 MousePos = io.MousePos;  // 鼠标位置
        float zoomSpeed = 0.05f;
        float zoomDelta = io.MouseWheel * zoomSpeed;
        float newZoomFactor = gameData.Radar.ZoomFactor + zoomDelta;
        newZoomFactor = newZoomFactor < 0.0f ? 0.0f : newZoomFactor;
        newZoomFactor = newZoomFactor > 0.95f ? 0.95f : newZoomFactor;
        // 更新显示位置
        uv0 = {0.0f + (newZoomFactor * 0.5f), 0.0f + (newZoomFactor * 0.5f)};
        uv1 = {1.0f - (newZoomFactor * 0.5f), 1.0f - (newZoomFactor * 0.5f)};
        float imageWidth = 1.0f + (uv1.x - uv0.x) * 2.0f;
        float imageHeight = 1.0f + (uv1.y - uv0.y) * 2.0f;
        // 缩放后图片大小
        ImVec2 newImageSize = {zoomedSize.x * imageWidth, zoomedSize.y * imageHeight};

        // 鼠标在屏幕中位置
        ImVec2 fromMousePos = ImVec2((MousePos.x - childPos.x - 8.0f), (MousePos.y - childPos.y - 8.0f));
        // 鼠标在图片中位置
        ImVec2 imageMousePos = ImVec2((fromMousePos.x / from_size.x - (0.5f)), (fromMousePos.y / from_size.y - (0.5f)));
        // 计算中心偏移
        ImVec2 deltaOffset = ImVec2(imageMousePos.x * newZoomFactor, imageMousePos.y * newZoomFactor);
        gameData.Radar.ScreenCenter = {deltaOffset.x, deltaOffset.y};
        gameData.Radar.ZoomFactor = newZoomFactor;
    }
    // 鼠标拖动平移
    if (io.MouseDown[ImGuiMouseButton_Left] && ImGui::IsWindowHovered()) {
        float deltaX = io.MouseDelta.x * 0.2f;
        float deltaY = io.MouseDelta.y * 0.2f;
        if (deltaX != 0.0f || deltaY != 0.0f) {
            float imageWidth = 1.0f + (uv1.x - uv0.x) * 2.0f;
            float imageHeight = 1.0f + (uv1.y - uv0.y) * 2.0f;
            ImVec2 newImageSize = {zoomedSize.x * imageWidth, zoomedSize.y * imageHeight};
            gameData.Radar.ScreenCenter.X -= deltaX / newImageSize.x;
            gameData.Radar.ScreenCenter.Y -= deltaY / newImageSize.y;
        }
    }
    // 计算边界,超过则补偿
    if (gameData.Radar.ScreenCenter.X != 0.0f || gameData.Radar.ScreenCenter.Y != 0.0f) {
        uv0 = {uv0.x + gameData.Radar.ScreenCenter.X, uv0.y + gameData.Radar.ScreenCenter.Y};
        uv1 = {uv1.x + gameData.Radar.ScreenCenter.X, uv1.y + gameData.Radar.ScreenCenter.Y};
        if (uv0.x < 0.0f) {
            uv1 = {uv1.x - uv0.x, uv1.y};
            uv0 = {0.0f, uv0.y};
        }

        if (uv0.y < 0.0f) {
            uv1 = {uv1.x, uv1.y - uv0.y};
            uv0 = {uv0.x, 0.0f};
        }

        if (uv1.x > 1.0f) {
            uv0 = {uv0.x + (1.0f - uv1.x), uv0.y};
            uv1 = {1.0f, uv1.y};
        }

        if (uv1.y > 1.0f) {
            uv0 = {uv0.x, uv0.y + (1.0f - uv1.y)};
            uv1 = {uv1.x, 1.0f};
        }
    }
    // zoomedSize图片大小(1080*1080),uv0,起始位置,uv1,结束位置(0,0)(1,1),更改uv0与uv1完成缩放
    ImGui::Image(reinterpret_cast<ImTextureID>(static_cast<intptr_t>(ImGui::Map)), zoomedSize, uv0, uv1);
    ImGui::End();
}

#endif  // IMGUI_DISABLE