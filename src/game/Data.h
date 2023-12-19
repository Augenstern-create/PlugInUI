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

struct PlayerInfo {
    EntityType Type;                 // 实体类型
    DWORD_PTR Entity;                // 实体指针
    int ObjID;                       // 对象ID
    DWORD_PTR pCharacterName;        // 角色名称指针
    FText CharacterName;             // 角色名称文本
    std::string Name;                // 角色名称字符串  ///
    Vector3 Location;                // 位置           ///
    Vector3 AimOffsets;              // 朝向           ///
    int TeamID;                      // 队伍ID //
    float Health;                    // 生命值//
    float GroggyHealth;              // 濒死状态生命值//
    float Distance;                  // 与玩家的距离
    bool IsMyTeam;                   // 是否为我方队伍
    DWORD_PTR RootComponent;         // 根组件指针
    DWORD_PTR MeshComponent;         // 网格组件指针
    DWORD_PTR PlayerState;           // 玩家状态指针
    DWORD_PTR BoneComponent;         // 骨骼组件指针
    DWORD_PTR StaticMesh;            // 静态网格指针
    FTransform ComponentToWorld;     // 组件到世界的转换
    int KillCount;                   // 击杀数//
    int SurvivalTier;                // 生存阶层//
    int SurvivalLevel;               // 生存等级//
    EPartnerLevel PartnerLevel;      // 伙伴等级//
    float DamageDealtOnEnemy;        // 对敌人造成的伤害//
    ECharacterState CharacterState;  // 角色状态//
    int SpectatedCount;              // 观察次数//
    DWORD_PTR WeaponProcessor;       // 武器处理器指针
    DWORD_PTR EquippedWeapons;       // 装备的武器指针
    BYTE CurrentWeaponIndex;         // 当前武器索引
    DWORD_PTR CurrentWeapon;         // 当前武器指针
    DWORD_PTR EncryptWeaponID;       // 加密武器ID指针
    int WeaponID;                    // 武器ID//
    std::string WeaponName;          // 武器名称//

    struct {
        std::unordered_map<int, FTransform> Bones;       // 骨骼的变换信息
        std::unordered_map<int, Vector3> LocationBones;  // 骨骼的位置信息
        std::unordered_map<int, Vector3> ScreenBones;    // 骨骼在屏幕上的位置信息
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
    DWORD PID;                   // 进程ID
    DWORD_PTR GameBase;          // 游戏数据库
    DWORD_PTR UWorld;            // 世界
    DWORD_PTR CurrentLevel;      // 当前等级
    DWORD_PTR GNames;            // 名称
    DWORD_PTR GameInstance;      // 游戏实例
    DWORD_PTR GameState;         // 游戏状态
    DWORD_PTR LocalPlayer;       // 当前玩家
    DWORD_PTR PlayerController;  // 当前玩家设备
    DWORD_PTR Actor;             // 参与者
    DWORD_PTR ActorArray;        // 参与者数组
    DWORD_PTR MyHUD;
    DWORD_PTR AcknowledgedPawn;     // 本人地址
    DWORD_PTR PlayerCameraManager;  // 相机地址

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
        int TeamID;
        DWORD_PTR Mesh;        //
        DWORD_PTR AnimScript;  //
        Vector3 Location;      // 位置

    } Myself;

    float FOV;         // 视角
    Vector3 Location;  // 地点
    Vector3 Rotation;  // 转向

    std::unordered_map<std::string, GamePlayerInfo> PlayerLists;  // 玩家列表
    mutable std::shared_mutex playerListsMutex;

    struct {
        DWORD_PTR MiniMapWidget;                         //
        float ImageMapSize;                              // 雷达地图大小
        float ZoomFactor;                                // 缩放因子
        FVector2D ScreenCenter;                          // 小地图雷达中心点
        bool MKey;                                       // M键状态
        std::unordered_map<DWORD_PTR, Vector3> Players;  // 雷达玩家数据
    } Radar;
    struct {
        DWORD_PTR MapWidget;                                                    // 地图小部件
        DWORD_PTR MapGrid;                                                      // 地图网格
        DWORD_PTR Slot;                                                         // 插槽
        std::string MapName;                                                    // 地图名称
        float MapSize;                                                          // 地图大小
        float MapZoomValue;                                                     // 缩放大小
        int MapPageIndex;                                                       // 地图索引
        bool Visibility;                                                        // 可见性
        FMargin Layout;                                                         // 布局
        FVector2D Position;                                                     // 位置
        Vector3 WorldOriginLocation;                                            // 世界地址 //初始位置
        int ScreenWidth = 1440;                                                 //
        int ScreenHeight = 900;                                                 //
        FVector2D ScreenCenter = FVector2D(ScreenWidth / 2, ScreenHeight / 2);  //
        std::unordered_map<DWORD_PTR, RadarPlayerData> Players;                 // 雷达玩家数据
    } Map;

    struct {
        mutable std::shared_mutex CachePlayersMutex;   // 缓存玩家互斥锁
        mutable std::shared_mutex PlayersMutex;        // 玩家互斥锁
        mutable std::shared_mutex CacheProjectsMutex;  // 缓存物品互斥锁
        mutable std::shared_mutex ProjectsMutex;       // 物品互斥锁
        mutable std::shared_mutex CacheItemsMutex;     // 缓存项目互斥锁
        mutable std::shared_mutex ItemsMutex;          // 项目互斥锁
        mutable std::shared_mutex ItemGroupsMutex;     // 物品组信息互斥锁

        std::unordered_map<DWORD_PTR, ActorEntityInfo> CacheEntity;  // 缓存实体

        std::vector<PlayerInfo> CachePlayers;    // 缓存玩家信息
        std::vector<PlayerInfo> Players;         // 玩家信息
        std::vector<ItemInfo> CacheItems;        // 缓存物品信息
        std::vector<ItemInfo> Items;             // 物品信息
        std::vector<ItemGroupInfo> ItemGroups;   // 物品组信息
        std::vector<ProjectInfo> CacheProjects;  // 缓存项目信息
        std::vector<ProjectInfo> Projects;       // 项目信息
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
            int DisplayDistance = 600;    // 显示距离
            bool Skeleton = true;         // 骨骼显示
            float SkeletonWidth = 0.5f;   // 骨骼宽度
            bool TeamID = true;           // 队伍标识显示
            bool Name = false;            // 名称显示
            bool KillCount = true;        // 击杀数显示
            bool SpectatedCount = true;   // 观察数显示
            bool Distance = true;         // 距离显示
            bool Health = true;           // 生命值显示
            bool WepaonName = true;       // 武器名称显示
            bool PartnerLevel = false;    // 伙伴等级显示
            bool SurvivalLevel = false;   // 生存等级显示
            bool CharacterState = false;  // 角色状态显示
            bool KDA = false;             // 击败比例显示
            bool Rank = false;            // 排名显示
        } ESP;
        struct {
            ImU32 PlayerTeamCol = IM_COL32(60, 255, 60, 255);  // 玩家队伍颜色
            ImU32 EnemyTeamCol = IM_COL32(255, 60, 60, 255);   // 敌人队伍颜色
            ImU32 VehicleCol = IM_COL32(255, 255, 60, 255);    // 载具颜色
        } color;
    } Config;
};

extern GameData gameData;

namespace Data {
inline void SetCachePlayers(std::vector<PlayerInfo> value) {
    std::unique_lock<std::shared_mutex> lock(gameData.Actors.CachePlayersMutex);
    gameData.Actors.CachePlayers = value;
}

inline std::vector<PlayerInfo> GetCachePlayers() {
    std::shared_lock<std::shared_mutex> lock(gameData.Actors.CachePlayersMutex);
    return gameData.Actors.CachePlayers;
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
    Sleep(100);
    gameData.ActorCount = 0;
    gameData.Actors.CachePlayers = {};
    gameData.Actors.Players = {};
    gameData.Actors.CacheItems = {};
    gameData.Actors.Items = {};
    gameData.Radar.Players.clear();
    gameData.Map.Players.clear();
    gameData.Map.MapGrid = 0;
    gameData.Map.MapWidget = 0;
    gameData.Map.Visibility = 0;
    gameData.PlayerLists.clear();
    gameData.Map.MapSize = 0;
}
}  // namespace Data