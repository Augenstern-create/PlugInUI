#include "Radar.h"
#include "GNames.h"
#include "SrcUtils.h"

FVector2D DefaultSize = {1080.0f, 1080.0f};

bool Radar::GetMap() {
    float MapSize = 0.0f;
    std::string MapName = gameData.Map.MapName;
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

    if (MapName == "Erangel_Main" || MapName == "Baltic_Main" || MapName == "Desert_Main" || MapName == "Kiki_Main" ||
        MapName == "Tiger_Main" || MapName == "DihorOtok_Main" || MapName == "Neon_Main") {
        gameData.Radar.ImageMapSize = 816000.f;
    } else if (MapName == "Savage_Main") {
        gameData.Radar.ImageMapSize = 408000.f;
    } else if (MapName == "Summerland_Main" || MapName == "Range_Main") {
        gameData.Radar.ImageMapSize = 204000.f;
    } else if (MapName == "Chimera_Main") {
        gameData.Radar.ImageMapSize = 306000.f;
    } else if (MapName == "Heaven_Main" || MapName == "Boardwalk_Main" || MapName == "Italy_Main") {
        gameData.Radar.ImageMapSize = 102000.f;
    }

    gameData.Map.MapSize = MapSize;

    if (MapSize > 0) {
        return true;
    } else {
        return false;
    }
}

// float WorldToRadar(float location) {
//     // 1.004
//     float Scale = 1;
//     if (MapName == "Baltic_Main") {
//         Scale = 0.99609375f;
//     }
//     return location / gameData.Radar.ImageMapSize * mapSprite.getLocalBounds().height *
//            (gameData.Radar.ImageMapSize == 816000 ? Scale : 1);
// }

bool Radar::GetMiniMap() {
    uint64_t minMapPtr = VmmCore::ReadValue<uint64_t>(gameData.MyHUD + Offset::WidgetStateMap);
    int minMapInt = VmmCore::ReadValue<int>(gameData.MyHUD + Offset::WidgetStateMap + 0x8);
    for (int i = 0; i < minMapInt; i++) {
        uint64_t minMapData = VmmCore::ReadValue<uint64_t>(minMapPtr + (i * 0x8));
        int ID = Decrypt::CIndex(VmmCore::ReadValue<int>(minMapData + Offset::ObjID));
        std::string WidgetName = GNames::GetNameByID(ID);

        if (WidgetName == "MinimapOriginalType_C") {
            gameData.Radar.MiniMapWidget = minMapData;
            return true;
        }
    }

    return false;
}

bool Radar::GetMapGrid() {
    uint64_t BlockInputWidgetList = VmmCore::ReadValue<uint64_t>(gameData.MyHUD + Offset::BlockInputWidgetList);
    int BlockInputWidgetListCount = VmmCore::ReadValue<int>(gameData.MyHUD + Offset::BlockInputWidgetList + 0x8);
    for (int i = 0; i < BlockInputWidgetListCount; i++) {
        uint64_t Widget = VmmCore::ReadValue<uint64_t>(BlockInputWidgetList + i * 0x8);
        int ID = Decrypt::CIndex(VmmCore::ReadValue<int>(Widget + Offset::ObjID));
        std::string WidgetName = GNames::GetNameByID(ID);
        if (WidgetName == "NewWorldMapWidget_BP_C") {
            gameData.Map.MapGrid = VmmCore::ReadValue<uint64_t>(Widget + Offset::TrainingMapGrid + 0x30);
            gameData.Map.MapWidget = Widget;
            return true;
        }
        if (WidgetName == "UI_TrainingWorldMapWidget_C") {
            gameData.Map.MapGrid = VmmCore::ReadValue<uint64_t>(Widget + Offset::TrainingMapGrid);
            gameData.Map.MapWidget = Widget;
            return true;
        }
    }

    return false;
}

bool Radar::GetVisibility() {
    auto Visibility = VmmCore::ReadValue<ESlateVisibility>(gameData.Map.MapWidget + Offset::Visibility) ==
                      ESlateVisibility::SelfHitTestInvisible;
    gameData.Map.Visibility = Visibility;
    return gameData.Map.Visibility;
}

float Radar::GetZoomFactor() {
    gameData.Map.Slot = VmmCore::ReadValue<DWORD_PTR>(gameData.Map.MapGrid + Offset::Slot);
    if (Utils::ValidPtr(gameData.Map.Slot)) return 0.0f;
    gameData.Map.Layout = VmmCore::ReadValue<FMargin>(gameData.Map.Slot + Offset::LayoutData + 0x0);
    const FVector2D CurrentSize = {gameData.Map.Layout.Right, gameData.Map.Layout.Bottom};
    auto factor = CurrentSize.X / DefaultSize.X;
    gameData.Map.MapZoomValue = factor;
    return factor;
};

Vector3 Radar::PlayerToRadarPosition(Vector3 position) {
    // Vector3 value;
    auto value = position + gameData.Map.WorldOriginLocation;
    // value = {position.x + woeldLocation.x, position.y + woeldLocation.y, position.z + woeldLocation.z};
    if (value.x < 0) value = {-value.x, value.y, value.z};
    if (value.y < 0) value = {value.x, -value.y, value.z};
    if (value.z < 0) value = {value.x, value.y, -value.z};
    return value;
}

FVector2D Radar::GetPosition() {
    // 鼠标坐标
    auto Alignment = VmmCore::ReadValue<FVector2D>(gameData.Map.Slot + Offset::LayoutData + 0x0 + Offset::Alignment);
    const FVector2D CurrentPos = {gameData.Map.Layout.Right * (Alignment.X - 0.5f) - gameData.Map.Layout.Left,
                                  gameData.Map.Layout.Bottom * (Alignment.Y - 0.5f) - gameData.Map.Layout.Top};
    gameData.Map.Position = {CurrentPos.X / DefaultSize.X / gameData.Map.MapZoomValue * 2.0f,
                             CurrentPos.Y / DefaultSize.Y / gameData.Map.MapZoomValue * 2.0f};
    return gameData.Map.Position;
}

void Radar::Update() {
    while (true) {
        if (gameData.Scene != Scene::Gameing) continue;

        if (gameData.Map.MapSize <= 0) {
            if (!Radar::GetMap()) {
                Sleep(10);
                continue;
            }
        }

        if (Utils::ValidPtr(gameData.Map.MapGrid)) {
            if (!Radar::GetMapGrid()) {
            }
        }
        // if (Utils::ValidPtr(gameData.Radar.MiniMapWidget)) {
        //     if (!Radar::GetMiniMap()) {
        //     }
        // }
        if (!gameData.Map.Visibility) {
            if (!Radar::GetVisibility()) {
            }
        }

        Radar::GetZoomFactor();
        Radar::GetPosition();

        const float MapSizeFactored = gameData.Map.MapSize / gameData.Map.MapZoomValue;
        const Vector3 WorldCenterLocation = {gameData.Map.MapSize * (1.0f + gameData.Map.Position.X),
                                             gameData.Map.MapSize * (1.0f + gameData.Map.Position.Y), 0.0f};

        const auto players = Data::GetPlayers();
        for (auto player : players) {
            const Vector3 WorldLocation = PlayerToRadarPosition(player.Location);
            const Vector3 RadarPos = WorldLocation - WorldCenterLocation;
            const FVector2D RadarScreenPos =
                gameData.Map.ScreenCenter + FVector2D{RadarPos.x / MapSizeFactored * gameData.Map.ScreenCenter.Y,
                                                      RadarPos.y / MapSizeFactored * gameData.Map.ScreenCenter.Y};

            gameData.Map.Players[player.Entity] = {RadarScreenPos};
            Vector3 minRadarPos = {RadarScreenPos.X / gameData.Map.Layout.Right,
                                   RadarScreenPos.Y / gameData.Map.Layout.Bottom, WorldLocation.z};
            Vector3 pos = {WorldLocation.x / (gameData.Map.MapSize * 2), WorldLocation.y / (gameData.Map.MapSize * 2),
                           WorldLocation.z};
            gameData.Radar.Players[player.Entity] = pos;
        }
    }
}