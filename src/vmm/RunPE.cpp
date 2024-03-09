#include "RunPE.h"

void RunPE() {
    // 创建进程
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // 启动一个新的进程
    if (!CreateProcess(NULL,  // 没有可执行文件路径，创建一个空的进程
                       NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        std::cerr << "CreateProcess failed. Error code: " << GetLastError() << std::endl;
        return;
    }

    std::cout << "Empty process created successfully." << std::endl;

    Sleep(2000);

    // 在傀儡进程中写入数据
    const char data[] = "Hello, puppet process!";
    SIZE_T bytesWritten;
    if (!WriteProcessMemory(pi.hProcess, (LPVOID)0x400000, data, sizeof(data), &bytesWritten)) {
        std::cerr << "WriteProcessMemory failed. Error code: " << GetLastError() << std::endl;
    } else {
        std::cout << "Data written to puppet process successfully." << std::endl;
    }

    // 等待子进程结束
    WaitForSingleObject(pi.hProcess, INFINITE);

    // 关闭句柄
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return;
}