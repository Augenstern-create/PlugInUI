#pragma once

#include <unordered_map>
#include <string>

#include "Constant.h"

struct EntityInfo {
    std::string DisplayName;
    EntityType Type;
    int ID;
};

EntityInfo findEntityInfoByID(int id);

extern std::unordered_map<std::string, EntityInfo> EntityLists;