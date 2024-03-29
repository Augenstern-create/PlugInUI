#pragma once
#include <iostream>
#include <string>
#include <shared_mutex>
#include "imgui.h"

enum KeyType { MagicBox = 0, NotHave = 1 };
enum Language { Chinese = 0, English = 1 };

struct ReadConfig {
    bool is_drawing_names = true;                             // 绘制名字
    bool is_drawing_hp = true;                                // 绘制血量
    bool is_drawing_teamID = true;                            // 绘制队伍id
    bool is_drawing_location = true;                          // 绘制位置
    bool is_drawing_sight = true;                             // 绘制瞄准线
    bool is_drawing_lock_my = false;                          // 锁定本人
    float location_size = 7.0f;                               // 位置大小
    ImColor foes_location_color = {1.0f, 0.0f, 0.0f, 1.00f};  // 敌人位置颜色
    ImColor team_location_color = {0.0f, 0.0f, 1.0f, 1.00f};  // 队友位置颜色
    ImColor my_location_color = {1.0f, 1.0f, 0.0f, 1.00f};    // 自己位置颜色
    ImColor hp_color = {0.0f, 1.0f, 0.0f, 1.00f};             // 血条颜色
};

struct SkeletonConfig {
    bool is_drawing_box = true;              // 绘制方框
    bool is_drawing_skeleton = true;         // 绘制骨骼
    bool is_drawing_hp = true;               // 绘制血量
    bool is_drawing_distance = true;         // 绘制距离
    bool is_drawing_names = true;            // 绘制名字
    bool is_drawing_team_name = true;        // 绘制队伍名字
    bool is_drawing_teamID = true;           // 绘制队伍id
    bool is_drawing_handheld_weapon = true;  // 绘制手持武器
    bool is_drawing_atlas_radar = true;      // 大地图雷达
    bool is_drawing_small_map_radar = true;  // 小地图雷达

    int blind_spot_quorum = 0;  // 视野盲区人数
    int faced_with_quorum = 0;  // 面对人数

    float skeleton_font_size = 18.0f;       // 骨骼字体大小
    float font_zoom_size = 1.0f;            // 缩放
    float time_per_frame = 0.0f;            // 每帧耗时
    float skeleton_show_distance = 500.0f;  // 骨骼显示距离
    float max_radar_size = 6.0f;            // 大地图雷达大小
    float min_radar_size = 6.0f;            // 小地图雷达大小

    ImVec2 windows_form_size = {0.0f, 0.0f};  // 窗口大小

    ImColor foes_name_color = {1.0f, 1.0f, 0.0f, 1.00f};     // 敌人名字颜色
    ImColor team_name_color = {0.0f, 1.0f, 0.0f, 1.00f};     // 队友名字颜色
    ImColor teamID_color = {0.2f, 1.0f, 0.2f, 1.00f};        // 队伍id颜色
    ImColor frame_rate_color = {0.0f, 1.0f, 0.0f, 1.00f};    // 帧率颜色
    ImColor in_game_info_color = {0.2f, 1.0f, 0.2f, 1.00f};  // 游戏内信息颜色
    ImColor hp_color = {1.0f, 1.0f, 0.0f, 1.00f};            // 血量颜色
    ImColor groggy_hp_color = {1.0f, 0.0f, 0.0f, 1.00f};     // 倒地血量颜色
    ImColor skeleton_color = {1.0f, 0.0f, 0.0f, 1.00f};      // 骨骼颜色
    ImColor box_color = {1.0f, 0.0f, 0.0f, 1.00f};           // 方框颜色
    ImColor max_radar_color = {1.0f, 0.0f, 0.0f, 1.00f};     // 大地图雷达颜色
    ImColor min_radar_color = {1.0f, 0.0f, 0.0f, 1.00f};     // 小地图雷达颜色
};

struct MapRadar {
    std::string map_name;    // 地图名字
    int map_id;              // 地图id
    float map_size;          // 地图大小
    float radar_size;        // 雷达大小
    Vector3 world_location;  // 世界地址

    struct MaxMap {
        DWORD_PTR map_grid;     // 大地图组件
        DWORD_PTR map_radar;    // 大地图雷达
        DWORD_PTR map_address;  // 地图地址
        FMargin declare;        // 布局
        FVector2D position;     // 位置
        bool is_ibility;        // 可见性
        float map_zoom_value;   // 地图缩放系数
    } max_map;

    struct MinMap {
        DWORD_PTR map_radar;    // 小地图雷达
        DWORD_PTR map_address;  // 地图地址
        FMargin declare;        // 布局
        FVector2D position;     // 位置
        bool is_ibility;        // 可见性
        float map_zoom_value;   // 地图缩放系数
    } min_map;

    std::unordered_map<DWORD_PTR, Vector3> rader_players;  // 雷达坐标/比值
};

struct AutoTargeting {
    bool is_lock;             // 锁定状态
    DWORD_PTR target_entity;  // 目标实体
    float target_distance;    // 目标距离
    FVector aiming_point;     // 瞄准点
    int FOV = 40;             // AR模式视野
};

struct Setting {
    // Language language = Language::Chinese;

    Language language = Language::Chinese;  // 中英文切换 0中文,1英文

    float default_font_size = 10.0f;  // 默认字体大小
    float menu_font_size = 18.0f;     // 菜单字体大小

    std::string COMID = "COM5";
    KeyType key_type = KeyType::MagicBox;

    ImColor subform_color = {0.1f, 0.11f, 0.13f, 1.00f};
};
