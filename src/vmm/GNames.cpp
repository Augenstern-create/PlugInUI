#include "SrcGame.h"
#include "game/Entitys.h"

std::string GNames::GetNameByID(int ID) {
    UINT64 fNamePtr = VmmCore::ReadValue<UINT64>(gameData.GNames + ((int(ID / Offset::ChunkSize)) * 8));
    UINT64 fName = VmmCore::ReadValue<UINT64>(fNamePtr + ((int(ID % Offset::ChunkSize)) * 8));
    char names_c[258];
    VmmCore::ReadByte(fName + 0x10, names_c, sizeof(names_c));
    std::string names = std::string(names_c);
    if (names == "") names = "fail";
    return names;
}

DWORD_PTR GNames::GetGNamesPtr() {
    DWORD_PTR gnames = 0;
    DWORD_PTR tmp = VmmCore::ReadValue<DWORD_PTR>(gameData.GameBase + Offset::GNames);
    DWORD64 v11 = Decrypt::Xe(tmp);
    DWORD64 v21 = VmmCore::ReadValue<DWORD_PTR>(v11);
    DWORD64 v22 = Decrypt::Xe(v21);
    DWORD64 v31 = VmmCore::ReadValue<DWORD_PTR>(v22);
    DWORD64 v32 = Decrypt::Xe(v31);

    gnames = VmmCore::ReadValue<DWORD_PTR>(v32);
    gnames = Decrypt::Xe(gnames);
    tmp = gnames;
    return v22;
}

EntityInfo GNames::GetName(int id, int Handle) {
    DWORD_PTR entity;
    VmmCore::ScatterReadEx(Handle, gameData.GNames + ((int(id / Offset::ChunkSize)) * 8), (DWORD_PTR*)&entity);
    VmmCore::ScatterExecuteReadEx(Handle);
    DWORD_PTR entity2;
    VmmCore::ScatterReadEx(Handle, entity + ((int(id % Offset::ChunkSize)) * 8), (DWORD_PTR*)&entity2);
    VmmCore::ScatterExecuteReadEx(Handle);
    FText Texts;
    VmmCore::ScatterReadEx(Handle, entity2 + +0x10, (FText*)&Texts);
    VmmCore::ScatterExecuteReadEx(Handle);
    std::string name = Texts.buffer;
    EntityInfo entityInfo;
    entityInfo = findEntityInfoByName(name);
    entityInfo.ID = id;
    return entityInfo;
}

void GNames::GetGNameLists() {
    std::vector<DWORD_PTR> gNameAddresses;
    std::vector<DWORD_PTR> gNameOffsets;

    int begin = 0;

    for (int i = begin; i < 900000; ++i) {
        auto entity = gameData.GNames + ((int(i / Offset::ChunkSize)) * 8);
        gNameAddresses.push_back(entity);
    }

    gNameOffsets = VmmCore::ReadScatterValues<std::vector<DWORD_PTR>>(gNameAddresses);

    std::vector<DWORD_PTR> gNameEntryAddresses;
    std::vector<DWORD_PTR> gNameEntryOffsets;

    for (int i = 0; i < gNameOffsets.size(); ++i) {
        auto entity = gNameOffsets.at(i) + ((int(i % Offset::ChunkSize)) * 8);
        gNameEntryAddresses.push_back(entity);
    }

    gNameEntryOffsets = VmmCore::ReadScatterValues<std::vector<DWORD_PTR>>(gNameEntryAddresses);

    std::vector<DWORD_PTR> gNameTextAddresses;
    std::vector<FText> gNameTexts;

    for (int i = 0; i < gNameEntryOffsets.size(); ++i) {
        auto entity = gNameEntryOffsets.at(i) + 0x10;
        gNameTextAddresses.push_back(entity);
    }

    gNameTexts = VmmCore::ReadScatterValues<std::vector<FText>>(gNameTextAddresses);

    for (int i = 0; i < gNameTexts.size(); ++i) {
        std::string name = std::string(gNameTexts.at(i).buffer);

        if (EntityLists.find(name) != EntityLists.end()) {
            EntityLists[name].ID = i + begin;
        }
        auto it = std::find_if(Weapon::WeaponTextureLists.begin(), Weapon::WeaponTextureLists.end(), Weapon::FindWeaponByName(name));
        if (it != Weapon::WeaponTextureLists.end()) {
            it->ID = i + begin;
        }
    }
    // Weapon::LOGDebug();
}
