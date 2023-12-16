#pragma once

#include "SrcGame.h"
#include "VmmCore.h"
#include "Decrypt.h"

class Radar {
   public:
    static bool GetMap();
    static bool GetMapGrid();
    static bool GetMiniMap();
    static bool GetVisibility();
    static float GetZoomFactor();
    static FVector2D GetPosition();
    static Vector3 PlayerToRadarPosition(Vector3 position);
    static void Update();
};