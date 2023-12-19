#pragma once
#include "imgui.h"
#include <vector>
#include <iostream>
#include <list>
#include <string>
#include <tuple>
#include <d3d11.h>

#ifndef IMGUI_DISABLE

namespace ImGui {
IMGUI_API void ShowRadarWindow(bool* p_open, ImVec2 display_size);
IMGUI_API void ShowRadarWindow2(bool* p_open);

}  // namespace ImGui
#endif  // IMGUI_DISABLE