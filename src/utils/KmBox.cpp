#include "KmBox.h"
#include <iostream>
#include <string>
#include <algorithm>

i_KM KmBox::km;
std::mutex KmBox::mutex_;

BOOL KmBox::Init(const char* Name) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (km.IsOpen()) {
        km.Close();
    }
    BOOL ret = km.OpenDevice(Name);
    if (!ret) {
        std::cout << "MagicBox Device Failure!" << std::endl;
        return FALSE;
    }
    std::cout << "-------------------------" << std::endl;
    std::cout << "MagicBox Connection successful!" << std::endl;
    std::cout << "model number" << km.GetModel() + 'A' - 1 << std::endl;
    std::cout << "Firmware version" << km.GetVersion() << std::endl;
    std::cout << "product key " << km.GetChipID() << std::endl;
    std::cout << "-------------------------" << std::endl;

    return TRUE;
}

void KmBox::Close() {
    std::lock_guard<std::mutex> lock(mutex_);
    km.Close();
}

BOOL KmBox::Move(float x, float y) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (!km.IsOpen()) {
        std::cout << "MagicBox Device Failure!" << std::endl;
        return FALSE;
    }
    km.MoveRD((short)x, (short)y, 2, 10);

    return TRUE;
}