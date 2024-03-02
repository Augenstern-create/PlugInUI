#include "Radar.h"
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

bool Radar::GetMiniMap() {
    DWORD_PTR minMapPtr = VmmCore::ReadValue<DWORD_PTR>(gameData.MyHUD + Offset::WidgetStateMap);
    int minMapInt = VmmCore::ReadValue<int>(gameData.MyHUD + Offset::WidgetStateMap + 0x8);
    for (int i = 0; i < 500; i++) {
        DWORD_PTR minMapData = VmmCore::ReadValue<DWORD_PTR>(minMapPtr + (i * 0x8));
        int ID = Decrypt::CIndex(VmmCore::ReadValue<int>(minMapData + Offset::ObjID));
        std::string WidgetName = GNames::GetNameByID(ID);
        if (WidgetName == "MinimapOriginalType_C") {
            gameData.mapRadar.min_map.map_radar = minMapData;
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
            gameData.mapRadar.max_map.map_grid = VmmCore::ReadValue<uint64_t>(Widget + Offset::TrainingMapGrid + 0x30);
            gameData.mapRadar.max_map.map_radar = Widget;
            return true;
        }
        if (WidgetName == "UI_TrainingWorldMapWidget_C") {
            gameData.mapRadar.max_map.map_grid = VmmCore::ReadValue<uint64_t>(Widget + Offset::TrainingMapGrid);
            gameData.mapRadar.max_map.map_radar = Widget;
            return true;
        }
    }

    return false;
}

bool Radar::GetVisibility(DWORD_PTR map_radar) {
    auto Visibility = VmmCore::ReadValue<ESlateVisibility>(map_radar + Offset::Visibility) == ESlateVisibility::SelfHitTestInvisible;
    return Visibility;
}

float Radar::GetZoomFactor(FVector2D default_size, DWORD_PTR map_grid, DWORD_PTR* map_address, FMargin* declare) {
    *map_address = VmmCore::ReadValue<DWORD_PTR>(map_grid + Offset::Slot);
    if (Utils::ValidPtr(*map_address)) return 0.0f;
    *declare = VmmCore::ReadValue<FMargin>(*map_address + Offset::LayoutData + 0x0);
    const FVector2D CurrentSize = {declare->Right, declare->Bottom};
    auto factor = CurrentSize.X / default_size.X;
    return factor;
}

FVector2D Radar::GetPosition(DWORD_PTR map_address, FMargin declare, FVector2D default_size, float zoom_value) {
    auto Alignment = VmmCore::ReadValue<FVector2D>(map_address + Offset::LayoutData + 0x0 + Offset::Alignment);
    const FVector2D CurrentPos = {declare.Right * (Alignment.X - 0.5f) - declare.Left, declare.Bottom * (Alignment.Y - 0.5f) - declare.Top};
    const FVector2D position = {CurrentPos.X / default_size.X / zoom_value * 2.0f, CurrentPos.Y / default_size.Y / zoom_value * 2.0f};
    return position;
}

void Radar::Update() {
    while (true) {
        if (gameData.Scene != Scene::Gameing) continue;
        FVector2D DefaultSize = {1080.0f, 1080.0f};
        // 大地图雷达
        {
            bool is_ibility;
            float map_zoom_value;
            DWORD_PTR map_address;
            FMargin declare;
            FVector2D position;
            is_ibility = Radar::GetVisibility(gameData.mapRadar.max_map.map_radar);
            map_zoom_value = Radar::GetZoomFactor(DefaultSize, gameData.mapRadar.max_map.map_grid, &map_address, &declare);
            position = Radar::GetPosition(map_address, declare, DefaultSize, map_zoom_value);

            auto player_location = gameData.Myself.Location;
            auto world_location = gameData.mapRadar.world_location;
            auto map_size = gameData.mapRadar.map_size;
            auto zoom_value = map_size / map_zoom_value;

            Vector3 player_to_world = {player_location.x + world_location.x, player_location.y + world_location.y, player_location.z};
            Vector3 world_center = {map_size * (1 + position.x), map_size * (1 + position.y), 0.0f};
            auto radar_location = player_to_world - world_center;

            gameData.mapRadar.max_map.map_address = map_address;
            gameData.mapRadar.max_map.is_ibility = is_ibility;
            gameData.mapRadar.max_map.map_zoom_value = map_zoom_value;
            gameData.mapRadar.max_map.declare = declare;
            gameData.mapRadar.max_map.position = position;
        }
    }
}