#pragma once

#include <Windows.h>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <list>
#include <unordered_map>
#include "Constant.h"

#include "Engine.h"
#include "SrcUe4Math.h"
#include "srcUtils.h"
#include "imgui.h"
#include "config.h"
#include "offset.h"

struct PlayerInfo {
    EntityType Type;                 // 实体类型
    DWORD_PTR Entity;                // 实体指针
    int ObjID;                       // 对象ID
    DWORD_PTR pCharacterName;        // 角色名称指针
    FText CharacterName;             // 角色名称文本
    std::string Name;                // 角色名称字符串
    std::string TeamName;            // 队伍名称字符串
    Vector3 Location;                // 位置
    Vector3 screenLocation;          // 位置转屏幕
    Vector3 AimOffsets;              // 朝向
    int TeamID;                      // 队伍ID
    float Health;                    // 生命值
    float HealthMax;                 // 生命值max
    float GroggyHealth;              // 濒死状态生命值
    float GroggyHealthMax;           // 濒死状态生命值max
    float Distance;                  // 距离
    bool IsMyTeam;                   // 是否为我方队伍
    bool IsSkeleton;                 // 骨骼可见
    DWORD_PTR RootComponent;         // 根组件指针
    DWORD_PTR MeshComponent;         // 网格组件指针
    DWORD_PTR PlayerState;           // 玩家状态指针
    DWORD_PTR BoneComponent;         // 骨骼组件指针
    DWORD_PTR StaticMesh;            // 静态网格指针
    FTransform ComponentToWorld;     // 组件到世界的转换
    int KillCount;                   // 击杀数
    int SurvivalTier;                // 生存阶层
    int SurvivalLevel;               // 生存等级
    EPartnerLevel PartnerLevel;      // 伙伴等级
    float DamageDealtOnEnemy;        // 对敌人造成的伤害
    ECharacterState CharacterState;  // 角色状态
    int SpectatedCount;              // 观察次数
    DWORD_PTR WeaponProcessor;       // 武器处理器指针
    DWORD_PTR EquippedWeapons;       // 装备的武器指针
    BYTE CurrentWeaponIndex;         // 当前武器索引
    DWORD_PTR CurrentWeapon;         // 当前武器指针
    DWORD_PTR EncryptWeaponID;       // 加密武器ID指针
    int WeaponID;                    // 武器ID
    std::string WeaponName;          // 武器名称

    DWORD_PTR Bone;    // 骨骼
    FVector BoneArry;  // 骨骼

    struct {
        std::unordered_map<int, FTransform> Bones;       // 骨骼的变换信息
        std::unordered_map<int, Vector3> LocationBones;  // 骨骼的位置信息
        std::unordered_map<int, Vector3> ScreenBones;    // 骨骼在屏幕上的位置信息
        Vector3 skeletonsBox1;
        Vector3 skeletonsBox2;
    } Skeleton;

    struct Skeletons {
        std::unordered_map<int, Vector3> Bones;  // 骨骼的位置信息
    };

    Skeletons skeletons;  // 骨骼信息

    FVector2D RadarScreenLocation;  // 雷达上的屏幕位置
};

struct ItemInfo {
    EntityType Type;                 // 实体类型
    DWORD_PTR Entity;                // 实体指针
    DWORD_PTR InformationComponent;  // 信息组件指针
    DWORD_PTR RootComponent;         // 根组件指针
    int ID;                          // 物品ID
    std::string Name;                // 物品名称
    Vector3 Location;                // 位置
    Vector3 ScreenLocation;          // 屏幕位置
    float Distance;                  // 与玩家的距离
    DWORD_PTR DroppedItem;
};

struct ItemGroupInfo {
    EntityType Type;   // 实体类型
    DWORD_PTR Entity;  // 实体指针
    int ID;            // 物品组ID
    DWORD_PTR Items;   // 物品指针
    int Count;         // 物品数量
};

struct ActorEntityInfo {
    DWORD_PTR Entity;  // 实体指针
    int ID;            // ID
    int decodeID;      // 解码后的ID
    EntityType type;   // 实体类型
};

struct ProjectInfo {
    EntityType Type;          // 实体类型
    DWORD_PTR Entity;         // 实体指针
    int ID;                   // 项目ID
    std::string Name;         // 项目名称
    DWORD_PTR RootComponent;  // 根组件指针
    Vector3 Location;         // 位置
    Vector3 ScreenLocation;   // 屏幕位置
    float Distance;           // 与玩家的距离
    float TimeTillExplosion;  // 爆炸剩余时间
};
struct RadarPlayerData {
    FVector2D RadarScreenLocation;  // 雷达玩家在屏幕上的位置
};

struct GamePlayerInfo {
    DWORD_PTR pPlayerInfo;                   // 玩家信息指针
    DWORD_PTR pTeam;                         // 玩家队伍指针
    DWORD_PTR pCharacter;                    // 玩家角色指针
    DWORD_PTR pPlayerName;                   // 玩家名称指针
    FText FPlayerName;                       // 玩家名称文本
    std::string PlayerName;                  // 玩家名称字符串
    DWORD_PTR pAccountId;                    // 玩家账户ID指针
    FText FAccountId;                        // 玩家账户ID文本
    std::string AccountId;                   // 玩家账户ID字符串
    int TeamID;                              // 玩家队伍ID
    EPartnerLevel PartnerLevel;              // 伙伴等级
    int KillCount;                           // 击杀数
    DWORD_PTR pPubgIdData;                   // PUBG ID数据指针
    FWuPubgIdData PubgIdData;                // PUBG ID数据结构
    FWuCharacterClanInfo CharacterClanInfo;  // 玩家角色公会信息
    DWORD_PTR pClanName;                     // 公会名称指针
    FText FClanName;                         // 公会名称文本
    std::string ClanName;                    // 公会名称字符串
    int E;
};

struct GameData {
    DWORD PID;                      // 进程ID
    DWORD_PTR GameBase;             // 游戏数据库
    DWORD_PTR GNames;               // 名称地址
    DWORD_PTR UWorld;               // 世界基址
    DWORD_PTR GameState;            // 游戏状态
    DWORD_PTR CurrentLevel;         // 世界一级
    DWORD_PTR GameInstance;         // 本人一级
    DWORD_PTR LocalPlayer;          // 本人三级
    DWORD_PTR PlayerController;     // 本人四级
    DWORD_PTR Actor;                // 指针地址
    DWORD_PTR AcknowledgedPawn;     // 本人数组
    DWORD_PTR PlayerCameraManager;  // 相机地址
    DWORD_PTR ViewTarget;           // 当前对象
    DWORD_PTR MyHUD;                // 地图组件

    Scene Scene = Scene::FindProcess;  // 场景
    int PlayerCount;                   // 玩家人数
    int ActorCount;                    // 参与者人数
    DWORD_PTR LocalPlayerPawn;         // 当前玩家位置
    DWORD_PTR LocalPlayerTeamID;       // 当前玩家队名
    DWORD_PTR LocalPlayerMesh;         // 当前玩家区域
    struct {
        DWORD_PTR PlayerPtr;
        std::string Name;
        std::string ClanName;
        std::string WeaponName;
        int WeaponID;
        int TeamID;
        DWORD_PTR Mesh;         //
        DWORD_PTR AnimScript;   //
        Vector3 Location;       // 位置
        ImVec2 AtlaseLocation;  // 转换后位置
        bool isGetPlayer = false;
    } Myself;

    float FOV;         // 视角
    Vector3 Location;  // 地点
    Vector3 Rotation;  // 转向
    int ScreenWidth;
    int ScreenHeight;
    FVector2D ScreenCenter;

    ReadConfig readConfig;
    SkeletonConfig skeletonConfig;
    MapRadar mapRadar;
    Setting setting;
    AutoTargeting autoTarget;
    std::unordered_map<std::string, uint32_t> Offset;
    std::unordered_map<std::string, GamePlayerInfo> PlayerLists;  // 玩家列表
    mutable std::shared_mutex playerListsMutex;

    struct {
        mutable std::shared_mutex CacheEntityMutex;    // 实体信息互斥锁
        mutable std::shared_mutex CachePlayersMutex;   // 缓存玩家互斥锁
        mutable std::shared_mutex CachePlayersMutex2;  // 缓存玩家互斥锁
        mutable std::shared_mutex PlayersMutex;        // 玩家互斥锁
        mutable std::shared_mutex CacheProjectsMutex;  // 缓存物品互斥锁
        mutable std::shared_mutex ProjectsMutex;       // 物品互斥锁
        mutable std::shared_mutex CacheItemsMutex;     // 缓存项目互斥锁
        mutable std::shared_mutex ItemsMutex;          // 项目互斥锁
        mutable std::shared_mutex ItemGroupsMutex;     // 物品组信息互斥锁
        std::vector<ActorEntityInfo> CacheEntity;      // 缓存实体
        std::vector<PlayerInfo> CachePlayers;          // 缓存玩家信息
        std::vector<PlayerInfo> CachePlayers2;         // 缓存玩家信息
        std::vector<PlayerInfo> Players;               // 玩家信息
        std::vector<ItemInfo> CacheItems;              // 缓存物品信息
        std::vector<ItemInfo> Items;                   // 物品信息
        std::vector<ItemGroupInfo> ItemGroups;         // 物品组信息
        std::vector<ProjectInfo> CacheProjects;        // 缓存投掷物信息
        std::vector<ProjectInfo> Projects;             // 投掷物信息
    } Actors;

    struct {
        bool Lock;               // 锁定状态
        DWORD_PTR TargetEntity;  // 目标实体
        float ScreenDistance;    // 屏幕距离
    } AimBot;

    struct {
        struct {
            struct {
                int FOV = 40;  // AR模式视野
            } AR;
        } AimBot;
        struct {
            ImU32 PlayerTeamCol = IM_COL32(60, 60, 255, 255);  // 玩家队伍颜色
            ImU32 EnemyTeamCol = IM_COL32(255, 60, 60, 255);   // 敌人队伍颜色
            ImU32 VehicleCol = IM_COL32(255, 255, 60, 255);    // 载具颜色

            ImU32 TeamSkeletonCol = IM_COL32(255, 255, 60, 255);  // 队友骨骼颜色
            ImU32 EnemySkeletonCol = IM_COL32(255, 60, 60, 255);  // 敌人骨骼颜色

            ImColor visualBoneColor = {0.5f, 0.5f, 0.5f, 1.00f};            // 可视骨骼颜色
            ImColor invisibleBoneColor = {0.5f, 0.5f, 0.5f, 1.00f};         // 不可视骨骼颜色
            ImColor fallenSkeletonColor = {0.5f, 0.5f, 0.5f, 1.00f};        // 倒地骨骼颜色
            ImColor downBloodColor = {0.5f, 0.5f, 0.5f, 1.00f};             // 倒地血量颜色
            ImColor botSkeletonColor = {0.5f, 0.5f, 0.5f, 1.00f};           // 机器人骨骼颜色
            ImColor rayColor = {0.5f, 0.5f, 0.5f, 1.00f};                   // 射线颜色
            ImColor collaboratorSkeletonColor = {0.5f, 0.5f, 0.5f, 1.00f};  // 协作者骨骼颜色
            ImColor collaboratorInfoColor = {0.5f, 0.5f, 0.5f, 1.00f};      // 协作者信息颜色
            ImColor playerNameColor = {0.5f, 0.5f, 0.5f, 1.00f};            // 玩家名称颜色
            ImColor playerInfoColor = {0.5f, 0.5f, 0.5f, 1.00f};            // 玩家信息颜色
            ImColor dangerInfoColor = {0.5f, 0.5f, 0.5f, 1.00f};            // 危险信息颜色
            ImColor handheldWeaponColor = {0.5f, 0.5f, 0.5f, 1.00f};        // 手持武器颜色
            ImColor vehicleColor = {0.5f, 0.5f, 0.5f, 1.00f};               // 车辆颜色
            ImColor airdropColor = {0.5f, 0.5f, 0.5f, 1.00f};               // 空投颜色
            ImColor boxColor = {0.5f, 0.5f, 0.5f, 1.00f};                   // 箱子颜色
            ImColor throwColor = {0.5f, 0.5f, 0.5f, 1.00f};                 // 投掷颜色
            ImColor materialsGroup1Color = {0.5f, 0.5f, 0.5f, 1.00f};       // 材料组 1 颜色
            ImColor materialsGroup2Color = {0.5f, 0.5f, 0.5f, 1.00f};       // 材料组 2 颜色
            ImColor materialsGroup3Color = {0.5f, 0.5f, 0.5f, 1.00f};       // 材料组 3 颜色
            ImColor materialsGroup4Color = {0.5f, 0.5f, 0.5f, 1.00f};       // 材料组 4 颜色
        } Color;

        struct {
            int multilingualism = 0;    // 中英文切换 0中文,1英文
            float DefaultSize = 10.0f;  // 默认字体大小
        } Font;

        struct {
            bool showBox = false;             // 显示盒子
            bool showSkeleton = false;        // 显示骨骼
            bool showBloodstain = false;      // 显示血量
            bool showLength = false;          // 显示距离
            bool showFov = false;             // 显示视野
            bool isGlobal = false;            // 全局开关
            bool vehicleHealth = false;       // 显示车辆健康信息
            bool vehicleFuel = false;         // 显示车辆燃料信息
            bool showAirdrop = false;         // 显示空投
            bool airdropItems = false;        // 显示空投物品
            bool showBox2 = false;            // 显示第二个盒子
            bool BoxItems = false;            // 显示盒子物品
            bool throwReminder = false;       // 投掷提醒
            bool pullPinTimer = false;        // 手雷引爆倒计时
            bool showKillCount = false;       // 显示击杀数量
            bool showAimLine = false;         // 显示瞄准线
            bool showPlayerName = false;      // 显示玩家名称
            bool showTeamName = false;        // 显示队伍名称
            bool showHandheldWeapon = false;  // 显示手持武器
            bool showSurvivalLevel = false;   // 显示生存等级
            bool showSpectatorCount = false;  // 显示观战人数
            bool showTeamNumber = false;      // 显示队伍编号
            bool showPlayerRank = false;      // 显示玩家排名
            bool showAverageDamage = false;   // 显示平均伤害
            bool showPlayerKda = false;       // 显示玩家KDA
            bool showDamageStats = false;     // 显示伤害统计
            bool showSmartDisplay = false;    // 显示智能显示
            bool showNearbyEnemies = false;   // 显示附近敌人
            bool showBigMapRadar = false;     // 显示大地图雷达
            bool showSmallMapRadar = false;   // 显示小地图雷达
        } Box;
        struct {
            float lengthDistance = 0.0f;     // 长度显示距离
            float skeletonDistance = 0.0f;   // 骨骼显示距离
            float nameDistance = 0.0f;       // 名称显示距离
            float handheldDistance = 0.0f;   // 手持武器显示距离
            float infoDistance = 0.0f;       // 信息显示距离
            float skeletonThickness = 0.0f;  // 骨骼线条粗细
            float vehicleDistance = 0.0f;    // 车辆信息显示距离
            float vehicleFontSize = 0.0f;    // 车辆信息字体大小
            float airdropDistance = 0.0f;    // 空投信息显示距离
            float airdropFontSize = 0.0f;    // 空投信息字体大小
            float boxDistance = 0.0f;        // 盒子信息显示距离
            float boxFontSize = 0.0f;        // 盒子信息字体大小
            float boxThickness = 0.0f;       // 盒子线条粗细
            float fontSize = 10.0f;          // 字体大小
            float skeletonfontSize = 18.0f;  // 骨骼字体大小
            float dangerKillCount = 0.0f;    // 危险击杀数量
            float nearbyDistance = 0.0f;     // 附近敌人显示距离
            float lockOpacity = 0.0f;        // 锁定透明度
            float throwDistance = 0.0f;      // 投掷物显示距离
            float throwFontSize = 0.0f;      // 投掷物字体大小
            float supplyDistance = 0.0f;     // 供应箱显示距离
            float supplyFontSize = 0.0f;     // 供应箱字体大小
        } Slider;
    } Config;
};

extern GameData gameData;

namespace Data {

inline void SetCacheEntity(std::vector<ActorEntityInfo> value) {
    std::unique_lock<std::shared_mutex> lock(gameData.Actors.CacheEntityMutex);
    gameData.Actors.CacheEntity = value;
}

inline std::vector<ActorEntityInfo> GetCacheEntity() {
    std::shared_lock<std::shared_mutex> lock(gameData.Actors.CacheEntityMutex);
    return gameData.Actors.CacheEntity;
}

inline void SetCachePlayers(std::vector<PlayerInfo> value) {
    std::unique_lock<std::shared_mutex> lock(gameData.Actors.CachePlayersMutex);
    gameData.Actors.CachePlayers = value;
}

inline std::vector<PlayerInfo> GetCachePlayers() {
    std::shared_lock<std::shared_mutex> lock(gameData.Actors.CachePlayersMutex);
    return gameData.Actors.CachePlayers;
}

inline void SetCachePlayers2(std::vector<PlayerInfo> value) {
    std::unique_lock<std::shared_mutex> lock(gameData.Actors.CachePlayersMutex2);
    gameData.Actors.CachePlayers2 = value;
}

inline std::vector<PlayerInfo> GetCachePlayers2() {
    std::shared_lock<std::shared_mutex> lock(gameData.Actors.CachePlayersMutex2);
    return gameData.Actors.CachePlayers2;
}

inline void SetPlayers(std::vector<PlayerInfo> value) {
    std::unique_lock<std::shared_mutex> lock(gameData.Actors.PlayersMutex);
    gameData.Actors.Players = value;
}

inline std::vector<PlayerInfo> GetPlayers() {
    std::shared_lock<std::shared_mutex> lock(gameData.Actors.PlayersMutex);
    return gameData.Actors.Players;
}

inline void SetCacheProjects(std::vector<ProjectInfo> value) {
    std::unique_lock<std::shared_mutex> lock(gameData.Actors.CacheProjectsMutex);
    gameData.Actors.CacheProjects = value;
}

inline std::vector<ProjectInfo> GetCacheProjects() {
    std::shared_lock<std::shared_mutex> lock(gameData.Actors.CacheProjectsMutex);
    return gameData.Actors.CacheProjects;
}

inline void SetProjects(std::vector<ProjectInfo> value) {
    std::unique_lock<std::shared_mutex> lock(gameData.Actors.ProjectsMutex);
    gameData.Actors.Projects = value;
}

inline std::vector<ProjectInfo> GetProjects() {
    std::shared_lock<std::shared_mutex> lock(gameData.Actors.ProjectsMutex);
    return gameData.Actors.Projects;
}

inline void SetCacheItems(std::vector<ItemInfo> value) {
    std::unique_lock<std::shared_mutex> lock(gameData.Actors.CacheItemsMutex);
    gameData.Actors.CacheItems = value;
}

inline std::vector<ItemInfo> GetCacheItems() {
    std::shared_lock<std::shared_mutex> lock(gameData.Actors.CacheItemsMutex);
    return gameData.Actors.CacheItems;
}

inline void SetItems(std::vector<ItemInfo> value) {
    std::unique_lock<std::shared_mutex> lock(gameData.Actors.ItemsMutex);
    gameData.Actors.Items = value;
}

inline std::vector<ItemInfo> GetItems() {
    std::shared_lock<std::shared_mutex> lock(gameData.Actors.ItemsMutex);
    return gameData.Actors.Items;
}

inline void SetItemGroups(std::vector<ItemGroupInfo> value) {
    std::unique_lock<std::shared_mutex> lock(gameData.Actors.ItemGroupsMutex);
    gameData.Actors.ItemGroups = value;
}

inline std::vector<ItemGroupInfo> GetItemGroups() {
    std::shared_lock<std::shared_mutex> lock(gameData.Actors.ItemGroupsMutex);
    return gameData.Actors.ItemGroups;
}

inline void Clears() {
    if (gameData.Actors.CacheEntity.size() != 0) {
        std::unique_lock<std::shared_mutex> lock(gameData.Actors.CacheEntityMutex);
        gameData.Actors.CacheEntity.clear();
    }
    if (gameData.Actors.CachePlayers.size() != 0) {
        std::unique_lock<std::shared_mutex> lock(gameData.Actors.CachePlayersMutex);
        gameData.Actors.CachePlayers.clear();
    }
    if (gameData.Actors.CachePlayers2.size() != 0) {
        std::unique_lock<std::shared_mutex> lock(gameData.Actors.CachePlayersMutex2);
        gameData.Actors.CachePlayers2.clear();
    }
    if (gameData.Actors.Players.size() != 0) {
        std::unique_lock<std::shared_mutex> lock(gameData.Actors.PlayersMutex);
        gameData.Actors.Players.clear();
        gameData.ActorCount = 0;
        gameData.FOV = 0.0f;
        gameData.Location = {0.0f, 0.0f, 0.0f};
        gameData.Rotation = {0.0f, 0.0f, 0.0f};
    }
    if (gameData.Actors.CacheItems.size() != 0) {
        std::unique_lock<std::shared_mutex> lock(gameData.Actors.CacheItemsMutex);
        gameData.Actors.CacheItems.clear();
    }
    if (gameData.Actors.Items.size() != 0) {
        std::unique_lock<std::shared_mutex> lock(gameData.Actors.ItemsMutex);
        gameData.Actors.Items.clear();
    }
    if (gameData.Actors.ItemGroups.size() != 0) {
        std::unique_lock<std::shared_mutex> lock(gameData.Actors.ItemGroupsMutex);
        gameData.Actors.ItemGroups.clear();
    }
    if (gameData.Actors.CacheProjects.size() != 0) {
        std::unique_lock<std::shared_mutex> lock(gameData.Actors.CacheProjectsMutex);
        gameData.Actors.CacheProjects.clear();
    }
    if (gameData.Actors.Projects.size() != 0) {
        std::unique_lock<std::shared_mutex> lock(gameData.Actors.ProjectsMutex);
        gameData.Actors.Projects.clear();
    }

    if (!gameData.Myself.Name.empty() || gameData.Myself.PlayerPtr != 0) {
        gameData.Myself.Name = "";
        gameData.Myself.PlayerPtr = 0;
        gameData.Myself.ClanName = "";
        gameData.Myself.TeamID = 0;
        gameData.Myself.Mesh = 0;
        gameData.Myself.AnimScript = 0;
        gameData.Myself.Location = {0, 0, 0};
    }
    if (gameData.mapRadar.map_name != "") {
        gameData.mapRadar.map_size = 0;
        gameData.mapRadar.radar_size = 0;
        gameData.mapRadar.world_location = {0.0f, 0.0f, 0.0f};

        gameData.mapRadar.max_map.map_grid = 0;
        gameData.mapRadar.max_map.map_radar = 0;
        gameData.mapRadar.max_map.map_address = 0;
        gameData.mapRadar.max_map.is_ibility = false;
        gameData.mapRadar.max_map.map_zoom_value = 0;
        gameData.mapRadar.max_map.position = {0.0f, 0.0f};
        gameData.mapRadar.max_map.declare = {0.0f, 0.0f, 0.0f, 0.0f};

        gameData.mapRadar.min_map.map_radar = 0;
        gameData.mapRadar.min_map.map_address = 0;
        gameData.mapRadar.min_map.is_ibility = false;
        gameData.mapRadar.min_map.map_zoom_value = 0;
        gameData.mapRadar.min_map.position = {0.0f, 0.0f};
        gameData.mapRadar.min_map.declare = {0.0f, 0.0f, 0.0f, 0.0f};

        gameData.mapRadar.rader_players.clear();
    }

    Sleep(100);
}

inline void OffsetInit() {
    gameData.Offset["Uworld"] = Offset::UWorld;
    gameData.Offset["Leve"] = Offset::CurrentLevel;
    gameData.Offset["GameInstence"] = Offset::GameInstance;
    gameData.Offset["GameState"] = Offset::GameState;
    gameData.Offset["LocalPlayer"] = Offset::LocalPlayer;
    gameData.Offset["PlayerController"] = Offset::PlayerController;
    gameData.Offset["Actor"] = Offset::Actor;
    gameData.Offset["GNames"] = Offset::GNames;
    gameData.Offset["ObjID"] = Offset::ObjID;
    gameData.Offset["AcknowledgedPawn"] = Offset::AcknowledgedPawn;
    gameData.Offset["PlayerCameraManager"] = Offset::PlayerCameraManager;
    gameData.Offset["ViewTarget"] = Offset::ViewTarget;
    gameData.Offset["MyHUD"] = Offset::MyHUD;

    gameData.Offset["Decrypt"] = Offset::XDecrypt;  // Shield指针
    gameData.Offset["IDD"] = Offset::ChunkSize;
    gameData.Offset["Pz"] = Offset::IsRor;           //
    gameData.Offset["TableOne"] = Offset::RorValue;  //
    gameData.Offset["TableTwo"] = Offset::SorValue;  //
    gameData.Offset["DecryptOne"] = Offset::XorKey1;
    gameData.Offset["DecryptTwo"] = Offset::XorKey2;

    gameData.Offset["WorldToMap"] = Offset::WorldToMap;
    gameData.Offset["PlayerArray"] = Offset::PlayerArray;

    gameData.Offset["m_rootComponent"] = Offset::RootComponent;
    gameData.Offset["ComponentLocation"] = Offset::ComponentLocation;
    gameData.Offset["TimeTillExplosion"] = Offset::TimeTillExplosion;
    gameData.Offset["DroppedItem"] = Offset::DroppedItem;
    gameData.Offset["DroppedItemGroup"] = Offset::DroppedItemGroup;

    gameData.Offset["PlayerName"] = Offset::PlayerName;
    gameData.Offset["AccountId"] = Offset::AccountId;
    gameData.Offset["PubgIdData"] = Offset::PubgIdData;
    gameData.Offset["PlayerState"] = Offset::PlayerState;
    gameData.Offset["TeamNumber"] = Offset::LastTeamNum;
    gameData.Offset["Playname"] = Offset::CharacterName;
    gameData.Offset["Health"] = Offset::Health;
    gameData.Offset["HealthMax"] = Offset::HealthMax;
    gameData.Offset["Health_Die"] = Offset::GroggyHealth;
    gameData.Offset["Health_DieMax"] = Offset::GroggyHealthMax;
    gameData.Offset["CharacterState"] = Offset::CharacterState;
    gameData.Offset["SpectatedCount"] = Offset::SpectatedCount;
    gameData.Offset["WeaponProcessor"] = Offset::WeaponProcessor;
    gameData.Offset["ComponentToWorld"] = Offset::ComponentToWorld;
    gameData.Offset["Bone"] = Offset::StaticMesh;
    gameData.Offset["PlayerSatisitc"] = Offset::PlayerStatistics;
    gameData.Offset["DamageDealtOnEnemy"] = Offset::DamageDealtOnEnemy;
    gameData.Offset["SurvivalLevel"] = Offset::SurvivalLevel;
    gameData.Offset["PartnerLevel"] = Offset::PartnerLevel;
    gameData.Offset["EquippedWeapons"] = Offset::EquippedWeapons;
    gameData.Offset["CurrentWeaponIndex"] = Offset::CurrentWeaponIndex;
    gameData.Offset["AimOffsets"] = Offset::AimOffsets;  // 被瞄偏移

    gameData.Offset["WidgetStateMap"] = Offset::WidgetStateMap;
    gameData.Offset["MapWidget"] = Offset::MapWidget;
    gameData.Offset["TrainingMapGrid"] = Offset::TrainingMapGrid;
    gameData.Offset["Visibility"] = Offset::Visibility;
    gameData.Offset["Slot"] = Offset::Slot;
    gameData.Offset["LayoutData"] = Offset::LayoutData;
    gameData.Offset["Alignment"] = Offset::Alignment;
    gameData.Offset["BlockInputWidgetList"] = Offset::BlockInputWidgetList;
    gameData.Offset["SelectMinimapSizeIndex"] = Offset::SelectMinimapSizeIndex;

    gameData.Offset["VehicleRiderComponent"] = Offset::VehicleRiderComponent;
    gameData.Offset["Mesh"] = Offset::Mesh;
    gameData.Offset["AnimScriptInstance"] = Offset::AnimScriptInstance;
    gameData.Offset["SeatIndex"] = Offset::SeatIndex;

    gameData.Offset["BoneArry"] = Offset::BoneArry;
    gameData.Offset["CameraFov"] = Offset::FOV;
    gameData.Offset["CameraRot"] = Offset::Rotation;
    gameData.Offset["CameraPos"] = Offset::Location;

    // automatic aiming
    gameData.Offset["WeaponTrajectoryData"] = Offset::WeaponTrajectoryData;
    gameData.Offset["TrajectoryConfig"] = Offset::TrajectoryConfig;
    gameData.Offset["FloatCurves"] = Offset::FloatCurves;  //
    gameData.Offset["Keys"] = Offset::Keys;                //
    gameData.Offset["ComponentVelocity"] = Offset::ComponentVelocity;
    gameData.Offset["Veloctity"] = Offset::Veloctity;
}
}  // namespace Data