#include "VmmCore.h"
#include "leechcore.h"

static VMM_HANDLE hVMM;
static DWORD pid;
static std::vector<VMMDLL_SCATTER_HANDLE> scatterhandles = {};
static ULONG64 gafAsyncKeyState;
static BYTE keyStateBitmap[50];

struct DProcess {
    DWORD dwPPID;
    DWORD dwPID;
};

BOOL VmmCore::Initialize() {
    LPSTR args[] = {(LPSTR) "-norefresh", (LPSTR) "-device", (LPSTR) "fpga", (LPSTR) "-printf", (LPSTR) "-v"};
    DWORD argc = sizeof(args) / sizeof(args[0]);
    hVMM = VMMDLL_Initialize(argc, args);
    if (hVMM == 0) {
        return false;
    } else {
        return true;
    }
}

BOOL VmmCore::Refresh() {
    VMMDLL_ConfigSet(hVMM, VMMDLL_OPT_REFRESH_FREQ_TLB_PARTIAL, 1);
    return true;
}

BOOL VmmCore::RefreshFast() {
    VMMDLL_ConfigSet(hVMM, VMMDLL_OPT_REFRESH_FREQ_MEM, 1);
    VMMDLL_ConfigSet(hVMM, VMMDLL_OPT_REFRESH_FREQ_FAST, 1);
    return true;
}

BOOL VmmCore::RefreshAll() {
    VMMDLL_ConfigSet(hVMM, VMMDLL_OPT_REFRESH_ALL, 1);
    return true;
}

DWORD_PTR VmmCore::ProcessGetModuleBase(LPSTR name) { return VMMDLL_ProcessGetModuleBaseU(hVMM, pid, name); }

DWORD VmmCore::GetGamePID() {
    PDWORD pdwPIDs = NULL;
    ULONG64 cPIDs = 0;
    QWORD i;

    VMMDLL_PidList(hVMM, NULL, &cPIDs);
    pdwPIDs = (PDWORD)LocalAlloc(LMEM_ZEROINIT, cPIDs * sizeof(DWORD));
    VMMDLL_PidList(hVMM, pdwPIDs, &cPIDs);

    std::vector<DProcess> pids = {};

    for (i = 0; i < cPIDs; i++) {
        VMMDLL_PROCESS_INFORMATION info;
        SIZE_T cbInfo = sizeof(VMMDLL_PROCESS_INFORMATION);
        ZeroMemory(&info, cbInfo);
        info.magic = VMMDLL_PROCESS_INFORMATION_MAGIC;
        info.wVersion = VMMDLL_PROCESS_INFORMATION_VERSION;

        if (VMMDLL_ProcessGetInformation(hVMM, pdwPIDs[i], &info, &cbInfo)) {
            if (strcmp(info.szNameLong, "TslGame.exe") == 0) {
                pids.push_back({info.dwPPID, info.dwPID});
            }
        }

        if (pids.size() == 2) {
            break;
        }
    }

    if (pids.size() != 2) {
        return 0;
    }

    for (const auto& process : pids) {
        for (const auto& potentialChild : pids) {
            if (process.dwPID == potentialChild.dwPPID && potentialChild.dwPPID != 0) {
                pid = potentialChild.dwPID;
                break;
            }
        }
    }

    return pid;
}

BOOL VmmCore::InitScatterHandles() {
    for (auto hs : scatterhandles) {
        VMMDLL_Scatter_Clear(hs, pid, VMMDLL_FLAG_NOCACHE | VMMDLL_FLAG_NOPAGING_IO);
        VMMDLL_Scatter_CloseHandle(hs);
    }

    scatterhandles = {};

    scatterhandles.push_back({VMMDLL_Scatter_Initialize(hVMM, pid, VMMDLL_FLAG_NOCACHE | VMMDLL_FLAG_NOPAGING_IO)});
    scatterhandles.push_back({VMMDLL_Scatter_Initialize(hVMM, pid, VMMDLL_FLAG_NOCACHE | VMMDLL_FLAG_NOPAGING_IO)});
    scatterhandles.push_back({VMMDLL_Scatter_Initialize(hVMM, pid, VMMDLL_FLAG_NOCACHE | VMMDLL_FLAG_NOPAGING_IO)});
    scatterhandles.push_back({VMMDLL_Scatter_Initialize(hVMM, pid, VMMDLL_FLAG_NOCACHE | VMMDLL_FLAG_NOPAGING_IO)});

    return scatterhandles.size() > 0;
}

int VmmCore::GetScatterHandleSize() { return scatterhandles.size(); }

VMMDLL_SCATTER_HANDLE VmmCore::GetScatterHandle(int index) { return scatterhandles.at(index); }

ULONG64 VmmCore::GetGafAsyncKeyState() { return gafAsyncKeyState; }

BYTE VmmCore::GetKeyStateBitmap() { return keyStateBitmap[50]; }

DWORD VmmCore::GetCsrssPID() {
    DWORD csrssPid = 0;
    DWORD cProcessInformation = 0;

    PVMMDLL_PROCESS_INFORMATION pProcessInformationEntry, pProcessInformationAll = NULL;
    bool result = VMMDLL_ProcessGetInformationAll(hVMM, &pProcessInformationAll, &cProcessInformation);

    if (result) {
        for (unsigned int i = 0; i < cProcessInformation; i++) {
            pProcessInformationEntry = &pProcessInformationAll[i];
            if (strcmp(pProcessInformationEntry->szName, "csrss.exe") == 0) {
                gafAsyncKeyState = VMMDLL_ProcessGetProcAddressU(hVMM, pProcessInformationEntry->dwPID, (LPSTR) "win32kbase.sys", (LPSTR) "gafAsyncKeyState");
                if (gafAsyncKeyState) {
                    result = VMMDLL_MemReadEx(hVMM, pProcessInformationEntry->dwPID, gafAsyncKeyState, (PBYTE)&keyStateBitmap, sizeof(keyStateBitmap), NULL,
                                              VMMDLL_FLAG_NOCACHE);
                    if (keyStateBitmap[0x24] == '\x2') {
                        csrssPid = pProcessInformationEntry->dwPID;
                        break;
                    }
                }
            }
        }
        VMMDLL_MemFree(pProcessInformationAll);
    }
    return csrssPid;
}

BOOL VmmCore::ReadByte(DWORD_PTR address, PVOID buffer, DWORD_PTR length) {
    return VMMDLL_MemReadEx(hVMM, (DWORD)(QWORD)pid, address, (PBYTE)buffer, (DWORD)length, 0, VMMDLL_FLAG_NOCACHE | VMMDLL_FLAG_NOPAGING_IO);
}

BOOL VmmCore::ReadProcessByte(DWORD pid, DWORD_PTR address, PVOID buffer, DWORD_PTR length) {
    return VMMDLL_MemReadEx(hVMM, (DWORD)(QWORD)pid, address, (PBYTE)buffer, (DWORD)length, 0, VMMDLL_FLAG_NOCACHE | VMMDLL_FLAG_NOPAGING_IO);
}

void VmmCore::ScatterPrepare(VMMDLL_SCATTER_HANDLE hs, DWORD_PTR address, DWORD_PTR length) { VMMDLL_Scatter_Prepare(hs, address, length); }

void VmmCore::ScatterExecuteRead(VMMDLL_SCATTER_HANDLE hs) { VMMDLL_Scatter_ExecuteRead(hs); }

void VmmCore::ScatterReadByte(VMMDLL_SCATTER_HANDLE hs, DWORD_PTR address, PVOID buffer, DWORD_PTR length) {
    VMMDLL_Scatter_Read(hs, address, (DWORD)length, (PBYTE)buffer, NULL);
}

void VmmCore::ScatterClear(VMMDLL_SCATTER_HANDLE hs) { VMMDLL_Scatter_Clear(hs, pid, VMMDLL_FLAG_NOCACHE | VMMDLL_FLAG_NOPAGING_IO); }

void VmmCore::Clear() {
    for (auto hs : scatterhandles) {
        VMMDLL_Scatter_Clear(hs, pid, VMMDLL_FLAG_NOCACHE | VMMDLL_FLAG_NOPAGING_IO);
    }
}

template <typename T>
BOOL VmmCore::ReadScatterBytes(std::vector<DWORD_PTR>& AddrVector, _Out_ std::vector<T>& Array) {
    VMMDLL_SCATTER_HANDLE hScatter = NULL;
    hScatter = VMMDLL_Scatter_Initialize(hVMM, pid, VMMDLL_FLAG_NOCACHE | VMMDLL_FLAG_NOPAGING_IO);

    for (int i = 0; i < AddrVector.size(); i++) {
        VMMDLL_Scatter_Prepare(hScatter, AddrVector.at(i), sizeof(T));
    }

    VMMDLL_Scatter_ExecuteRead(hScatter);

    for (int i = 0; i < AddrVector.size(); i++) {
        T temp;
        VMMDLL_Scatter_Read(hScatter, AddrVector.at(i), sizeof(T), (PBYTE)&temp, NULL);
        Array.push_back(temp);
    }

    VMMDLL_Scatter_Clear(hScatter, pid, NULL);
    VMMDLL_Scatter_CloseHandle(hScatter);
    return true;
}

template BOOL VmmCore::ReadScatterBytes(std::vector<DWORD_PTR>& AddrVector, _Out_ std::vector<int>& Array);
template BOOL VmmCore::ReadScatterBytes(std::vector<DWORD_PTR>& AddrVector, _Out_ std::vector<Vector3>& Array);
template BOOL VmmCore::ReadScatterBytes(std::vector<DWORD_PTR>& AddrVector, _Out_ std::vector<FTransform>& Array);
template BOOL VmmCore::ReadScatterBytes(std::vector<DWORD_PTR>& AddrVector, _Out_ std::vector<float>& Array);
template BOOL VmmCore::ReadScatterBytes(std::vector<DWORD_PTR>& AddrVector, _Out_ std::vector<FText>& Array);
template BOOL VmmCore::ReadScatterBytes(std::vector<DWORD_PTR>& AddrVector, _Out_ std::vector<DWORD_PTR>& Array);

template <typename T>
BOOL VmmCore::ReadScatterBytes(VMMDLL_SCATTER_HANDLE Handle, std::vector<DWORD_PTR>& AddrVector, _Out_ std::vector<T>& Array, DWORD_PTR length) {
    for (int i = 0; i < AddrVector.size(); i++) {
        if (length) {
            VMMDLL_Scatter_Prepare(Handle, AddrVector.at(i), length);
        } else {
            VMMDLL_Scatter_Prepare(Handle, AddrVector.at(i), sizeof(T));
        }
    }

    VMMDLL_Scatter_ExecuteRead(Handle);

    for (int i = 0; i < AddrVector.size(); i++) {
        T temp;
        VMMDLL_Scatter_Read(Handle, AddrVector.at(i), sizeof(T), (PBYTE)&temp, NULL);
        Array.push_back(temp);
    }

    VMMDLL_Scatter_Clear(Handle, pid, VMMDLL_FLAG_NOCACHE | VMMDLL_FLAG_NOPAGING_IO);

    return true;
}

template BOOL VmmCore::ReadScatterBytes(VMMDLL_SCATTER_HANDLE Handle, std::vector<DWORD_PTR>& AddrVector, _Out_ std::vector<int>& Array, DWORD_PTR length);
template BOOL VmmCore::ReadScatterBytes(VMMDLL_SCATTER_HANDLE Handle, std::vector<DWORD_PTR>& AddrVector, _Out_ std::vector<Vector3>& Array, DWORD_PTR length);
template BOOL VmmCore::ReadScatterBytes(VMMDLL_SCATTER_HANDLE Handle, std::vector<DWORD_PTR>& AddrVector, _Out_ std::vector<FTransform>& Array,
                                        DWORD_PTR length);
template BOOL VmmCore::ReadScatterBytes(VMMDLL_SCATTER_HANDLE Handle, std::vector<DWORD_PTR>& AddrVector, _Out_ std::vector<float>& Array, DWORD_PTR length);
template BOOL VmmCore::ReadScatterBytes(VMMDLL_SCATTER_HANDLE Handle, std::vector<DWORD_PTR>& AddrVector, _Out_ std::vector<FText>& Array, DWORD_PTR length);
template BOOL VmmCore::ReadScatterBytes(VMMDLL_SCATTER_HANDLE Handle, std::vector<DWORD_PTR>& AddrVector, _Out_ std::vector<DWORD_PTR>& Array,
                                        DWORD_PTR length);
template BOOL VmmCore::ReadScatterBytes(VMMDLL_SCATTER_HANDLE Handle, std::vector<DWORD_PTR>& AddrVector, _Out_ std::vector<FRotator>& Array, DWORD_PTR length);

template <typename T>
T VmmCore::BatchRead(std::vector<DWORD_PTR> ptr, uint32_t offset, int handleIndex, DWORD_PTR length) {
    std::vector<DWORD_PTR> addresses;
    T results;

    for (int i = 0; i < ptr.size(); ++i) {
        addresses.push_back(ptr[i] + offset);
    }

    results = VmmCore::ReadScatterValues<T>(VmmCore::GetScatterHandle(handleIndex), addresses, length);
    return results;
}

template std::vector<DWORD_PTR> VmmCore::BatchRead(std::vector<DWORD_PTR> ptr, uint32_t offset, int handleIndex, DWORD_PTR length);
template std::vector<int> VmmCore::BatchRead(std::vector<DWORD_PTR> ptr, uint32_t offset, int handleIndex, DWORD_PTR length);
template std::vector<Vector3> VmmCore::BatchRead(std::vector<DWORD_PTR> ptr, uint32_t offset, int handleIndex, DWORD_PTR length);
template std::vector<float> VmmCore::BatchRead(std::vector<DWORD_PTR> ptr, uint32_t offset, int handleIndex, DWORD_PTR length);
template std::vector<ECharacterState> VmmCore::BatchRead(std::vector<DWORD_PTR> ptr, uint32_t offset, int handleIndex, DWORD_PTR length);
template std::vector<FRotator> VmmCore::BatchRead(std::vector<DWORD_PTR> ptr, uint32_t offset, int handleIndex, DWORD_PTR length);
