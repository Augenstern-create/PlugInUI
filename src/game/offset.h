#pragma once
#include <cstdint>

namespace Offset {
// world
constexpr uint32_t UWorld = 0xA5D2208;            // 游戏世界偏移
constexpr uint32_t CurrentLevel = 0x0150;         // 当前关卡偏移
constexpr uint32_t GameInstance = 0x0160;         // 游戏实例偏移
constexpr uint32_t GameState = 0x0110;            // 游戏状态偏移
constexpr uint32_t LocalPlayer = 0x00E8;          // 本地玩家信息偏移
constexpr uint32_t PlayerController = 0x0030;     // 玩家控制器偏移
constexpr uint32_t Actor = 0xB0;                  // 游戏中角色偏移
constexpr uint32_t GNames = 0x0A7DDE90;           // 游戏中使用的名称地址
constexpr uint32_t ObjID = 0x10;                  // 对象 ID 偏移
constexpr uint32_t AcknowledgedPawn = 0x4B0;      // 本人组地址
constexpr uint32_t PlayerCameraManager = 0x04D8;  // 相机地址
constexpr uint32_t ViewTarget = 0x0420;           // 当前镜头

// Decrypt
constexpr uint32_t Decrypt = 0x8B88728;   // 用于解密操作偏移
constexpr uint32_t ChunkSize = 0x4030;    // 内存块的大小 IDD
constexpr uint32_t IsRor = 0x1;           // 是否使用 ROR 操作进行解密的标志
constexpr uint32_t RorValue = 0x4;        // ROR 操作的值
constexpr uint32_t XorKey1 = 0xB5A001AB;  // XOR 解密的密钥1 DecryptInternalIndexXorKey1
constexpr uint32_t XorKey2 = 0xBB42C13;   // XOR 解密的密钥2 DecryptInternalIndexXorKey2

// Game
constexpr uint32_t WorldToMap = 0x9B4;    // 世界坐标到地图坐标的转换参数
constexpr uint32_t PlayerArray = 0x0418;  // 玩家数组偏移

// Project
constexpr uint32_t RootComponent = 0x2A0;            // 根组件偏移
constexpr uint32_t ComponentLocation = 0x200;        // 组件位置偏移
constexpr uint32_t TimeTillExplosion = 0x7AC;        // 投掷物爆炸时间
constexpr uint32_t DroppedItem = 0x448;              // 物品组件
constexpr uint32_t ItemInformationComponent = 0xA8;  // 物品一
constexpr uint32_t DroppedItemGroup = 0x2E8;         // 其他物品

// Player
constexpr uint32_t PlayerName = 0x0460;         // 玩家姓名偏移
constexpr uint32_t AccountId = 0x0498;          // 玩家账户 ID 偏移
constexpr uint32_t PubgIdData = 0xE70;          // PUBG ID 数据偏移
constexpr uint32_t PlayerState = 0x0438;        // 玩家状态偏移
constexpr uint32_t TeamNumber = 0x04C0;         // 玩家所在队伍编号偏移
constexpr uint32_t CharacterName = 0x1350;      // 角色名字偏移
constexpr uint32_t Health = 0x09A0;             // 玩家生命值偏移
constexpr uint32_t HealthMax = 0x09D4;          // 正常血量上限
constexpr uint32_t GroggyHealth = 0x1D70;       // 玩家濒死状态生命值偏移
constexpr uint32_t GroggyHealthMax = 0x1280;    // 倒地血量上限
constexpr uint32_t CharacterState = 0x1320;     // 玩家角色状态偏移
constexpr uint32_t SpectatedCount = 0x1E90;     // 观察的数量
constexpr uint32_t WeaponProcessor = 0x0900;    // 武器处理器偏移
constexpr uint32_t ComponentToWorld = 0x1F0;    // 组件转世界坐标偏移
constexpr uint32_t StaticMesh = 0x0AC0;         // 静态网格偏移
constexpr uint32_t PlayerSatisitc = 0x5F8;      // 玩家统计数据偏移 //PlayerStatistics
constexpr uint32_t DamageDealtOnEnemy = 0x7dc;  // 对敌人造成的伤害值偏移
constexpr uint32_t SurvivalTier = 0x0000;       // 存活层级偏移
constexpr uint32_t SurvivalLevel = 0x0004;      // 存活等级偏移
constexpr uint32_t PartnerLevel = 0x0BDE;       // 伙伴等级偏移
constexpr uint32_t EquippedWeapons = 0x01F8;    // 已装备武器偏移
constexpr uint32_t CurrentWeaponIndex = 0x301;  // 当前武器索引偏移
constexpr uint32_t AimOffsets = 0x17D0;         // 被瞄偏移 取Y

// Map
constexpr uint32_t WidgetStateMap = 0x0508;
constexpr uint32_t MapWidget = 0x700;  //
constexpr uint32_t TrainingMapGrid = 0x06B8;
constexpr uint32_t Visibility = 0x00A1;
constexpr uint32_t Slot = 0x0030;
constexpr uint32_t LayoutData = 0x0038;
constexpr uint32_t Alignment = 0x0020;
constexpr uint32_t BlockInputWidgetList = 0x0570;

// Myself
constexpr uint32_t VehicleRiderComponent = 0x1FC0;
constexpr uint32_t Mesh = 0x0528;  // 玩家模型偏移
constexpr uint32_t AnimScriptInstance = 0x0C60;
constexpr uint32_t Playname = 0x1350;  // CharacterName
constexpr uint32_t SeatIndex = 0x0220;
constexpr uint32_t LastTeamNum = 0x1510;

// skeleton
constexpr uint32_t Bone = 0xAC0;       // StaticMesh
constexpr uint32_t BoneArry = 0x1F0;   // ComponentToWorld
constexpr uint32_t FOV = 0x15A8;       // 视野参数偏移
constexpr uint32_t Rotation = 0x1004;  // 玩家旋转参数偏移
constexpr uint32_t Location = 0x1594;  // 玩家位置参数偏移

//
constexpr uint32_t IsABot = 0x44c;        //
constexpr uint32_t IsSpectator = 0x410;   //
constexpr uint32_t AlivePlayers = 0x498;  //
constexpr uint32_t AliveTeams = 0x4DC;    //

constexpr uint32_t CharacterClanInfo = 0x0C28;

constexpr uint32_t WeaponTrajectoryData = 0x1038;
constexpr uint32_t TrajectoryConfig = 0x0108;
constexpr uint32_t BallisticCurve = 0x0028;
constexpr uint32_t FloatCurves = 0x0038;
constexpr uint32_t Keys = 0x60;

constexpr uint32_t LastVehiclePawn = 0x0270;
constexpr uint32_t ReplicatedMovement = 0x0060;

constexpr uint32_t Veloctity = 0x3D0;

constexpr uint32_t ControlRotation_CP = 0x07FC;
constexpr uint32_t RecoilADSRotation_CP = 0x09FC;
constexpr uint32_t ComponentVelocity = 0x0234;

constexpr uint32_t DroppedItemGroupCount = 0x0728;
constexpr uint32_t DroppedItemGroupUItem = 0x0728;
constexpr uint32_t ItemID = 0x0268;  // 物品二

}  // namespace Offset