#pragma once

#include <unordered_map>
#include <string>

#include "Constant.h"
#include <algorithm>

struct EntityInfo {
    std::string DisplayName;
    EntityType Type;
    int ID;
};

EntityInfo findEntityInfoByID(int id);
std::string findNameByID(int id);

extern std::unordered_map<std::string, EntityInfo> EntityLists;

namespace Weapon {
struct WeaponInfo {
    std::string WeaponName;
    int Width;
    int Height;
    unsigned int TextureID;
    int ID;
};
struct FindWeaponByName {
    std::string weaponName;
    int weaponID;
    FindWeaponByName(const std::string& name) : weaponName(name), weaponID(0) {}
    FindWeaponByName(int id) : weaponID(id) {}
    // bool operator()(const WeaponInfo& weapon) const { return weapon.WeaponName == weaponName; }
    // bool operator()(const WeaponInfo& weapon) const { return weapon.ID == weaponID; }
    bool operator()(const WeaponInfo& weapon) const {
        if (!weaponName.empty()) {
            return weapon.WeaponName == weaponName;
        } else {
            return weapon.ID == weaponID;
        }
    }
};

extern void LoadTexture(std::string Path);
extern unsigned int GetTextureID(std::string Name, int* width, int* height);
extern unsigned int GetTextureID(int ID, int* width, int* height);
extern std::vector<WeaponInfo> WeaponTextureLists;
extern void LOGDebug();

}  // namespace Weapon