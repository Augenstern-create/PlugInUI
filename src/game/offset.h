#pragma once
#include <cstdint>

namespace Offset {
// world
constexpr uintptr_t UWorld = 0x9D80918;           // 游戏世界偏移
constexpr uint32_t CurrentLevel = 0x0158;         // 当前关卡偏移
constexpr uint32_t GameInstance = 0x0160;         // 游戏实例偏移
constexpr uint32_t GameState = 0x0118;            // 游戏状态偏移
constexpr uint32_t LocalPlayer = 0x0038;          // 本地玩家信息偏移
constexpr uint32_t PlayerController = 0x0038;     // 玩家控制器偏移
constexpr uint32_t Actor = 0x01E8;                // 游戏中角色偏移
constexpr uint32_t ActorArray = 0x01E8;           // 角色数组偏移
constexpr uintptr_t GNames = 0x09F8CB48;          // 游戏中使用的名称地址
constexpr uint32_t ObjID = 0x0010;                // 对象 ID 偏移
constexpr uint32_t AcknowledgedPawn = 0x0480;     // 本人组地址
constexpr uint32_t PlayerCameraManager = 0x04A0;  // 相机地址
constexpr uint32_t ViewTarget = 0x1650;           // 当前镜头

// Decrypt
constexpr uintptr_t Decrypt = 0x0862E328;  // 用于解密操作偏移
constexpr uint32_t ChunkSize = 0x041A8;    // 内存块的大小 IDD
constexpr uintptr_t IsRor = 0x1;           // 是否使用 ROR 操作进行解密的标志
constexpr uintptr_t RorValue = 0x0;        // ROR 操作的值
constexpr uintptr_t XorKey1 = 0x92A6C3B6;  // XOR 解密的密钥1
constexpr uintptr_t XorKey2 = 0x10511EBC;  // XOR 解密的密钥2

// Game
constexpr uint32_t WorldToMap = 0x09B4;   // 世界坐标到地图坐标的转换参数
constexpr uint32_t PlayerArray = 0x0408;  // 玩家数组偏移

// Player
constexpr uint32_t PlayerName = 0x0428;          // 玩家姓名偏移
constexpr uint32_t AccountId = 0x0A60;           // 玩家账户 ID 偏移
constexpr uint32_t PlayerTeamId = 0x0A84;        // 玩家所在队伍 ID 偏移
constexpr uint32_t PubgIdData = 0x0E60;          // PUBG ID 数据偏移
constexpr uint32_t RootComponent = 0x01B0;       // 玩家根组件偏移
constexpr uint32_t PlayerState = 0x0428;         // 玩家状态偏移
constexpr uint32_t TeamNumber = 0x0A84;          // 玩家所在队伍编号偏移
constexpr uint32_t CharacterName = 0x1C60;       // 角色名字偏移
constexpr uint32_t Health = 0x0980;              // 玩家生命值偏移
constexpr uint32_t GroggyHealth = 0x1470;        // 玩家濒死状态生命值偏移
constexpr uint32_t CharacterState = 0x1629;      // 玩家角色状态偏移
constexpr uint32_t SpectatedCount = 0x14C4;      // 观察的数量
constexpr uint32_t WeaponProcessor = 0x08E0;     // 武器处理器偏移
constexpr uint32_t ComponentLocation = 0x02C0;   // 组件位置偏移
constexpr uint32_t ComponentToWorld = 0x02B0;    // 组件转世界坐标偏移
constexpr uint32_t StaticMesh = 0x0AC0;          // 静态网格偏移
constexpr uint32_t PlayerSatisitc = 0x7C4;       // 玩家统计数据偏移
constexpr uint32_t DamageDealtOnEnemy = 0x07A8;  // 对敌人造成的伤害值偏移
constexpr uint32_t SurvivalTier = 0x0000;        // 存活层级偏移
constexpr uint32_t SurvivalLevel = 0x0004;       // 存活等级偏移
constexpr uint32_t PartnerLevel = 0x060E;        // 伙伴等级偏移
constexpr uint32_t EquippedWeapons = 0x0208;     // 已装备武器偏移
constexpr uint32_t CurrentWeaponIndex = 0x0311;  // 当前武器索引偏移
constexpr uint32_t FOV = 0x0028;                 // 视野参数偏移
constexpr uint32_t Rotation = 0x002C;            // 玩家旋转参数偏移
constexpr uint32_t Location = 0x000C;            // 玩家位置参数偏移
constexpr uint32_t AimOffsets = 0x18D0;          // 被瞄偏移 取Y

// Map
constexpr uint32_t WidgetStateMap = 0x04F8;
constexpr uint32_t MapWidget = 0x6F0;
constexpr uint32_t TrainingMapGrid = 0x06C0;
constexpr uint32_t Visibility = 0x00A9;
constexpr uint32_t Slot = 0x0038;
constexpr uint32_t LayoutData = 0x0040;
constexpr uint32_t Alignment = 0x0020;
constexpr uint32_t BlockInputWidgetList = 0x0560;

// Myself
constexpr uint32_t VehicleRiderComponent = 0x1F80;
constexpr uint32_t Mesh = 0x04B8;  // 玩家模型偏移
constexpr uint32_t AnimScriptInstance = 0x0C60;
constexpr uint32_t Playname = 0x1C60;
constexpr uint32_t SeatIndex = 0x0230;
constexpr uint32_t LastTeamNum = 0x1CD0;

//
constexpr uint32_t IsABot = 0x44c;        //
constexpr uint32_t IsSpectator = 0x410;   //
constexpr uint32_t AlivePlayers = 0x498;  //
constexpr uint32_t AliveTeams = 0x4DC;    //

constexpr uint32_t CharacterClanInfo = 0x0C28;
constexpr uint32_t HealthMax = 0x09BC;
constexpr uint32_t GroggyHealthMax = 0x12B8;

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
constexpr uint32_t DroppedItem = 0x0440;
constexpr uint32_t DroppedItemGroup = 0x0390;
constexpr uint32_t DroppedItemGroupCount = 0x0728;
constexpr uint32_t DroppedItemGroupUItem = 0x0728;
constexpr uint32_t ItemID = 0x0268;                  // 物品二
constexpr uint32_t ItemInformationComponent = 0xA8;  // 物品一 //

constexpr uint32_t TimeTillExplosion = 0x07AC;

}  // namespace Offset