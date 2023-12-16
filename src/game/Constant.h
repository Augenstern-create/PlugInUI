#pragma once

#include <iostream>

/// @brief 实体类型
enum class EntityType {
    Unknown = 0,       // 未知
    Player = 1,        // 玩家
    AI = 2,            //  AI
    DroppedItem,       // 掉落物品
    DroppedItemGroup,  // 刷新物品
    Item,              // 普通物品
    Vehicle,           // 汽车
    Drug,              // 药物
    Armor,             // 防具
    Accessories,       // 配件
    Weapon,            // 手持武器
    AirPack,           // 空投
    DeadBox,           // 死亡盒子
    Grenade,           // 投掷物品
    Project,           // 投掷物品
    Widget,            // 小工具
    Map                // 地图
};

// 场景
enum class Scene {
    FindProcess = 0,  // 未进游戏
    Lobby = 1,        // 大厅
    Gameing = 2,      // 游戏内
};

/// @brief 文本对齐方式,左,中,右
enum class TextAlignment { Left, Center, Right };
