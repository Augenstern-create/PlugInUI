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

bool isDrawing(std::unordered_map<int, Vector3> skeletons) {
    ImVec2 infoPos = {skeletons[forehead].x, skeletons[forehead].y};
    ImVec2 formSize = gameData.skeletonConfig.windows_form_size;
    if (infoPos.x <= 0.0f || infoPos.y <= 0.0f) {
        return false;
    } else if (infoPos.x >= formSize.x || infoPos.y >= formSize.y) {
        return false;
    } else {
        return true;
    }
}

bool DrawingPlayer(PlayerInfo Player) {
    auto skeletons = Player.Skeleton.ScreenBones;
    // 判断是否绘制
    {
        // 本人跳过
        if (Player.Entity == gameData.Myself.PlayerPtr) {
            return false;
        }
        // 不在训练营倒地生命为0跳过
        if (gameData.mapRadar.map_name != "Range_Main" && Player.GroggyHealth <= 0) {
            return false;
        }

        if (!isDrawing(skeletons)) {
            gameData.skeletonConfig.blind_spot_quorum++;
            return false;
        }
        gameData.skeletonConfig.faced_with_quorum++;
    }

    auto& Config = gameData.skeletonConfig;
    ImVec2 infoPos = {skeletons[forehead].x, skeletons[forehead].y};
    ImVec2 namePos = {infoPos.x, infoPos.y - 40.0f};
    bool is_team = (Player.TeamID == gameData.Myself.TeamID);
    ImVec2 distancePos = {skeletons[forehead].x - Config.skeleton_font_size * 1.2f, skeletons[forehead].y};

    // 绘制组件
    {
        // 绘制名字
        if (Config.is_drawing_names) {
            std::string name = Player.Name;
            ImU32 playerNameColor = is_team ? Config.team_name_color : Config.foes_name_color;
            if (Config.is_drawing_team_name) {
                name = Player.TeamName + name;
            }
            ImVec2 textSize = ImGui::CalcTextSize(name.c_str());
            textSize = textSize * Config.font_zoom_size;
            namePos.x -= (textSize.x * 0.25f);
            ImGui::DrawPlayerName(name.c_str(), namePos, Config.skeleton_font_size, playerNameColor);
        }
        // 绘制队伍ID
        if (Config.is_drawing_teamID) {
            std::string txt = std::to_string(Player.TeamID);
            ImVec2 txtSize = ImGui::CalcTextSize(txt.c_str());
            txtSize = txtSize * Config.font_zoom_size;
            ImVec2 TeamIDPos = {namePos.x - Config.skeleton_font_size - 2.0f, namePos.y};
            ImU32 TeamIDCol = Config.teamID_color;
            ImGui::DrawPlayerTeamID(txt.c_str(), TeamIDPos, Config.skeleton_font_size, Config.font_zoom_size, TeamIDCol);
        }
        // 绘制血量
        if (Config.is_drawing_hp) {
            float health = Player.Health <= 0 ? Player.GroggyHealth : Player.Health;
            float size = 1.0f - Player.Distance / 1000.0f;
            health = (health / (Player.Health <= 0 ? Player.GroggyHealthMax : Player.HealthMax)) * 100.0f;
            ImU32 healthCol = Player.Health > 0 ? Config.hp_color : Config.groggy_hp_color;
            ImGui::DrawPlayerHitPoint(health, infoPos, size, healthCol);
        }
        // 绘制武器
        if (Config.is_drawing_handheld_weapon) {
            std::string WeaponName = Player.WeaponName;
            int WeaponID = Player.WeaponID;
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
                txtSize = txtSize * Config.font_zoom_size;
                WeaponPos.x -= txtSize.x * 0.5f;
                WeaponPos.y -= Config.skeleton_font_size;
                ImGui::RenderCustomSizedText(WeaponPos, WeaponName.c_str(), Config.skeleton_font_size);
            }
        }
        // 绘制骨骼/方框
        {
            ImVec2 Screen = Config.windows_form_size;

            if (Player.Distance > Config.skeleton_show_distance) {
                if (Config.is_drawing_box) {
                    ImU32 skeletonColor = Config.box_color;
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
                if (Config.is_drawing_skeleton) {
                    ImU32 skeletonColor = Config.skeleton_color;
                    ImGui::DrawPlayerBones(Player.Name.c_str(), 2, skeletons, Screen, skeletonColor);
                    float stopPos = skeletons[foot_r].y > skeletons[foot_l].y ? skeletons[foot_r].y : skeletons[foot_l].y;
                    distancePos.y = stopPos + 5.0f;
                }
            }
        }
        // 绘制距离
        if (Config.is_drawing_distance) {
            ImU32 distanceColor = is_team ? Config.team_name_color : Config.foes_name_color;
            std::string data = std::to_string((int)Player.Distance);
            data += "M";
            ImVec2 txtSize = ImGui::CalcTextSize(data.c_str());
            distancePos.x += txtSize.x * 0.5f;
            ImGui::RenderCustomSizedText(distancePos, data.c_str(), Config.skeleton_font_size, distanceColor);
        }
    }
}

static float lastFrameTime = 0.0f;

static ImVec2 g_display_size = {0, 0};

void ImGui::ShowSkeletonWindow(bool* p_open, ImVec2 display_size) {
    auto& Config = gameData.skeletonConfig;
    // 创建窗口
    {
        if (Config.windows_form_size != display_size) {
            ImGui::SetNextWindowSize(display_size, ImGuiCond_Always);
            Config.windows_form_size = display_size;
        }
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
    }

    // 帧率
    {
        float currentTime = (float)ImGui::GetTime();
        float deltaTime = currentTime - Config.time_per_frame;
        Config.time_per_frame = currentTime;
        float fps = 1.0f / deltaTime;
        std::string fpss = "FPS: " + std::to_string(fps);
        ImGui::RenderCustomSizedText({10, 10}, fpss.c_str(), Config.skeleton_font_size * 2, Config.frame_rate_color);
    }
    // 游戏内
    {
        if (gameData.Scene == Scene::Gameing) {
            Config.font_zoom_size = Config.skeleton_font_size / gameData.setting.default_font_size;
            auto Players = Data::GetPlayers();
            for (auto Player : Players) {
                DrawingPlayer(Player);
            }

            // 绘制大小地图雷达
            {
                if (gameData.mapRadar.max_map.is_ibility && Config.is_drawing_atlas_radar) {
                    ImVec2 form_size = {display_size.x, display_size.y};
                    auto world_location = gameData.mapRadar.world_location;
                    auto map_size = gameData.mapRadar.map_size;

                    for (auto Player : Players) {
                        if (Player.TeamID == gameData.Myself.TeamID) continue;
                        std::string data = std::to_string((int)Player.TeamID);
                        ImVec2 location = {0.0f, 0.0f};
                        ImGui::PlayerDisplay(data.c_str(), Config.max_radar_size, Player.AimOffsets, location, Config.max_radar_color)
                    }
                }
            }

            // 游戏内信息
            {
                std::string text = "面前[" + std::to_string(Config.faced_with_quorum) + "]人。视野盲区[" + std::to_string(Config.blind_spot_quorum) + "]人。";
                ImVec2 txtSize = ImGui::CalcTextSize(text.c_str());
                ImVec2 textPos = {Config.windows_form_size.x * 0.5f - (txtSize.x * Config.font_zoom_size) * 0.5f, Config.windows_form_size.y * 0.8f};
                ImGui::RenderCustomSizedText(textPos, text.c_str(), Config.skeleton_font_size * 1.5f, Config.in_game_info_color);
                Config.faced_with_quorum = 0;
                Config.blind_spot_quorum = 0;
            }
        }
    }

    ImGui::End();
}

#endif  // IMGUI_DISABLE
