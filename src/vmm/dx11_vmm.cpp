#include "dx11_vmm.h"

#include <string.h>
#include <iostream>
#include <windows.h>

#include "vmmdll.h"

bool vmm_init() {
    system("chcp 65001");
    VMMDLL_CloseAll();
    LPSTR arguments[] = {(LPSTR) "-device", (LPSTR) "fpga", (LPSTR) "-norefresh"};
    DWORD argc = sizeof(arguments) / sizeof(arguments[0]);
    VMM_HANDLE vmmHandle = VMMDLL_Initialize(argc, arguments);

    if (vmmHandle != NULL) {
        fprintf(stderr, "Initialization successful\n");

        // 调用VMMDLL_PidList并提供必要的参数
        // DWORD* pPIDs = NULL;  // 用于存储PID的数组，可以为NULL，以获取PID的数量
        // SIZE_T pcPIDs = 0;    // 输入时是pPIDs数组的大小，输出时是实际PID的数量

        // 定义变量pcPIDs，表示系统中进程的数量
        SIZE_T pcPIDs;

        // 获取系统中所有进程的PID列表，并将进程数目保存在pcPIDs中
        VMMDLL_PidList(vmmHandle, nullptr, &pcPIDs);

        // 在堆上分配一块内存，用于存储进程PID列表
        DWORD* pPIDs = (DWORD*)new char[pcPIDs * 4];

        // 将系统中所有进程的PID列表存储到pPIDs指向的缓冲区中
        VMMDLL_PidList(vmmHandle, pPIDs, &pcPIDs);

        // 遍历所有系统进程
        for (int i = 0; i < pcPIDs; i++) {
            // 定义一个结构体ProcessInformation，用于存储进程信息
            VMMDLL_PROCESS_INFORMATION ProcessInformation = {0};

            // 设置ProcessInformation的魔数
            ProcessInformation.magic = VMMDLL_PROCESS_INFORMATION_MAGIC;

            // 设置ProcessInformation的版本号
            ProcessInformation.wVersion = VMMDLL_PROCESS_INFORMATION_VERSION;

            // 定义变量pcbProcessInformation，表示ProcessInformation结构体的大小
            SIZE_T pcbProcessInformation = sizeof(VMMDLL_PROCESS_INFORMATION);

            // 获取指定进程的信息，并将结果存储在ProcessInformation中
            VMMDLL_ProcessGetInformation(vmmHandle, pPIDs[i], &ProcessInformation, &pcbProcessInformation);

            // 定义一个指向VMMDLL_MAP_MODULEENTRY结构体的指针ppModuleMapEntry
            VMMDLL_MAP_MODULEENTRY* ppModuleMapEntry = nullptr;

            // 获取指定模块的信息，并将结果存储在ppModuleMapEntry中
            VMMDLL_Map_GetModuleFromNameU(vmmHandle, pPIDs[i], ProcessInformation.szName, &ppModuleMapEntry,
                                          VMMDLL_MODULE_FLAG_NORMAL);

            // 判断是否成功获取到模块的信息
            if (ppModuleMapEntry) {
                // 定义一个变量temp，用于存储从内存中读取的数据
                ULONG temp = 0;

                // 读取指定模块的内存页面信息
                VMMDLL_MemRead(vmmHandle, pPIDs[i], ppModuleMapEntry->vaBase, (PBYTE)&temp, 4);

                // 修改指定模块的内存页面信息
                VMMDLL_MemWrite(vmmHandle, pPIDs[i], ppModuleMapEntry->vaBase, (PBYTE)&temp, 4);

                // 输出当前进程的PID和名称
                std::cout << pPIDs[i] << "---" << ProcessInformation.szName;

                // 输出当前模块的完整路径
                std::cout << "---" << ppModuleMapEntry->uszFullName << std::endl;

                std::cout << "" << std::endl;
            }
        }
    } else {
        fprintf(stderr, "Initialization failure error \n");
    }
    return true;
}