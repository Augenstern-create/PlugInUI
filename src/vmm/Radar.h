#pragma once

#include "SrcGame.h"
#include "VmmCore.h"
#include "Decrypt.h"

class Radar {
   public:
    static bool GetMap();
    static bool GetMapGrid();
    static bool GetMiniMap();
    static bool GetVisibility(DWORD_PTR map_radar);
    static float GetZoomFactor(FVector2D default_size, DWORD_PTR map_grid, DWORD_PTR* map_address, FMargin* declare);
    static FVector2D GetPosition(DWORD_PTR map_address, FMargin declare, FVector2D default_size, float zoom_value);
    static Vector3 PlayerToRadarPosition(Vector3 position);
    static void Update();
};