#pragma once

#include "Include.h"
#include "SrcGame.h"
#include "SrcUe4Math.h"
#include "vmmdll.h"

class VmmCore {
   public:
    template <typename T>
    static BOOL ReadScatterBytes(std::vector<DWORD_PTR>& AddrVector, _Out_ std::vector<T>& Array);

    template <typename T>
    static BOOL ReadScatterBytes(VMMDLL_SCATTER_HANDLE Handle, std::vector<DWORD_PTR>& AddrVector,
                                 _Out_ std::vector<T>& Array, DWORD_PTR length = 0);

    template <typename T>
    static T ReadScatterValues(std::vector<DWORD_PTR>& addrVector) {
        T values;
        ReadScatterBytes(addrVector, values);
        return values;
    };

    template <typename T>
    static T ReadScatterValues(VMMDLL_SCATTER_HANDLE Handle, std::vector<DWORD_PTR>& addrVector) {
        T values;
        ReadScatterBytes(Handle, addrVector, values);
        return values;
    };

    template <typename T>
    static T ReadScatterValues(VMMDLL_SCATTER_HANDLE Handle, std::vector<DWORD_PTR>& addrVector, DWORD_PTR length) {
        T values;
        ReadScatterBytes(Handle, addrVector, values, length);
        return values;
    };

    template <typename T>
    static T ReadValue(DWORD_PTR addr) {
        T value;
        ReadByte(addr, &value, sizeof(T));
        return value;
    }

    template <typename T>
    static T ScatterReadValue(VMMDLL_SCATTER_HANDLE hs, DWORD_PTR addr) {
        T value;
        ScatterReadByte(hs, addr, &value, sizeof(T));
        return value;
    };

    template <typename T>
    static T BatchRead(std::vector<DWORD_PTR> ptr, uint32_t offset, int handleIndex, DWORD_PTR length = 0);

    static BOOL Initialize();
    static BOOL InitScatterHandles();
    static VMMDLL_SCATTER_HANDLE GetScatterHandle(int index);
    static DWORD_PTR ProcessGetModuleBase(LPSTR name);
    static DWORD GetGamePID();
    static DWORD GetCsrssPID();
    static ULONG64 GetGafAsyncKeyState();
    static BYTE GetKeyStateBitmap();

    static BOOL Refresh();
    static BOOL RefreshFast();
    static BOOL RefreshAll();
    static BOOL ReadByte(DWORD_PTR address, PVOID buffer, DWORD_PTR length);
    static BOOL ReadProcessByte(DWORD pid, DWORD_PTR address, PVOID buffer, DWORD_PTR length);

    static void ScatterPrepare(VMMDLL_SCATTER_HANDLE hs, DWORD_PTR address, DWORD_PTR length);
    static void ScatterExecuteRead(VMMDLL_SCATTER_HANDLE hs);
    static void ScatterReadByte(VMMDLL_SCATTER_HANDLE hs, DWORD_PTR address, PVOID buffer, DWORD_PTR length);
    static void ScatterClear(VMMDLL_SCATTER_HANDLE hs);
    static int GetScatterHandleSize();

    static void Clear();

    template <typename T>
    static BOOL ScatterReadEx(int hsIndex, uintptr_t address, T* buffer) {
        return VMMDLL_Scatter_PrepareEx(GetScatterHandle(hsIndex), address, sizeof(T), (PBYTE)buffer, NULL);
    }
    // static BOOL ScatterReadEx(int hsIndex, uintptr_t address, int* buffer) {
    //     return VMMDLL_Scatter_PrepareEx(GetScatterHandle(hsIndex), address, sizeof(int), (PBYTE)buffer, NULL);
    // }

    template <typename T>
    static void ScatterRead(VMMDLL_SCATTER_HANDLE Handle, DWORD_PTR Vector, T* temp) {
        VMMDLL_Scatter_Read(Handle, Vector, sizeof(T), (PBYTE)temp, NULL);
    }

    static BOOL ScatterExecuteReadEx(int hsIndex) {
        // BOOL result = FALSE;
        VMMDLL_SCATTER_HANDLE hs = GetScatterHandle(hsIndex);
        BOOL result = VMMDLL_Scatter_ExecuteRead(hs);
        ScatterClear(hs);
        return result;
    }
};