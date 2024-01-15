#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "imgui_skeleton.h"
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
#include <algorithm>
#include <limits>

float Min(float values, float values2) { return values < values2 ? values : values2; }
float Max(float values, float values2) { return values > values2 ? values : values2; }

float findMin(float values[], int size) {
    if (size <= 0) {
        return 0.0f;
    }
    float min_value = values[0];
    for (int i = 1; i < size; ++i) {
        if (values[i] < min_value) {
            min_value = values[i];
        }
    }
    return min_value;
}

float findMax(float values[], int size) {
    if (size <= 0) {
        return 0.0f;
    }
    float max_value = values[0];
    for (int i = 1; i < size; ++i) {
        if (values[i] > max_value) {
            max_value = values[i];
        }
    }
    return max_value;
}

static float lastFrameTime = 0.0f;

void ImGui::ShowSkeletonWindow(bool* p_open, ImVec2 display_size) {
    IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing Dear ImGui context. Refer to examples app!");
    ImGui::SetNextWindowSize(display_size);
    ImGui::SetNextWindowPos({0, 0});
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_None;                   // 0
    window_flags |= ImGuiWindowFlags_NoTitleBar;             // 禁用标题栏
    window_flags |= ImGuiWindowFlags_NoScrollbar;            // 禁用滚动条
    window_flags |= ImGuiWindowFlags_NoMove;                 // 禁止用户拖动
    window_flags |= ImGuiWindowFlags_NoResize;               // 禁止用户使用鼠标调整大小并禁用窗口边框
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;  // 聚焦时禁止将窗口置为上层

    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);

    if (!ImGui::Begin("SkeletonWindow", p_open, window_flags)) {
        ImGui::End();
        return;
    }
    float currentTime = (float)ImGui::GetTime();
    float deltaTime = currentTime - lastFrameTime;
    lastFrameTime = currentTime;
    float fps = 1.0f / deltaTime;
    std::string fpss = "FPS: " + std::to_string(fps);
    ImGui::RenderCustomSizedText({10, 10}, fpss.c_str(), gameData.Config.Slider.fontSize * 2, IM_COL32(60, 255, 60, 255));
    if (gameData.Scene == Scene::Gameing) {
        auto Players = Data::GetPlayers();
        for (auto Player : Players) {
            if (Player.Entity == gameData.Myself.PlayerPtr) {
                continue;
            }
            if (gameData.Map.MapName != "Range_Main" && Player.GroggyHealth <= 0) continue;
            auto skeletons = Player.Skeleton.ScreenBones;

            float skeletonFontSize = gameData.Config.Slider.skeletonfontSize;
            float fontSize = ImGui::GetFontSize();
            float fontZoom = skeletonFontSize / fontSize;
            ImVec2 infoPos = {skeletons[forehead].x, skeletons[forehead].y};
            ImVec2 namePos = {infoPos.x, infoPos.y - 40.0f - (skeletonFontSize * 0.5f)};
            ImVec2 distancePos = {skeletons[forehead].x - skeletonFontSize * 1.2f, skeletons[forehead].y};

            // 名字
            if (gameData.Config.Box.showPlayerName) {
                std::string name = Player.Name;
                ImU32 playerNameColor = gameData.Config.Color.playerNameColor;
                if (gameData.Config.Box.showTeamName) {
                    name = Player.TeamName + name;
                }
                ImVec2 textSize = ImGui::CalcTextSize(name.c_str());
                textSize = textSize * fontZoom;
                namePos.x -= (textSize.x * 0.5f);
                ImGui::DrawPlayerName(name.c_str(), namePos, skeletonFontSize, playerNameColor);
            }
            // 队伍id
            if (gameData.Config.Box.showTeamNumber) {
                std::string txt = std::to_string(Player.TeamID);
                ImVec2 txtSize = ImGui::CalcTextSize(txt.c_str());
                txtSize = txtSize * fontZoom;
                ImVec2 TeamIDPos = {namePos.x - txtSize.x * 2.0f - 2.0f, namePos.y};
                ImVec2 TeamIDPos2 = {TeamIDPos.x + txtSize.x * 0.5f, TeamIDPos.y + txtSize.y * 0.5f};
                ImU32 TeamIDCol = Player.TeamID == gameData.Myself.TeamID ? gameData.Config.Color.collaboratorInfoColor : gameData.Config.Color.dangerInfoColor;
                ImGui::DrawPlayerTeamID(txt.c_str(), TeamIDPos, TeamIDPos2, skeletonFontSize, TeamIDCol);
            }

            // 显示血量
            if (gameData.Config.Box.showBloodstain) {
                float health = Player.Health <= 0 ? Player.GroggyHealth : Player.Health;
                health = (health / (Player.Health <= 0 ? Player.GroggyHealthMax : Player.HealthMax)) * 100.0f;
                ImU32 healthCol = Player.Health >= 0 ? IM_COL32(255, 255, 60, 255) : gameData.Config.Color.downBloodColor;
                ImGui::DrawPlayerHitPoint(health, infoPos, skeletonFontSize, healthCol);
            }

            // 绘制骨骼/方框
            {
                ImVec2 Screen = {(float)gameData.ScreenWidth, (float)gameData.ScreenHeight};
                ImU32 skeletonColor = gameData.Config.Color.visualBoneColor;
                if (!Player.IsSkeleton) skeletonColor = gameData.Config.Color.invisibleBoneColor;
                if (Player.Health <= 0) skeletonColor = gameData.Config.Color.fallenSkeletonColor;
                if (Player.Distance > gameData.Config.Slider.skeletonDistance) {
                    if (gameData.Config.Box.showBox) {
                        float minX = (float)gameData.ScreenWidth;
                        float minY = (float)gameData.ScreenHeight;
                        float maxX = 0.0f;
                        float maxY = 0.0f;

                        for (const auto& pair : skeletons) {
                            const Vector3& vector = pair.second;
                            minX = Min(minX, vector.x);
                            maxX = Max(maxX, vector.x);
                            minY = Min(minY, vector.y);
                            maxY = Max(maxY, vector.y);
                        }

                        ImVec2 startPos = {-5.0f + minX, -5.0f + minY};
                        ImVec2 stopPos = {5.0f + maxX, 5.0f + maxY};
                        ImGui::DrawPlayerBox(startPos, stopPos, Screen, skeletonColor);
                        distancePos.y = stopPos.y + 5.0f;
                    }

                } else {
                    if (gameData.Config.Box.showSkeleton) {
                        ImGui::DrawPlayerBones(Player.Name.c_str(), 2, skeletons, Screen, skeletonColor);
                        float stopPos = skeletons[foot_r].y > skeletons[foot_l].y ? skeletons[foot_r].y : skeletons[foot_l].y;
                        distancePos.y = stopPos + 5.0f;
                    }
                }
            }

            // 绘制距离
            if (gameData.Config.Box.showLength) {
                ImU32 distanceColor =
                    Player.TeamID == gameData.Myself.TeamID ? gameData.Config.Color.collaboratorInfoColor : gameData.Config.Color.dangerInfoColor;
                std::string data = std::to_string((int)Player.Distance);
                data += "M";
                ImGui::RenderCustomSizedText(distancePos, data.c_str(), skeletonFontSize, distanceColor);
            }

            // 绘制武器
            if (gameData.Config.Box.showHandheldWeapon) {
                std::string WeaponName = Player.WeaponName;
                int WeaponID = Player.WeaponID;
                ImU32 playerNameColor = gameData.Config.Color.playerNameColor;
                ImVec2 WeaponPos = {infoPos.x, namePos.y};
                int width = 0, height = 0;
                if (WeaponID != 0) {
                    auto TextureID = Weapon::GetTextureID(WeaponID, &width, &height);
                    width *= 0.15f;
                    height *= 0.15f;
                    WeaponPos.x -= width * 0.5f;
                    WeaponPos.y -= height;
                    ImGui::DrawPlayerArms(reinterpret_cast<ImTextureID>(static_cast<intptr_t>(TextureID)), WeaponPos, {(float)width, (float)height});
                } else {
                    ImVec2 txtSize = ImGui::CalcTextSize(WeaponName.c_str());
                    txtSize = txtSize * fontZoom;
                    WeaponPos.x -= txtSize.x * 0.5f;
                    WeaponPos.y -= skeletonFontSize;
                    ImGui::RenderCustomSizedText(WeaponPos, WeaponName.c_str(), skeletonFontSize, playerNameColor);
                }
            }
        }
    }

    ImGui::End();
}

#endif  // IMGUI_DISABLE
