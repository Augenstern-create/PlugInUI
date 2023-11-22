#pragma once
#include "imgui.h"
#include <vector>
#include <iostream>
#include <list>
#include <string>

#ifndef IMGUI_DISABLE

namespace ImGui {

IMGUI_API void ShowMenuWindow(bool* p_open = NULL);
IMGUI_API void ShowExampleAppMenu();
IMGUI_API void ShowExampleAppPlayerInstall();
IMGUI_API void ShowExampleAppAlternateInstall();
IMGUI_API void ShowExampleAppMenuInitializelist();
IMGUI_API bool CustomizedButton(const char* label, ImVec2& size, ImU32 borderColor, ImU32 textColor);

}  // namespace ImGui

#endif  // #ifndef IMGUI_DISABLE
