#pragma once

void HackStart();

void UpdateEntitys();
void UpdatePlayers();
void UpdatePlayers2();
void UpdateEntitys2();

struct FName {
    int ComparisonIndex = 0;
    int Number = 0;
    FName() : ComparisonIndex(0), Number(0) {}
    bool operator==(const FName& rhs) const { return ComparisonIndex == rhs.ComparisonIndex && Number == rhs.Number; }
    bool operator!=(const FName& rhs) const { return !(*this == rhs); }
};
struct FMeshBoneInfo {
    FName Name;
    int32_t ParentIndex;
    int32_t pad;
};

enum ERelativeTransformSpace : uint8_t {
    RTS_World,
    RTS_Actor,
    RTS_Component,
    RTS_ParentBoneSpace,
};