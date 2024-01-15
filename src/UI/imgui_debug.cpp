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

void ImGui::ShowDebugWindow(bool* p_open, ImVec2 display_size) {
    IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing Dear ImGui context. Refer to examples app!");
    ImGui::SetNextWindowSize({display_size.x, display_size.y * 0.5f}, ImGuiCond_FirstUseEver);
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
            ImGui::Text("UWorld: %lld", gameData.UWorld);
            ImGui::Text("CurrentLevel: %lld", gameData.CurrentLevel);
            ImGui::Text("GNames: %lld", gameData.GNames);
            ImGui::Text("GameInstance: %lld", gameData.GameInstance);
            ImGui::Text("GameState: %lld", gameData.GameState);
            ImGui::Text("LocalPlayer: %lld", gameData.LocalPlayer);
            ImGui::Text("PlayerController: %lld", gameData.PlayerController);
            ImGui::Text("AcknowledgedPawn: %lld", gameData.AcknowledgedPawn);
            ImGui::Text("PlayerCameraManager: %lld", gameData.PlayerCameraManager);
            ImGui::Text("Actor: %lld", gameData.Actor);
            ImGui::Text("MyHUD: %lld", gameData.MyHUD);
            ImGui::Text("Scene: %d", (int)gameData.Scene);
            ImGui::Text("MapName: %s", gameData.Map.MapName.c_str());
            ImGui::Text("MapPageIndex: %d", gameData.Map.MapPageIndex);
            ImGui::Text("Windows Form Width: %d  Height: %d", gameData.ScreenWidth, gameData.ScreenHeight);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("entity")) {
            auto entitys = Data::GetCacheEntity();
            ImGui::Text("size: %d", entitys.size());
            ImGui::Columns(4, nullptr, false);
            for (auto entity : entitys) {
                ImGui::Text("Entity: %lld", entity.Entity);
                ImGui::NextColumn();
                ImGui::Text("ID: %d", entity.ID);
                ImGui::NextColumn();
                ImGui::Text("decodeID: %d", entity.decodeID);
                ImGui::NextColumn();
                ImGui::Text("type: %d", (int)entity.type);
                ImGui::NextColumn();
            }
            ImGui::Columns(1);
            ImGui::Text("end");

            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Player")) {
            ImGui::Text("ActorCount: %d", gameData.ActorCount);
            auto Players = Data::GetPlayers();
            ImGui::Columns(6, nullptr, false);
            ImGui::SetColumnWidth(0, display_size.x / 8.0f);
            ImGui::SetColumnWidth(1, display_size.x / 6.0f);
            ImGui::SetColumnWidth(2, display_size.x / 8.0f);
            ImGui::SetColumnWidth(3, display_size.x / 6.0f);
            ImGui::SetColumnWidth(4, display_size.x / 6.0f);
            ImGui::SetColumnWidth(5, display_size.x / 4.0f);
            for (auto entity : Players) {
                ImGui::Text("Entity: %lld", entity.Entity);
                ImGui::NextColumn();
                ImGui::Text("Name: %s", entity.Name.c_str());
                ImGui::NextColumn();
                ImGui::Text("TeamID: %d", entity.TeamID);
                ImGui::NextColumn();
                ImGui::Text("AimOffsets: %f", entity.AimOffsets.y);
                ImGui::NextColumn();
                ImGui::Text("Distance: %f", entity.Distance);
                ImGui::NextColumn();
                ImGui::Text("Location: x: %f  y: %f  z: %f", entity.Location.x, entity.Location.y, entity.Location.z);
                ImGui::NextColumn();
            }
            ImGui::Columns(1);
            ImGui::Text("end");
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("skeleton")) {
            auto Players = Data::GetPlayers();
            ImGui::Columns(2, nullptr, false);
            for (auto entity : Players) {
                auto skeletons = entity.Skeleton.ScreenBones;
                auto Location = skeletons[EBoneIndex::forehead];
                ImGui::Text("Name: %s", entity.Name.c_str());
                ImGui::NextColumn();
                ImGui::Text("Location: x: %f  y: %f  z: %f", Location.x, Location.y, Location.z);
                ImGui::NextColumn();
            }
            ImGui::Columns(1);
            ImGui::Text("end");

            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Project")) {
            auto Project = Data::GetProjects();
            ImGui::Columns(6, nullptr, false);
            for (auto entity : Project) {
                ImGui::Text("Entity: %lld", entity.Entity);
                ImGui::NextColumn();
                ImGui::Text("Name: %s", entity.Name.c_str());
                ImGui::NextColumn();
                ImGui::Text("Distance: %f", entity.Distance);
                ImGui::NextColumn();
                ImGui::Text("TimeTillExplosion: %f", entity.TimeTillExplosion);
                ImGui::NextColumn();
                ImGui::Text("Location: x: %f  y: %f  z: %f", entity.Location.x, entity.Location.y, entity.Location.z);
                ImGui::NextColumn();
                ImGui::Text("ScreenLocation: x: %f  y: %f  z: %f", entity.ScreenLocation.x, entity.ScreenLocation.y, entity.ScreenLocation.z);
                ImGui::NextColumn();
            }
            ImGui::Columns(1);
            ImGui::Text("end");
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("items")) {
            auto items = Data::GetItems();
            ImGui::Columns(5, nullptr, false);
            for (auto entity : items) {
                ImGui::Text("Entity: %lld", entity.Entity);
                ImGui::NextColumn();
                ImGui::Text("Name: %s", entity.Name.c_str());
                ImGui::NextColumn();
                ImGui::Text("Distance: %f", entity.Distance);
                ImGui::NextColumn();
                ImGui::Text("Location: x: %f  y: %f  z: %f", entity.Location.x, entity.Location.y, entity.Location.z);
                ImGui::NextColumn();
                ImGui::Text("ScreenLocation: x: %f  y: %f  z: %f", entity.ScreenLocation.x, entity.ScreenLocation.y, entity.ScreenLocation.z);
                ImGui::NextColumn();
            }
            ImGui::Columns(1);
            ImGui::Text("end");
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("ItemGroupInfo")) {
            auto ItemGroupInfos = Data::GetItemGroups();
            ImGui::Columns(2, nullptr, false);
            for (auto entity : ItemGroupInfos) {
                ImGui::Text("Entity: %lld", entity.Entity);
                ImGui::NextColumn();
                ImGui::Text("Items: %lld", entity.Items);
                ImGui::NextColumn();
            }
            ImGui::Columns(1);
            ImGui::Text("end");
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Radar")) {
            ImGui::Text("Map name: %s", gameData.Map.MapName.c_str());
            ImGui::Text("MapWidget: %lld", gameData.Map.MapWidget);
            ImGui::Text("MapGrid: %lld", gameData.Map.MapGrid);
            ImGui::Text("Slot: %lld", gameData.Map.Slot);
            ImGui::Text("size: %f", gameData.Map.MapSize);
            ImGui::Text("zoomSize: %f", gameData.Map.MapZoomValue);
            ImGui::Text("ImageMapSize: %f", gameData.Radar.ImageMapSize);
            ImGui::Text("ZoomFactor: %f", gameData.Map.MapZoomValue);
            ImGui::Text("Visibility: %d", (int)gameData.Map.Visibility);
            ImGui::Text("Position x: %f  y: %f", gameData.Map.Position.X, gameData.Map.Position.Y);
            ImGui::Text("Layout Left: %f  Top: %f  Right: %f  Bottom: %f", gameData.Map.Layout.Left, gameData.Map.Layout.Top, gameData.Map.Layout.Right,
                        gameData.Map.Layout.Bottom);
            ImGui::Text("WorldOriginLocation x: %f  y: %f", gameData.Map.WorldOriginLocation.x, gameData.Map.WorldOriginLocation.y);
            ImGui::Text("ImageMapSize: %f", gameData.Radar.ImageMapSize);
            ImGui::Text("ZoomFactor: %f", gameData.Radar.ZoomFactor);
            ImGui::Text("ScreenCenter x: %f  y: %f", gameData.Radar.ScreenCenter.X, gameData.Radar.ScreenCenter.Y);
            // fov
            ImGui::Text("FOV: %f", gameData.FOV);
            ImGui::Text("Location x: %f y: %f z: %f", gameData.Location.x, gameData.Location.y, gameData.Location.z);
            ImGui::Text("Rotation x: %f y: %f z: %f", gameData.Rotation.x, gameData.Rotation.y, gameData.Rotation.z);
            ImGui::Text("ScreenSize x: %f  y: %f", gameData.ScreenWidth, gameData.ScreenHeight);

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Myself")) {
            ImGui::Text("Name: %s", gameData.Myself.Name.c_str());
            ImGui::Text("ClanName: %s", gameData.Myself.ClanName.c_str());
            ImGui::Text("TeamID: %d", gameData.Myself.TeamID);
            ImGui::Text("Mesh: %lld", gameData.Myself.Mesh);
            ImGui::Text("AnimScript: %lld", gameData.Myself.AnimScript);
            ImGui::Text("PlayerPtr: %lld", gameData.Myself.PlayerPtr);

            ImGui::EndTabItem();
        }
    }

    ImGui::End();
}

#endif  // IMGUI_DISABLE