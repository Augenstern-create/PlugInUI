#pragma once
#include <iostream>
#include <windows.h>
#include <string>
#include <mutex>
#include "i_KM.h"

class KmBox {
   public:
    static BOOL Init(const char* Name);
    static BOOL Move(float x, float y);
    static void Close();

   private:
    static i_KM km;
    static std::mutex mutex_;
};

// i_KM KmBox::km;
// std::mutex KmBox::mutex_;