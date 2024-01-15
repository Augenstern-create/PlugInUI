#pragma once
#include "imgui.h"
#include <vector>
#include <iostream>
#include <list>
#include <string>
#include <tuple>

#ifndef IMGUI_DISABLE

namespace ImGui {
IMGUI_API void ShowDebugWindow(bool* p_open, ImVec2 display_size);
}  // namespace ImGui
#endif  // IMGUI_DISABLE