#pragma once

#include "Include.h"
#include "game/Entitys.h"

class GNames {
   public:
    static std::string GetNameByID(int ID);
    static std::string GetNameByID(int id, int Handle);
    static EntityInfo GNames::GetName(int id, int Handle);
    static DWORD_PTR GetGNamesPtr();
    static void GetGNameLists();
};