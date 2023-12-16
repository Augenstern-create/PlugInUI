#pragma once

#include "Include.h"

static auto DecFunction = reinterpret_cast<DWORD_PTR (*)(DWORD_PTR key, DWORD_PTR base)>(0);

class Decrypt {
   public:
    // 解密OBJID
    static DWORD CIndex(DWORD value);
    // 解密指针
    static DWORD_PTR Xe(DWORD_PTR addr);
    // 销毁解密函数
    static void DestroyXe();
};
