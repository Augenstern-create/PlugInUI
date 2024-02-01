#pragma once
#include <cstdint>

namespace Offset {
// world
constexpr uint32_t UWorld = 0x0A5D6488;           // 游戏世界偏移
constexpr uint32_t CurrentLevel = 0x0158;         // 当前关卡偏移 Leve
constexpr uint32_t GameInstance = 0x0168;         // 游戏实例偏移 GameInstence
constexpr uint32_t GameState = 0x0118;            // 游戏状态偏移
constexpr uint32_t LocalPlayer = 0x0048;          // 本地玩家信息偏移
constexpr uint32_t PlayerController = 0x0038;     // 玩家控制器偏移
constexpr uint32_t Actor = 0x0038;                // 游戏中角色偏移
constexpr uint32_t GNames = 0x0A7E2100;           // 游戏中使用的名称地址
constexpr uint32_t ObjID = 0x0018;                // 对象 ID 偏移
constexpr uint32_t AcknowledgedPawn = 0x04B0;     // 本人组地址
constexpr uint32_t PlayerCameraManager = 0x04D8;  // 相机地址
constexpr uint32_t ViewTarget = 0x0480;           // 当前镜头

// Decrypt
constexpr uint32_t Decrypt = 0x08B71928;  // 用于解密操作偏移 Shield指针
constexpr uint32_t ChunkSize = 0x3EF8;    // 内存块的大小 IDD
constexpr uint32_t IsRor = 0x1;           // 是否使用 ROR 操作进行解密的标志
constexpr uint32_t RorValue = 0xF;        // ROR 操作的值
constexpr uint32_t XorKey1 = 0x8EE400B7;  // XOR 解密的密钥1 DecryptInternalIndexXorKey1
constexpr uint32_t XorKey2 = 0x1F0E061B;  // XOR 解密的密钥2 DecryptInternalIndexXorKey2

// Game
constexpr uint32_t WorldToMap = 0x09C4;   // 世界坐标到地图坐标的转换参数 世界转小地图
constexpr uint32_t PlayerArray = 0x0418;  // 玩家数组偏移

// Project
constexpr uint32_t RootComponent = 0x02F0;           // 根组件偏移
constexpr uint32_t ComponentLocation = 0x0250;       // 组件位置偏移 //
constexpr uint32_t TimeTillExplosion = 0x07AC;       // 投掷物爆炸时间
constexpr uint32_t DroppedItem = 0x0448;             // 物品组件
constexpr uint32_t ItemInformationComponent = 0xB0;  // 物品一
constexpr uint32_t DroppedItemGroup = 0x0260;        // 其他物品

// Player
constexpr uint32_t PlayerName = 0x0468;          // 玩家姓名偏移
constexpr uint32_t AccountId = 0x0498;           // 玩家账户 ID 偏移
constexpr uint32_t PubgIdData = 0x0E70;          // PUBG ID 数据偏移
constexpr uint32_t PlayerState = 0x0430;         // 玩家状态偏移
constexpr uint32_t TeamNumber = 0x0A6C;          // 玩家所在队伍编号偏移
constexpr uint32_t CharacterName = 0x1EB8;       // 角色名字偏移 //
constexpr uint32_t Health = 0x09B0;              // 玩家生命值偏移
constexpr uint32_t HealthMax = 0x09B4;           // 正常血量上限
constexpr uint32_t GroggyHealth = 0x2D90;        // 玩家濒死状态生命值偏移
constexpr uint32_t GroggyHealthMax = 0x128C;     // 倒地血量上限
constexpr uint32_t CharacterState = 0x2780;      // 玩家角色状态偏移
constexpr uint32_t SpectatedCount = 0x0F90;      // 观察的数量 //
constexpr uint32_t WeaponProcessor = 0x0910;     // 武器处理器偏移
constexpr uint32_t ComponentToWorld = 0x0240;    // 组件转世界坐标偏移 //
constexpr uint32_t StaticMesh = 0x0AD0;          // 静态网格偏移
constexpr uint32_t PlayerSatisitc = 0x0A60;      // 玩家统计数据偏移 //PlayerStatistics
constexpr uint32_t DamageDealtOnEnemy = 0x07E8;  // 对敌人造成的伤害值偏移
constexpr uint32_t SurvivalTier = 0x0020;        // 存活层级偏移 //
constexpr uint32_t SurvivalLevel = 0x0024;       // 存活等级偏移
constexpr uint32_t PartnerLevel = 0x0636;        // 伙伴等级偏移
constexpr uint32_t EquippedWeapons = 0x0208;     // 已装备武器偏移
constexpr uint32_t CurrentWeaponIndex = 0x0311;  // 当前武器索引偏移
constexpr uint32_t AimOffsets = 0x18A0;          // 被瞄偏移 取Y //

// MapVisibility
constexpr uint32_t WidgetStateMap = 0x0508;
constexpr uint32_t MapWidget = 0x708;
constexpr uint32_t TrainingMapGrid = 0x06D8;
constexpr uint32_t Visibility = 0x00A9;
constexpr uint32_t Slot = 0x0038;
constexpr uint32_t LayoutData = 0x0040;
constexpr uint32_t Alignment = 0x0020;  //
constexpr uint32_t BlockInputWidgetList = 0x0570;

// Myself
constexpr uint32_t VehicleRiderComponent = 0x1FE0;
constexpr uint32_t Mesh = 0x0508;  // 玩家模型偏移
constexpr uint32_t AnimScriptInstance = 0x0C70;
constexpr uint32_t Playname = 0x1EB8;  //
constexpr uint32_t SeatIndex = 0x0230;
constexpr uint32_t LastTeamNum = 0x1190;  //

// skeleton
constexpr uint32_t Bone = 0x0AD0;      // StaticMesh
constexpr uint32_t BoneArry = 0x0240;  // ComponentToWorld
constexpr uint32_t FOV = 0x1024;       // 视野参数偏移
constexpr uint32_t Rotation = 0x0A70;  // 玩家旋转参数偏移
constexpr uint32_t Location = 0x0A98;  // 玩家位置参数偏移

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