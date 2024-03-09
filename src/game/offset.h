#pragma once
#include <cstdint>

#include <iostream>
#include <fstream>
#include <locale>
#include <codecvt>

namespace Offset {
// world
constexpr uint32_t UWorld = 0xC34D118;           // 游戏世界偏移
constexpr uint32_t CurrentLevel = 0x150;         // 当前关卡偏移 Leve
constexpr uint32_t GameInstance = 0x160;         // 游戏实例偏移 GameInstence
constexpr uint32_t GameState = 0x110;            // 游戏状态偏移
constexpr uint32_t LocalPlayer = 0x48;           // 本地玩家信息偏移
constexpr uint32_t PlayerController = 0x30;      // 玩家控制器偏移
constexpr uint32_t Actor = 0x280;                // 游戏中角色偏移
constexpr uint32_t GNames = 0xC55A050;           // 游戏中使用的名称地址
constexpr uint32_t ObjID = 0x18;                 // 对象 ID 偏移
constexpr uint32_t AcknowledgedPawn = 0x4A8;     // 本人组地址
constexpr uint32_t PlayerCameraManager = 0x4D0;  // 相机地址
constexpr uint32_t ViewTarget = 0xA70;           // 当前镜头
constexpr uint32_t MyHUD = 0x4C8;                // 地图组件

// Decrypt
constexpr uint32_t XDecrypt = 0xA95A328;  // 用于解密操作偏移 Shield指针
constexpr uint32_t ChunkSize = 0x40C4;    // 内存块的大小 IDD
constexpr uint32_t IsRor = 0x0;           // 是否使用 ROR 操作进行解密的标志
constexpr uint32_t RorValue = 0x1;        // ROR 操作的值
constexpr uint32_t XorKey1 = 0xD97332BA;  // XOR 解密的密钥1 DecryptInternalIndexXorKey1
constexpr uint32_t XorKey2 = 0xC9EB964F;  // XOR 解密的密钥2 DecryptInternalIndexXorKey2

// Game
constexpr uint32_t WorldToMap = 0x9B4;   // 世界坐标到地图坐标的转换参数 世界转小地图
constexpr uint32_t PlayerArray = 0x410;  // 玩家数组偏移

// Project
constexpr uint32_t RootComponent = 0x3B0;      // 根组件偏移
constexpr uint32_t ComponentLocation = 0x330;  // 组件位置偏移
constexpr uint32_t TimeTillExplosion = 0x7AC;  // 投掷物爆炸时间
constexpr uint32_t DroppedItem = 0x458;        // 物品组件
constexpr uint32_t DroppedItemGroup = 0x360;   // 其他物品

// Player
constexpr uint32_t PlayerName = 0x3F8;          // 玩家姓名偏移
constexpr uint32_t AccountId = 0xA68;           // 玩家账户 ID 偏移
constexpr uint32_t PubgIdData = 0xE88;          // PUBG ID 数据偏移
constexpr uint32_t PlayerState = 0x0418;        // 玩家状态偏移
constexpr uint32_t TeamNumber = 0x1E78;         // 玩家所在队伍编号偏移
constexpr uint32_t CharacterName = 0xED0;       // 角色名字偏移 //Playname
constexpr uint32_t Health = 0x9A0;              // 玩家生命值偏移
constexpr uint32_t HealthMax = 0x9D4;           // 正常血量上限
constexpr uint32_t GroggyHealth = 0x1C70;       // 玩家濒死状态生命值偏移
constexpr uint32_t GroggyHealthMax = 0x1E5C;    // 倒地血量上限
constexpr uint32_t CharacterState = 0xF30;      // 玩家角色状态偏移
constexpr uint32_t SpectatedCount = 0x1E18;     // 观察的数量
constexpr uint32_t WeaponProcessor = 0x900;     // 武器处理器偏移
constexpr uint32_t ComponentToWorld = 0x320;    // 组件转世界坐标偏移
constexpr uint32_t StaticMesh = 0xAB0;          // 静态网格偏移 //Bone
constexpr uint32_t PlayerStatistics = 0xAA4;    // 玩家统计数据偏移 //PlayerStatistics
constexpr uint32_t DamageDealtOnEnemy = 0x6D4;  // 对敌人造成的伤害值偏移
constexpr uint32_t SurvivalLevel = 0xE8C;       // 存活等级偏移
constexpr uint32_t PartnerLevel = 0x70E;        // 伙伴等级偏移
constexpr uint32_t EquippedWeapons = 0x1F8;     // 已装备武器偏移
constexpr uint32_t CurrentWeaponIndex = 0x301;  // 当前武器索引偏移
constexpr uint32_t AimOffsets = 0x1810;         // 被瞄偏移 取Y

// MapVisibility
constexpr uint32_t WidgetStateMap = 0x500;
constexpr uint32_t MapWidget = 0x700;
constexpr uint32_t TrainingMapGrid = 0x6D0;
constexpr uint32_t Visibility = 0xA1;
constexpr uint32_t Slot = 0x30;
constexpr uint32_t LayoutData = 0x38;
constexpr uint32_t Alignment = 0x20;
constexpr uint32_t BlockInputWidgetList = 0x568;
constexpr uint32_t SelectMinimapSizeIndex = 0x5B8;

// Mesh
constexpr uint32_t VehicleRiderComponent = 0x1FB0;
constexpr uint32_t Mesh = 0x4A0;  // 玩家模型偏移
constexpr uint32_t AnimScriptInstance = 0xC50;
constexpr uint32_t Playname = 0xED0;
constexpr uint32_t SeatIndex = 0x220;
constexpr uint32_t LastTeamNum = 0x1E78;  // TeamNumber

// skeleton
constexpr uint32_t Bone = 0xAB0;      // StaticMesh
constexpr uint32_t BoneArry = 0x320;  // ComponentToWorld
constexpr uint32_t FOV = 0x490;       // 视野参数偏移 //CameraFov
constexpr uint32_t Rotation = 0xA30;  // 玩家旋转参数偏移 //CameraRot
constexpr uint32_t Location = 0xA10;  // 玩家位置参数偏移 //CameraPos

// automatic aiming
constexpr uint32_t WeaponTrajectoryData = 0x10C0;
constexpr uint32_t TrajectoryConfig = 0x100;
constexpr uint32_t FloatCurves = 0x30;
constexpr uint32_t Keys = 0x60;
constexpr uint32_t ComponentVelocity = 0x2F0;    // 速度组件
constexpr uint32_t Veloctity = 0x3D0;            // 速度
constexpr uint32_t TrajectoryGravityZ = 0x102C;  // 重力轨迹
constexpr uint32_t FiringAttachPoint = 0x860;    // 发射链接点
constexpr uint32_t ScopingAttachPoint = 0xC68;   // 范围界定
constexpr uint32_t Mesh3P = 0x7B0;               // 范围界定

constexpr uint32_t IsABot = 0x44c;
constexpr uint32_t IsSpectator = 0x410;
constexpr uint32_t AlivePlayers = 0x498;
constexpr uint32_t AliveTeams = 0x4DC;
constexpr uint32_t ItemInformationComponent = 0xB0;  // 物品一

constexpr uint32_t CharacterClanInfo = 0x0C28;

constexpr uint32_t BallisticCurve = 0x0028;

constexpr uint32_t LastVehiclePawn = 0x0270;
constexpr uint32_t ReplicatedMovement = 0x0060;

constexpr uint32_t ControlRotation_CP = 0x07FC;
constexpr uint32_t RecoilADSRotation_CP = 0x09FC;

constexpr uint32_t DroppedItemGroupCount = 0x0728;
constexpr uint32_t DroppedItemGroupUItem = 0x0728;
constexpr uint32_t ItemID = 0x0268;  // 物品二

}  // namespace Offset