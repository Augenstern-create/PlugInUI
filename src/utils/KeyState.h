#pragma once

#include <Windows.h>
#include <iostream>
#include <array>
#include "SrcVmm.h"

class KeyState {
   public:
    static void Init();
    static bool WasGet(std::uint8_t const vk);
    static bool Get(std::uint8_t const vk);
};