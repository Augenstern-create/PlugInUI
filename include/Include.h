#pragma once
#include <windows.h>
#include <string.h>
#include <iostream>
#include <cstdint>
#include <vector>
#include <chrono>
#include <cstdlib>  // 包含 std::system 函数的头文件
#include "version.h"
#include <time.h>
#include <tchar.h>
#include <filesystem>
#include <list>
#include <string>
#include <tuple>

#define TAG "PLuiinUI"

#define LOGFILE(level, fmt, tag, ...) printf(("\n%s/%s:" fmt), level, tag, ##__VA_ARGS__);

#define LOGDL(fmt, tag, ...) LOGFILE("D", fmt, tag, ##__VA_ARGS__)

#define LOGIL(fmt, tag, ...) LOGFILE("I", fmt, tag, ##__VA_ARGS__)
#define LOGEL(fmt, tag, ...) LOGFILE("E", fmt, tag, ##__VA_ARGS__)

//---start define project using macro.=====================
#define LSO_LOGD(fmt, ...) LOGDL(fmt, TAG, ##__VA_ARGS__);
#define LSO_LOGI(fmt, ...) LOGIL(fmt, TAG, ##__VA_ARGS__);
#define LSO_LOGE(fmt, ...) LOGEL(fmt, TAG, ##__VA_ARGS__);

#define LOGE LSO_LOGE
#define LOGI LSO_LOGI
#define LOGD LSO_LOGD
