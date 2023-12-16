#include "KeyState.h"
#include "Utils.h"

DWORD csrssPid;
ULONG64 gafAsyncKeyState;
std::array<std::uint8_t, 256 * 2 / 8> key_state_bitmap;
std::array<std::uint8_t, 256 / 8> key_state_recent_bitmap;

bool is_key_down(std::uint8_t const vk) { return key_state_bitmap[(vk * 2 / 8)] & 1 << vk % 4 * 2; }

bool was_key_pressed(std::uint8_t const vk) {
    bool const result = key_state_recent_bitmap[vk / 8] & 1 << vk % 8;
    key_state_recent_bitmap[vk / 8] &= ~(1 << vk % 8);
    return result;
}

void update_key_state_bitmap() {
    if (!csrssPid) {
        return;
    }

    auto prev_key_state_bitmap = key_state_bitmap;
    SIZE_T size = 0;

    VmmCore::ReadProcessByte(csrssPid, gafAsyncKeyState, &key_state_bitmap,
                             sizeof(std::array<std::uint8_t, 256 * 2 / 8>));

    for (auto vk = 0u; vk < 256; ++vk) {
        if ((key_state_bitmap[(vk * 2 / 8)] & 1 << vk % 4 * 2) &&
            !(prev_key_state_bitmap[(vk * 2 / 8)] & 1 << vk % 4 * 2))
            key_state_recent_bitmap[vk / 8] |= 1 << vk % 8;
    }
}

void KeyState::Init() {
    csrssPid = VmmCore::GetCsrssPID();
    gafAsyncKeyState = VmmCore::GetGafAsyncKeyState();
    if (csrssPid) {
        Utils::Log(1, "Gaf async key state initialized successfully");
    } else {
        Utils::Log(2, "Gaf async key state initialized errors");
    }
}

bool KeyState::WasGet(std::uint8_t const vk) {
    update_key_state_bitmap();
    return was_key_pressed(vk);
}

bool KeyState::Get(std::uint8_t const vk) {
    update_key_state_bitmap();
    return is_key_down(vk);
}