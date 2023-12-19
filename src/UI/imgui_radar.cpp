
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

namespace ImGui {
extern ImTextureID Map;
}  // namespace ImGui

void ImGui::ShowRadarWindow(bool* p_open, ImVec2 display_size) {
    IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing Dear ImGui context. Refer to examples app!");
    ImGuiWindowFlags window_flags = 0;
    ImGui::SetNextWindowPos({display_size.x / 4, 20}, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(display_size.x * 1 / 2, display_size.x * 1 / 2 + 30), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin("Windows Radar", nullptr, ImGuiWindowFlags_None | ImGuiWindowFlags_NoScrollbar)) {
        ImGui::End();
        if (ImGui::Map) {
            ID3D11ShaderResourceView* d3dTexture = static_cast<ID3D11ShaderResourceView*>(ImGui::Map);
            d3dTexture->Release();
        }
        return;
    }

    ImVec2 from_size = ImGui::GetContentRegionAvail();
    ImVec2 childPos = ImGui::GetWindowPos();
    ImVec2 imageSize = ImVec2(gameData.Radar.ImageMapSize, gameData.Radar.ImageMapSize);
    ImVec2 zoomedSize = ImVec2(from_size.x, from_size.y - 30.0f);
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 uv0 = {0.0f + (gameData.Radar.ZoomFactor * 0.5f), 0.0f + (gameData.Radar.ZoomFactor * 0.5f)};
    ImVec2 uv1 = {1.0f - (gameData.Radar.ZoomFactor * 0.5f), 1.0f - (gameData.Radar.ZoomFactor * 0.5f)};
    if (io.MouseWheel != 0.0f) {
        ImVec2 MousePos = io.MousePos;
        float zoomSpeed = 0.05f;
        float zoomDelta = io.MouseWheel * zoomSpeed;
        float newZoomFactor = gameData.Radar.ZoomFactor + zoomDelta;
        newZoomFactor = newZoomFactor < 0.0f ? 0.0f : newZoomFactor;
        newZoomFactor = newZoomFactor > 0.95f ? 0.95f : newZoomFactor;
        gameData.Radar.ZoomFactor = newZoomFactor;

        uv0 = {0.0f + (newZoomFactor * 0.5f), 0.0f + (newZoomFactor * 0.5f)};
        uv1 = {1.0f - (newZoomFactor * 0.5f), 1.0f - (newZoomFactor * 0.5f)};
        float imageWidth = uv1.x - uv0.x;
        float imageHeight = uv1.y - uv0.y;
        // 鼠标在屏幕中位置
        ImVec2 fromMousePos = ImVec2((MousePos.x - childPos.x - 8), (MousePos.y - childPos.y - 28));
        // 鼠标在图片中位置
        ImVec2 imageMousePos = ImVec2(fromMousePos.x / zoomedSize.x, fromMousePos.y / zoomedSize.y);
        ImVec2 deltaOffset = ImVec2(((fromMousePos.x - (zoomedSize.x * 0.5f)) / zoomedSize.x * newZoomFactor),
                                    ((fromMousePos.y - (zoomedSize.y * 0.5f)) / zoomedSize.y * newZoomFactor));
        // 中心点偏移
        gameData.Radar.ScreenCenter = {deltaOffset.x, deltaOffset.y};
    }
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
    ImGui::Image(ImGui::Map, zoomedSize, uv0, uv1);
    float imageWidth = uv1.x - uv0.x;
    float imageHeight = uv1.y - uv0.y;
    auto Playerlist = Data::GetPlayers();
    auto PlayerPos = gameData.Radar.Players;
    for (auto player : Playerlist) {
        auto it = PlayerPos.find(player.Entity);
        if (it != PlayerPos.end()) {
            // pos 图中某一点的坐标,需要跟随图片缩放
            auto pos = it->second;
            ImVec2 zoomedPoint =
                ImVec2((pos.x - uv0.x) * zoomedSize.x / imageWidth, (pos.y - uv0.y) * zoomedSize.y / imageHeight);
            // 窗体起始坐标(childPos.x + 8,childPos.y + 28 )
            ImVec2 screenPoint = ImVec2(childPos.x + 8 + zoomedPoint.x, childPos.y + 28 + zoomedPoint.y);
            ImU32 PlayerColor =
                player.IsMyTeam ? gameData.Config.color.PlayerTeamCol : gameData.Config.color.EnemyTeamCol;
            ImGui::PlayerDisplay(player.Name.c_str(), (8.0f * (1 - gameData.Radar.ZoomFactor * 0.1f)),
                                 player.AimOffsets.y, ImVec2(screenPoint.x, screenPoint.y), PlayerColor);
        } else {
            continue;
        }
    }
    ImGui::End();
}

void ImGui::ShowRadarWindow2(bool* p_open) {
    IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing Dear ImGui context. Refer to examples app!");
    // 创建ImGui窗口，大小与hwnd窗口的高度相匹配，无边框
    ImVec2 original_size = {ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y};
    ImVec2 original_pos = {0, 0};
    ImGui::SetNextWindowSize(ImVec2(original_size.x, original_size.y));
    ImGui::SetNextWindowPos(original_pos);
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;   // 禁用标题栏
    window_flags |= ImGuiWindowFlags_NoScrollbar;  // 禁用滚动条
    window_flags |= ImGuiWindowFlags_NoMove;       // 禁止用户拖动
    window_flags |= ImGuiWindowFlags_NoResize;     // 禁止用户使用鼠标调整大小并禁用窗口边框
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;  // 聚焦时禁止将窗口置为上层
    window_flags |= ImGuiWindowFlags_NoScrollWithMouse;  // 禁用用户使用鼠标滚轮垂直滚动，但如果设置了
    // NoScrollbar，则鼠标滚轮会转发给父窗口

    if (!ImGui::Begin("ReadWindow", p_open, window_flags)) {
        ImGui::End();
        if (ImGui::Map) {
            ID3D11ShaderResourceView* d3dTexture = static_cast<ID3D11ShaderResourceView*>(ImGui::Map);
            d3dTexture->Release();
        }
        return;
    }
    // 开始位置
    ImVec2 CursorPos = ImVec2((original_size.x - original_size.y) * 0.5f, 0);
    CursorPos = {CursorPos.x - ((gameData.Radar.ZoomFactor * original_size.y - original_size.y) * 0.5f),
                 CursorPos.y - ((gameData.Radar.ZoomFactor * original_size.y - original_size.y) * 0.5f)};
    // 图片大小
    ImVec2 ImageSize = ImVec2(original_size.y * gameData.Radar.ZoomFactor, original_size.y * gameData.Radar.ZoomFactor);
    // // 获取鼠标滚轮的变化值
    float scroll_delta = ImGui::GetIO().MouseWheel;
    if (scroll_delta != 0.0f) {
        gameData.Radar.ZoomFactor += scroll_delta * 0.5f;
        gameData.Radar.ZoomFactor = gameData.Radar.ZoomFactor < 1.0f ? 1.0f : gameData.Radar.ZoomFactor;
        ImageSize = {original_size.y * gameData.Radar.ZoomFactor, original_size.y * gameData.Radar.ZoomFactor};
        // CursorPos = {CursorPos.x - (ImageSize.x - original_size.y) * 0.5f,
        //              CursorPos.x - (ImageSize.y - original_size.y) * 0.5f};
    }
    ImGui::SetCursorPos(CursorPos);
    ImGui::Image(ImGui::Map, ImageSize * 1.1, ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f));
    ImGui::End();
}

#endif  // IMGUI_DISABLE