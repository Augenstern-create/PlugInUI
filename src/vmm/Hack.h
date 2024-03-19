#pragma once

void HackStart();

void UpdateEntitys();
void UpdatePlayers();
void UpdatePlayers2();
void UpdateEntitys2();

#define MEASURE_TIME_START auto start = std::chrono::high_resolution_clock::now();
#define MEASURE_TIME_END                                             \
    auto end = std::chrono::high_resolution_clock::now();            \
    std::chrono::duration<double, std::milli> elapsed = end - start; \
    if (elapsed.count() > 4) std::cout << "Elapsed time: " << elapsed.count() << " milliseconds\n";

enum VMM_Handle : int {
    ENTITY = 0,    // 实体
    PLAYER = 1,    // 玩家
    LOCATION = 2,  // 位置
    FOV = 3,       // 视角
    SKELETON = 4,  // 骨骼
};

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