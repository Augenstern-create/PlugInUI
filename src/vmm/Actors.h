#pragma once

#include "Include.h"
#include "SrcGame.h"
#include "srcUe4math.h"
#include "vmmdll.h"
#include "VmmCore.h"
#include "Decrypt.h"

std::vector<DWORD_PTR> GetEntitys(DWORD_PTR Actor);

std::vector<int> GetEntityIDs(std::vector<DWORD_PTR> entitys);

std::vector<std::string> GetPlayerNames(std::vector<DWORD_PTR> entitys);

std::vector<float> GetPlayerHealths(std::vector<DWORD_PTR> entitys);

std::vector<float> GetPlayerGroggyHealths(std::vector<DWORD_PTR> entitys);

std::vector<int> GetPlayerTeamNumbers(std::vector<DWORD_PTR> entitys);

std::vector<DWORD_PTR> GetDroppedItems(std::vector<DWORD_PTR> ptr);

std::vector<DWORD_PTR> GetEntityRootComponents(std::vector<DWORD_PTR> entitys);
std::vector<DWORD_PTR> GetPlayerMeshComponents(std::vector<DWORD_PTR> entitys);

std::vector<Vector3> GetEntityLocations(std::vector<DWORD_PTR> rootComponents);

std::vector<DWORD_PTR> GetPlayerStates(std::vector<DWORD_PTR> ptr);

std::vector<int> GetPlayerKillCounts(std::vector<DWORD_PTR> ptr);

std::vector<int> GetPlayerPartnerLevels(std::vector<DWORD_PTR> ptr);

std::vector<DWORD_PTR> GetPlayerBones(std::vector<DWORD_PTR> ptr);

std::vector<FTransform> GetPlayerHeadBones(std::vector<DWORD_PTR> ptr);
std::vector<FTransform> GetPlayerIndexBones(std::vector<DWORD_PTR> ptr, EBoneIndex index);

std::vector<PlayerSkeletons> GetPlayerAllBones(std::vector<DWORD_PTR> ptr, std::vector<FTransform> playerComponentToWorlds);

std::vector<PlayerSkeletons> GetPlayerAllBones2(std::vector<DWORD_PTR> ptr, std::vector<FTransform> playerComponentToWorlds);

std::vector<FTransform> GetPlayerComponentToWorlds(std::vector<DWORD_PTR> ptr);
std::vector<DWORD_PTR> GetItemInformationComponents(std::vector<DWORD_PTR> ptr);

std::vector<int> GetItemIDs(std::vector<DWORD_PTR> ptr);
template <typename T>
void SetScatterPrepare(DWORD_PTR entity) {
    VmmCore::ScatterPrepare(VmmCore::GetScatterHandle(0), entity, sizeof(T));
}
int GetEntityID(DWORD_PTR entity);
void SetScatterExecute();
void SetScatterClear();

template <typename T>
void GetScatterRead(DWORD_PTR entity, T& vale) {
    VmmCore::ScatterRead<T>(VmmCore::GetScatterHandle(0), entity, vale);
}
