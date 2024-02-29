#include "Decrypt.h"
#include "VmmCore.h"
#include "SrcGame.h"

DWORD Decrypt::CIndex(DWORD value) {
    DWORD xorResult = value ^ Offset::XorKey1;
    DWORD rotated = Offset::IsRor ? _rotr(xorResult, Offset::RorValue) : _rotl(xorResult, Offset::RorValue);

    return rotated ^ (rotated << 0x10) ^ Offset::XorKey2;
}

void Decrypt::DestroyXe() { DecFunction = reinterpret_cast<DWORD_PTR (*)(DWORD_PTR key, DWORD_PTR base)>(0); }

DWORD_PTR Decrypt::Xe(DWORD_PTR addr) {
    try {
        static DWORD_PTR Ptr = 0;
        if (DecFunction == nullptr) {
            int64_t DecryptPtr = 0x0;
            while (!DecryptPtr) {
                DecryptPtr = VmmCore::ReadValue<DWORD_PTR>((DWORD_PTR)gameData.GameBase + Offset::Decrypt);
                Sleep(1000);
            }

            int32_t Tmp1Add = VmmCore::ReadValue<uint32_t>(DecryptPtr + 3);
            Ptr = Tmp1Add + DecryptPtr + 7;
            unsigned char ShellcodeBuff[1024] = {NULL};
            ShellcodeBuff[0] = 0x90;
            ShellcodeBuff[1] = 0x90;
            VmmCore::ReadByte(DecryptPtr, &ShellcodeBuff[2], sizeof(ShellcodeBuff) - 2);
            ShellcodeBuff[2] = 0x48;
            ShellcodeBuff[3] = 0x8B;
            ShellcodeBuff[4] = 0xC1;
            ShellcodeBuff[5] = 0x90;
            ShellcodeBuff[6] = 0x90;
            ShellcodeBuff[7] = 0x90;
            ShellcodeBuff[8] = 0x90;
            DecFunction = reinterpret_cast<decltype(DecFunction)>(VirtualAlloc(nullptr, 4096, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE));
            RtlCopyMemory((LPVOID)DecFunction, (LPVOID)ShellcodeBuff, sizeof(ShellcodeBuff));
        }

        return DecFunction(Ptr, addr);
    } catch (const std::exception&) {
        return 0;
    }
}
