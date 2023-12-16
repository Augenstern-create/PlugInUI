#pragma once

#include "Include.h"

class GNames {
   public:
    static std::string GetNameByID(int ID);
    static DWORD_PTR GetGNamesPtr();
    static void GetGNameLists();
};