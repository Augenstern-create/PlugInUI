#pragma once

#include <string>
#include <Windows.h>
#include <ShlObj.h>
#include <iostream>
#include <ctime>
#include <sstream>
#include <codecvt>
#include <string>
#include "INIReader.h"

#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_RESET "\033[0m"
#define COLOR_LYELLOW "\033[1;33m"
#define COLOR_LGREEN "\033[1;32m"
// 浅蓝色
#define COLOR_LBLUE "\033[1;34m"

template <typename... T>
static constexpr auto make_array(T&&... t) -> std::array<std::decay_t<std::common_type_t<T...>>, sizeof...(T)> {
    return {{std::forward<T>(t)...}};
}

namespace Utils {
inline std::wstring getSystemFontsFolderPath() {
    wchar_t* path = nullptr;
    HRESULT hr = SHGetKnownFolderPath(FOLDERID_Fonts, 0, NULL, &path);
    if (SUCCEEDED(hr)) {
        std::wstring fontsFolderPath(path);
        CoTaskMemFree(path);
        return fontsFolderPath;
    }
    return L"";
}

inline std::wstring StringToWstring(std::string wstr) {
    std::wstring res;
    int len = MultiByteToWideChar(CP_ACP, 0, wstr.c_str(), (int)wstr.size(), nullptr, 0);
    if (len < 0) {
        return res;
    }
    wchar_t* buffer = new wchar_t[len + 1];
    if (buffer == nullptr) {
        return res;
    }
    MultiByteToWideChar(CP_ACP, 0, wstr.c_str(), (int)wstr.size(), buffer, len);
    buffer[len] = '\0';
    res.append(buffer);
    delete[] buffer;
    return res;
}

inline bool ValidPtr(DWORD_PTR Ptr) { return (bool)(Ptr < 0x1000000 || Ptr > 0x7FFFFFF00000 || Ptr % sizeof(DWORD_PTR)); }

inline bool ValidPtr(PVOID Ptr) {
    auto v1 = (ULONG64)Ptr;

    return (bool)(v1 < 0x1000000 || v1 > 0x7FFFFFF00000 || v1 % sizeof(DWORD_PTR));
}

inline std::string getCurrentTime() {
    time_t currentTime;
    struct tm timeinfo;

    // 获取当前时间
    time(&currentTime);

    // 使用localtime_s函数代替localtime
    localtime_s(&timeinfo, &currentTime);

    // 将时间转换为time_t类型
    time_t midnight = mktime(&timeinfo);

    // 将时间转换为字符串格式
    char timeStr[9];
    strftime(timeStr, sizeof(timeStr), "%T", &timeinfo);

    return timeStr;
}

inline char* UnicodeToAnsi(char* wstr) {
    if (!wstr) {
        return NULL;
    }
    int strleng = WideCharToMultiByte(CP_ACP, NULL, (LPCWCH)wstr, (int)wcslen((LPCWCH)wstr), NULL, 0, NULL, FALSE);
    char* str = new char[strleng + 1];
    WideCharToMultiByte(CP_ACP, NULL, (LPCWCH)wstr, (int)wcslen((LPCWCH)wstr), str, strleng, NULL, FALSE);
    str[strleng] = '\0';
    return str;
}

inline void Log(const int& type, const char* format, ...) {
    std::ostringstream oss;

    oss << COLOR_LYELLOW << "[" << getCurrentTime() << "]";

    if (type == 1) {
        oss << COLOR_LGREEN << "[SUCCESS] ";
    } else if (type == 2) {
        oss << COLOR_RED << "[ERROR] ";
    } else {
        oss << COLOR_LBLUE << "[INFO] ";
    }

    va_list args;
    va_start(args, format);
    char buffer[256];
    std::vsnprintf(buffer, sizeof(buffer), format, args);

    va_end(args);

    oss << buffer;
    oss << COLOR_RESET;

    std::cout << oss.str() << std::endl;
}

inline std::string AnalyzeClanName(std::string Name) {
    std::string ClanName = "";
    size_t startPos = Name.find("[");
    if (startPos != std::string::npos) {
        size_t endPos = Name.find("]", startPos);
        if (endPos != std::string::npos) {
            ClanName = Name.substr(startPos, endPos - startPos + 1);
        }
    }
    return ClanName;
}
inline std::string AnalyzeName(std::string Name) {
    std::string ClanName = Name;
    size_t startPos = Name.find("]");
    if (startPos != std::string::npos) {
        ClanName = Name.substr(startPos + 1);
    }
    return ClanName;
}

}  // namespace Utils
