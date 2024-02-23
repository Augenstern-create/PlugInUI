﻿#include "Radar.h"
#include "GNames.h"
#include "SrcUtils.h"

FVector2D DefaultSize = {1080.0f, 1080.0f};

bool Radar::GetMap() {
    float MapSize = 0.0f;
    float raderMapSize = 0.0f;
    std::string MapName = gameData.mapRadar.map_name;
    if (MapName == "Tiger_Main" || MapName == "Kiki_Main" || MapName == "Desert_Main" || MapName == "Neon_Main") {
        MapSize = 408000.f;
    } else if (MapName == "Desert_Main" || MapName == "Neon_Main") {
        MapSize = 406400.f;
    } else if (MapName == "Range_Main" || MapName == "Summerland_Main" || MapName == "Italy_Main") {
        MapSize = 101175.f;
    } else if (MapName == "Baltic_Main") {
        MapSize = 406387.5;
    } else if (MapName == "Heaven_Main") {
        MapSize = 51420.f;
    } else if (MapName == "Savage_Main") {
        MapSize = 202387.5f;
    } else if (MapName == "DihorOtok_Main") {
        MapSize = 408000.f;
    } else if (MapName == "Chimera_Main") {
        MapSize = 152950.f;
    }

    if (MapName == "Erangel_Main" || MapName == "Baltic_Main" || MapName == "Desert_Main" || MapName == "Kiki_Main" || MapName == "Tiger_Main" ||
        MapName == "DihorOtok_Main" || MapName == "Neon_Main") {
        raderMapSize = 816000.f;
    } else if (MapName == "Savage_Main") {
        raderMapSize = 408000.f;
    } else if (MapName == "Summerland_Main" || MapName == "Range_Main") {
        raderMapSize = 204000.f;
    } else if (MapName == "Chimera_Main") {
        raderMapSize = 306000.f;
    } else if (MapName == "Heaven_Main" || MapName == "Boardwalk_Main" || MapName == "Italy_Main") {
        raderMapSize = 102000.f;
    }

    gameData.mapRadar.map_size = MapSize;
    gameData.mapRadar.radar_size = raderMapSize;

    if (MapSize > 0) {
        return true;
    } else {
        return false;
    }
}

// struct FTslWidgetState {
//     unsigned char Pad[0x30];
//     unsigned char WidgetClass[0x8];
//     unsigned char pad_38[0x8];
//     ULONG64 Widget;
//     unsigned char pad_48[0x20];
// };
// bool Radar::GetMiniMap() {
//     auto WidgetStateMap = Drive::ReadValue<TMap<FString, FTslWidgetState>>(gameData.MyHUD + gameData.Offset["WidgetStateMap"]);

//     for (auto& Elem : WidgetStateMap.GetVector()) {
//         auto& Key = Elem.Value.Key;
//         auto& Value = Elem.Value.Value;
//         if (Utils::ValidPtr(Value.Widget)) continue;
//         ULONG ID = Decrypt::CIndex(Drive::ReadValue<ULONG>(Value.Widget + gameData.Offset["ObjID"]));
//         std::string WidgetPtrName = GNames::GetName(ID);

//         if (WidgetPtrName == "MinimapOriginalType_C") {
//             gameData.Radar.MiniMapWidget = Value.Widget;
//             return true;
//         }
//     }
//     return false;
// }

bool Radar::GetMiniMap() {
    DWORD_PTR minMapPtr = VmmCore::ReadValue<DWORD_PTR>(gameData.MyHUD + Offset::WidgetStateMap);
    int minMapInt = VmmCore::ReadValue<int>(gameData.MyHUD + Offset::WidgetStateMap + 0x8);
    for (int i = 0; i < 500; i++) {
        DWORD_PTR minMapData = VmmCore::ReadValue<DWORD_PTR>(minMapPtr + (i * 0x8));
        int ID = Decrypt::CIndex(VmmCore::ReadValue<int>(minMapData + Offset::ObjID));
        std::string WidgetName = GNames::GetNameByID(ID);
        if (WidgetName == "MinimapOriginalType_C") {
            gameData.mapRadar.small_map_radar = minMapData;
            return true;
        }
    }

    return false;
}

bool Radar::GetMapGrid() {
    uint64_t BlockInputWidgetList = VmmCore::ReadValue<uint64_t>(gameData.MyHUD + Offset::BlockInputWidgetList);
    int BlockInputWidgetListCount = VmmCore::ReadValue<int>(gameData.MyHUD + Offset::BlockInputWidgetList + 0x8);
    for (int i = 0; i < 500; i++) {
        uint64_t Widget = VmmCore::ReadValue<uint64_t>(BlockInputWidgetList + i * 0x8);
        int ID = Decrypt::CIndex(VmmCore::ReadValue<int>(Widget + Offset::ObjID));
        std::string WidgetName = GNames::GetNameByID(ID);
        if (WidgetName == "NewWorldMapWidget_BP_C") {
            gameData.mapRadar.map_grid = VmmCore::ReadValue<uint64_t>(Widget + Offset::TrainingMapGrid + 0x30);
            gameData.mapRadar.atlas_radar = Widget;
            return true;
        }
        if (WidgetName == "UI_TrainingWorldMapWidget_C") {
            gameData.mapRadar.map_grid = VmmCore::ReadValue<uint64_t>(Widget + Offset::TrainingMapGrid);
            gameData.mapRadar.atlas_radar = Widget;
            return true;
        }
    }

    return false;
}

bool Radar::GetVisibility() {
    auto Visibility = VmmCore::ReadValue<ESlateVisibility>(gameData.mapRadar.atlas_radar + Offset::Visibility) == ESlateVisibility::SelfHitTestInvisible;
    gameData.mapRadar.is_ibility = Visibility;
    return gameData.mapRadar.is_ibility;
}

float Radar::GetZoomFactor() {
    gameData.mapRadar.map_address = VmmCore::ReadValue<DWORD_PTR>(gameData.mapRadar.map_grid + Offset::Slot);
    if (Utils::ValidPtr(gameData.mapRadar.map_address)) return 0.0f;
    gameData.mapRadar.declare = VmmCore::ReadValue<FMargin>(gameData.mapRadar.map_address + Offset::LayoutData + 0x0);
    const FVector2D CurrentSize = {gameData.mapRadar.declare.Right, gameData.mapRadar.declare.Bottom};
    auto factor = CurrentSize.X / DefaultSize.X;
    gameData.mapRadar.map_zoom_value = factor;
    return factor;
};

FVector2D Radar::GetPosition() {
    auto Alignment = VmmCore::ReadValue<FVector2D>(gameData.mapRadar.map_address + Offset::LayoutData + 0x0 + Offset::Alignment);
    const FVector2D CurrentPos = {gameData.mapRadar.declare.Right * (Alignment.X - 0.5f) - gameData.mapRadar.declare.Left,
                                  gameData.mapRadar.declare.Bottom * (Alignment.Y - 0.5f) - gameData.mapRadar.declare.Top};
    gameData.mapRadar.position = {CurrentPos.X / DefaultSize.X / gameData.mapRadar.map_zoom_value * 2.0f,
                                  CurrentPos.Y / DefaultSize.Y / gameData.mapRadar.map_zoom_value * 2.0f};
    return gameData.mapRadar.position;
}

void Radar::Update() {
    while (true) {
        if (gameData.Scene != Scene::Gameing) continue;

        if (gameData.mapRadar.map_size <= 0) {
            if (!Radar::GetMap()) {
                Sleep(10);
                continue;
            }
        }

        if (Utils::ValidPtr(gameData.mapRadar.small_map_radar)) {
            if (!Radar::GetMiniMap()) {
            }
        }
        if (Utils::ValidPtr(gameData.mapRadar.map_grid)) {
            if (!Radar::GetMapGrid()) {
            }
        }
        if (!gameData.mapRadar.is_ibility) {
            if (!Radar::GetVisibility()) {
            }
        }

        Radar::GetZoomFactor();
        Radar::GetPosition();

        const float MapSizeFactored = gameData.mapRadar.map_size / gameData.mapRadar.map_zoom_value;
        const Vector3 WorldCenterLocation = {gameData.mapRadar.map_size * (1.0f + gameData.mapRadar.position.X),
                                             gameData.mapRadar.map_size * (1.0f + gameData.mapRadar.position.Y), 0.0f};
    }
}