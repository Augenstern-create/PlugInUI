#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "imgui_debug.h"
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
#include "game/Constant.h"
#include "game/Data.h"

static ImVec2 g_display_size = {0, 0};

void ImGui::ShowDebugWindow(bool* p_open, ImVec2 display_size) {
    if (g_display_size != display_size) {
        ImGui::SetNextWindowSize(ImVec2(display_size.x * 0.5f, display_size.y * 0.5f), ImGuiCond_Always);
    }
    // ImGui::SetNextWindowPos({0, 0});
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_None;  // 0

    if (!ImGui::Begin("DebugWindow", p_open, window_flags)) {
        ImGui::End();
        return;
    }

    if (ImGui::BeginTabBar("DeBugTabs", ImGuiTabBarFlags_None)) {
        if (ImGui::BeginTabItem("Main")) {
            ImGui::Text("PID: %d", gameData.PID);
            ImGui::Text("GameBase: %lld", gameData.GameBase);
            ImGui::Text("GNames: %lld", gameData.GNames);
            ImGui::Text("UWorld: %lld", gameData.UWorld);
            ImGui::Text("GameState: %lld", gameData.GameState);
            ImGui::Text("CurrentLevel: %lld", gameData.CurrentLevel);
            ImGui::Text("GameInstance: %lld", gameData.GameInstance);
            ImGui::Text("LocalPlayer: %lld", gameData.LocalPlayer);
            ImGui::Text("PlayerController: %lld", gameData.PlayerController);
            ImGui::Text("Actor: %lld", gameData.Actor);
            ImGui::Text("AcknowledgedPawn: %lld", gameData.AcknowledgedPawn);
            ImGui::Text("PlayerCameraManager: %lld", gameData.PlayerCameraManager);
            ImGui::Text("ViewTarget: %lld", gameData.ViewTarget);
            ImGui::Text("MyHUD: %lld", gameData.MyHUD);
            ImGui::Text("Scene: %d", (int)gameData.Scene);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("MapRadar")) {
            ImGui::Text("map_id: %d", gameData.mapRadar.map_id);
            ImGui::Text("map_name: %s", gameData.mapRadar.map_name.c_str());
            ImGui::Text("map_size: %f", gameData.mapRadar.map_size);
            ImGui::Text("radar_size: %f", gameData.mapRadar.radar_size);
            ImGui::Text("world_location: x: %f  y: %f", gameData.mapRadar.world_location.x, gameData.mapRadar.world_location.y);
            {
                ImGui::NextColumn();
                auto map = gameData.mapRadar.max_map;
                ImGui::Text("max_map_grid: %lld", map.map_grid);
                ImGui::Text("max_map_radar: %lld", map.map_radar);
                ImGui::Text("max_map_address: %lld", map.map_address);
                ImGui::Text("max_is_ibility: %d", (int)map.is_ibility);
                ImGui::Text("max_map_zoom_value: %f", map.map_zoom_value);
                ImGui::Text("max_position: x: %f  y: %f  ", map.position.X, map.position.Y);
                ImGui::Text("max_declare: Left: %f  Top: %f  Right: %f  Bottom: %f", map.declare.Left, map.declare.Top, map.declare.Right, map.declare.Bottom);
            }
            {
                ImGui::NextColumn();
                auto map = gameData.mapRadar.min_map;
                ImGui::Text("min_map_radar: %lld", map.map_radar);
                ImGui::Text("min_map_address: %lld", map.map_address);
                ImGui::Text("min_is_ibility: %d", (int)map.is_ibility);
                ImGui::Text("min_map_zoom_value: %f", map.map_zoom_value);
                ImGui::Text("min_position: x: %f  y: %f  ", map.position.X, map.position.Y);
                ImGui::Text("min_declare: Left: %f  Top: %f  Right: %f  Bottom: %f", map.declare.Left, map.declare.Top, map.declare.Right, map.declare.Bottom);
            }

            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Gameing")) {
            ImGui::Text("PlayerCount: %d", gameData.PlayerCount);
            ImGui::Text("ActorCount: %d", gameData.ActorCount);
            ImGui::Text("FOV: %f", gameData.FOV);
            ImGui::Text("width: %d  height:　%d", gameData.ScreenWidth, gameData.ScreenHeight);
            ImGui::Text("Location: x: %f  y: %f  z: %f", gameData.Location.x, gameData.Location.y, gameData.Location.z);
            ImGui::Text("Rotation: x: %f  y: %f  z: %f", gameData.Rotation.x, gameData.Rotation.y, gameData.Rotation.z);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Myself")) {
            ImGui::Text("PlayerPtr: %lld", gameData.Myself.PlayerPtr);
            ImGui::Text("Mesh: %lld", gameData.Myself.Mesh);
            ImGui::Text("AnimScript: %lld", gameData.Myself.AnimScript);
            ImGui::Text("Name: %s", gameData.Myself.Name.c_str());
            ImGui::Text("ClanName: %s", gameData.Myself.ClanName.c_str());
            ImGui::Text("TeamID: %d", gameData.Myself.TeamID);
            ImGui::Text("Location: x: %f  y: %f  z: %f", gameData.Myself.Location.x, gameData.Myself.Location.y, gameData.Myself.Location.z);
            ImGui::Text("AtlaseLocation: x: %f  y: %f ", gameData.Myself.AtlaseLocation.x, gameData.Myself.AtlaseLocation.y);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Player")) {
            ImVec2 from_size = ImGui::GetContentRegionAvail();
            ImGui::Columns(6, nullptr, false);
            ImGui::SetColumnWidth(0, from_size.x / 7);
            ImGui::SetColumnWidth(1, from_size.x / 7);
            ImGui::SetColumnWidth(2, from_size.x / 7);
            ImGui::SetColumnWidth(3, from_size.x / 7);
            ImGui::SetColumnWidth(4, from_size.x * 2 / 7);

            ImGui::Text("PlayerPtr");
            ImGui::NextColumn();
            ImGui::Text("name");
            ImGui::NextColumn();
            ImGui::Text("TeamID");
            ImGui::NextColumn();
            ImGui::Text("Ttpe");
            ImGui::NextColumn();
            ImGui::Text("Distance");
            ImGui::NextColumn();
            ImGui::Text("Location");
            ImGui::NextColumn();

            auto Players = Data::GetPlayers();
            auto PlayersPos = gameData.mapRadar.rader_players;
            for (auto player : Players) {
                ImGui::Text("%lld", player.Entity);
                ImGui::NextColumn();
                ImGui::Text("%s", player.Name.c_str());
                ImGui::NextColumn();
                ImGui::Text("%d", player.TeamID);
                ImGui::NextColumn();
                ImGui::Text("%d", (int)player.Type);
                ImGui::NextColumn();
                ImGui::Text("%f", player.Distance);
                ImGui::NextColumn();
                ImGui::Text("x: %f  y: %f", PlayersPos[player.Entity].x, PlayersPos[player.Entity].y);
                ImGui::NextColumn();
            }
            ImGui::Columns(1);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Skeleton")) {
            ImVec2 from_size = ImGui::GetContentRegionAvail();
            ImGui::Columns(3, nullptr, false);

            ImGui::Text("PlayerPtr");
            ImGui::NextColumn();
            ImGui::Text("name");
            ImGui::NextColumn();
            ImGui::Text("forehead");
            ImGui::NextColumn();
            auto Players = Data::GetPlayers();
            for (auto player : Players) {
                auto skeletons = player.Skeleton.ScreenBones;
                ImGui::Text("%lld", player.Entity);
                ImGui::NextColumn();
                ImGui::Text("%s", player.Name.c_str());
                ImGui::NextColumn();
                ImGui::Text("x: %f  y: %f", skeletons[forehead].x, skeletons[forehead].y);
                ImGui::NextColumn();
            }
            ImGui::Columns(1);
            ImGui::EndTabItem();
        }
    }

    ImGui::End();
}

#endif  // IMGUI_DISABLE