#include "Include.h"

#include "Hack.h"
#include "GNames.h"
#include "Process.h"
#include "Actors.h"
#include "SrcGame.h"
#include "srcUe4math.h"
#include "SrcUtils.h"
#include "Radar.h"
#include "VmmCore.h"
#include "vmmdll.h"
#include <unordered_set>
#include <cfloat>
#include <cmath>

// #include "gui/OverlayWindow.h"

#include <thread>
#include <iostream>
#include <algorithm>
#include <omp.h>
#include <chrono>

#define MEASURE_TIME(file, line, code)                                                                                                     \
    do {                                                                                                                                   \
        auto start_time = std::chrono::high_resolution_clock::now();                                                                       \
        code;                                                                                                                              \
        auto end_time = std::chrono::high_resolution_clock::now();                                                                         \
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);                                      \
        if (duration.count() >= 10)                                                                                                        \
            std::cout << "File: " << file << ", Line: " << line << " - Time taken: " << duration.count() << " milliseconds." << std::endl; \
    } while (false)

template <typename T>
T Clamping(T value, T min, T max) {
    return value <= min ? min : (value >= max ? max : value);
}

FVector GetVihecleSpeed(ULONG64 ACharacter) {
    ULONG64 VehicleRiderComponent = VmmCore::ReadValue<DWORD_PTR>(ACharacter + Offset::VehicleRiderComponent);

    int Seatindex = VmmCore::ReadValue<int>(VehicleRiderComponent + Offset::SeatIndex);

    if (Seatindex != -1) {
        ULONG64 MovementComponent = VmmCore::ReadValue<DWORD_PTR>(VehicleRiderComponent + Offset::LastVehiclePawn);
        return VmmCore::ReadValue<FVector>(MovementComponent + Offset::ReplicatedMovement);
    } else {
        return FVector(0, 0, 0);
    }
}

FVector GetPredictPlayerLocation(FVector EntityLocation, FVector EntitySpeed, FLOAT Distance, FVector WeaponTrajectory) {
    FLOAT BulletFlightTime = FLOAT();

    FLOAT FlyRatio = FLOAT();

    FLOAT tmp = (WeaponTrajectory.X / 1000.f);

    FlyRatio = (FLOAT)(WeaponTrajectory.Y / tmp);

    BulletFlightTime = (Distance / WeaponTrajectory.Y) * FlyRatio;

    return FVector(EntityLocation.X + EntitySpeed.X * BulletFlightTime, EntityLocation.Y + EntitySpeed.Y * BulletFlightTime,
                   EntityLocation.Z + (0.5f * (Distance * WeaponTrajectory.Y) * powf(BulletFlightTime, 2)));
}

float GetBulletDrop(float step, FWeaponTrajectoryConfig CurrentWeaponConfig) {
    float TGZ = 9.8f;
    float SST = CurrentWeaponConfig.SimulationSubstepTime;
    float VDC = CurrentWeaponConfig.VDragCoefficient;
    float BDS = CurrentWeaponConfig.BDS;
    float STS = 100.f;
    if (BDS <= 0.0 || SST <= 0.0f || VDC <= 0.0f) {
        BDS = 1.0f;
        VDC = 0.75f;
        SST = 0.016f;
    }
    float DP = 0.0f;
    for (float x = 0; x < step; x += SST) {
        DP = ((((TGZ * SST) * STS) * VDC) * BDS) + DP;
    }
    return DP;
}

void AimBot() {
    const double targetFrameTime = 1.0 / 240.0;
    while (true) {
        auto startTime = std::chrono::high_resolution_clock::now();
        if (!gameData.AimBot.TargetEntity) continue;
        auto key = VK_XBUTTON2;
        // KeyState::Get(key) ||
        if (false) {
            gameData.AimBot.Lock = true;

            std::vector<PlayerInfo> players = Data::GetPlayers();

            PlayerInfo targetPlayer, localPlayer;

            for (PlayerInfo player : players) {
                if (player.Entity == gameData.AimBot.TargetEntity) {
                    targetPlayer = player;
                }
                if (player.Entity == gameData.LocalPlayerPawn) {
                    localPlayer = player;
                }
            }

            //     DWORD_PTR WeaponData = VmmCore::ReadValue<DWORD_PTR>(localPlayer.CurrentWeapon + Offset::WeaponTrajectoryData);
            //     FWeaponTrajectoryConfig TrajectoryConfig = VmmCore::ReadValue<FWeaponTrajectoryConfig>(WeaponData + Offset::TrajectoryConfig);
            //     FRichCurve RichCurve = VmmCore::ReadValue<FRichCurve>(TrajectoryConfig.BallisticCurve + Offset::FloatCurves + Offset::Keys);

            //     std::vector<FRichCurveKey> Keys;
            //     DWORD_PTR KeysArray = VmmCore::ReadValue<DWORD_PTR>(WeaponData + Offset::BallisticCurve + Offset::FloatCurves + Offset::Keys);
            //     auto KeysArrayCount = VmmCore::ReadValue<ULONG32>(TrajectoryConfig.BallisticCurve + Offset::FloatCurves + Offset::Keys + 8);

            //     Utils::Log(2, "TrajectoryConfig.BallisticCurve: 0x%11x", TrajectoryConfig.BallisticCurve);

            //     for (auto i = 0; i < KeysArrayCount; i++) {
            //         FRichCurveKey RichCurveKey = Read<FRichCurveKey>(KeysArray + (0x1C * i));
            //         Keys.push_back(RichCurveKey);
            //     }
            //     // Reading keys (I don't know why but I can't read TArray from UE4)
            //     float TimeScale = 0.0f;
            //     Vector3 TargetActorS = VmmCore::ReadValue<Vector3>(targetPlayer.Entity + Offset::Veloctity);
            //     float flyTime = targetPlayer.Distance / TrajectoryConfig.InitialSpeed;
            //     Vector3 PredictedPos = targetPlayer.Skeleton.LocationBones[EBoneIndex::forehead] + TargetActorS * flyTime;
            //     flyTime /= 0, 1;
            //     if (TrajectoryConfig.InitialSpeed > 0) {
            //         PredictedPos.z = PredictedPos.z + (GetBulletDrop(flyTime / 10.0, TrajectoryConfig) - (targetPlayer.Distance >= 320.0f ? 0 : 15.6f) +
            //                                            (TargetActorS.z * flyTime * 0.0360f));

            //         Utils::Log(2, "Distance: %f, TrajectoryConfig: %f", targetPlayer.Distance, PredictedPos.z);
            //     }
            //     Utils::Log(2, "TrajectoryConfig.BallisticCurve: 0x%11x", TrajectoryConfig.BallisticCurve);
            //     Utils::Log(2, "RichCurve: %f", RichCurve.DefaultValue);
            //     Utils::Log(2, "RichCurve: %d", KeysArrayCount);

            //     auto WeaponProcessor = VmmCore::ReadValue<DWORD_PTR>(gameData.LocalPlayerPawn + Offset::WeaponProcessor);

            //     auto EquippedWeapons = VmmCore::ReadValue<DWORD_PTR>(WeaponProcessor + Offset::EquippedWeapons);

            //     auto CurrentWeaponIndex = VmmCore::ReadValue<BYTE>(WeaponProcessor + Offset::CurrentWeaponIndex);
            //     if (CurrentWeaponIndex >= 0 && CurrentWeaponIndex < 8) {
            //         auto pWeap = VmmCore::ReadValue<DWORD_PTR>(EquippedWeapons + CurrentWeaponIndex * 8);
            //         if (pWeap > 0) {
            //             ULONG CurrentWeaponID = Decrypt::CIndex(VmmCore::ReadValue<DWORD_PTR>(pWeap + Offset::ObjID));
            //             DWORD_PTR WeaponData = VmmCore::ReadValue<DWORD_PTR>(pWeap + Offset::WeaponTrajectoryData);
            //             FWeaponTrajectoryConfig TrajectoryConfig = VmmCore::ReadValue<FWeaponTrajectoryConfig>(WeaponData + Offset::TrajectoryConfig);
            //             float Gravity = VmmCore::ReadValue<float>(WeaponData + Offset::TrajectoryConfig + 0x18);

            //             FVector velocity{};
            //             velocity = GetVihecleSpeed(player.Entity);
            //             if (velocity.X == 0.f && velocity.Y == 0.f && velocity.Z == 0.f) {
            //                 velocity = VmmCore::ReadValue<FVector>(player.RootComponent + Offset::ComponentVelocity);
            //             }

            //             FVector WeaponTrajectory{};
            //             WeaponTrajectory.X = TrajectoryConfig.InitialSpeed;

            //             WeaponTrajectory.Y = TrajectoryConfig.WeaponSpread;

            //             WeaponTrajectory.Y = Gravity;

            //             auto pos = player.Skeleton.LocationBones[EBoneIndex::forehead];
            //             Vector3 screenPos;
            //             float dist;
            //             WorldToScreen(pos, &screenPos, &dist);
            //             FVector ObjePos = GetPredictPlayerLocation(FVector(pos.x, pos.y, pos.z), velocity, 1, WeaponTrajectory);
            //             Vector3 aimPOS = WorldToScreenAim(Vector3(ObjePos.X, ObjePos.Y, ObjePos.Z));
            //             Utils::Log(2, "AimBot Target: %s | %f | %f | %f | %f", player.Name, dist, aimPOS.x, aimPOS.y, screenPos.z);
            //             KmBox::Move(aimPOS.x, aimPOS.y);
            //             Utils::Log(2, "AimBot Target: %s | %d | %f | %f", player.Name, CurrentWeaponID, TrajectoryConfig.InitialSpeed, Gravity);
            //         }
            //     }
            // } else {
            //     gameData.AimBot.Lock = false;
            // }

            // auto endTime = std::chrono::high_resolution_clock::now();
            // std::chrono::duration<double> frameTime = endTime - startTime;

            // double delayTime = targetFrameTime - frameTime.count();
            // if (delayTime > 0.0) {
            //     std::this_thread::sleep_for(std::chrono::duration<double>(delayTime));
        }
    }
}

// bool IsLobby(const std::string& mapName) {
//     return (mapName.find("fail") != std::string::npos || mapName.find("None") != std::string::npos || mapName.find("TslLobby") != std::string::npos);
// }

bool JudgeOneself() {
    int is = VmmCore::ReadValue<int>(VmmCore::ReadValue<DWORD_PTR>(gameData.ViewTarget + Offset::VehicleRiderComponent) + Offset::SeatIndex);
    if (is == -1 || is == 1 || is == 2 || is == 3 || is == 4 || is == 5) {
        return true;
    }
    return false;
}

void UpdateAddress() {
    while (true) {
        if (gameData.PID > 0) {
            DWORD_PTR GNames = Decrypt::Xe(VmmCore::ReadValue<DWORD_PTR>(gameData.GameBase + Offset::GNames));// 140697243680768 | 206938192 | 177578792
            gameData.GNames = Decrypt::Xe(VmmCore::ReadValue<DWORD_PTR>(GNames));
            gameData.UWorld = Decrypt::Xe(VmmCore::ReadValue<DWORD_PTR>(gameData.GameBase + Offset::UWorld));
            gameData.GameState = Decrypt::Xe(VmmCore::ReadValue<DWORD_PTR>(gameData.UWorld + Offset::GameState));
            gameData.CurrentLevel = Decrypt::Xe(VmmCore::ReadValue<DWORD_PTR>(gameData.UWorld + Offset::CurrentLevel));
            gameData.GameInstance = Decrypt::Xe(VmmCore::ReadValue<DWORD_PTR>(gameData.UWorld + Offset::GameInstance));
            gameData.LocalPlayer = Decrypt::Xe(VmmCore::ReadValue<DWORD_PTR>(VmmCore::ReadValue<DWORD_PTR>(gameData.GameInstance + Offset::LocalPlayer)));
            gameData.PlayerController = Decrypt::Xe(VmmCore::ReadValue<DWORD_PTR>(gameData.LocalPlayer + Offset::PlayerController));
            gameData.AcknowledgedPawn = Decrypt::Xe(VmmCore::ReadValue<DWORD_PTR>(gameData.PlayerController + Offset::AcknowledgedPawn));
            gameData.PlayerCameraManager = VmmCore::ReadValue<DWORD_PTR>(gameData.PlayerController + Offset::PlayerCameraManager);
            gameData.ViewTarget = VmmCore::ReadValue<DWORD_PTR>(gameData.PlayerCameraManager + Offset::ViewTarget);
            gameData.MyHUD = VmmCore::ReadValue<DWORD_PTR>(gameData.PlayerController + Offset::MyHUD);
            UpdateGameScene();

            gameData.Actor = Decrypt::Xe(VmmCore::ReadValue<DWORD_PTR>(gameData.CurrentLevel + Offset::Actor));

            if (gameData.Scene == Scene::Gameing) {
                gameData.PlayerCount = VmmCore::ReadValue<int>(gameData.GameState + Offset::PlayerArray + 0x8);
                if (JudgeOneself()) {
                    DWORD_PTR NamePtr = VmmCore::ReadValue<DWORD_PTR>(gameData.ViewTarget + Offset::Playname);
                    if (!Utils::ValidPtr(NamePtr)) {
                        FText NameText = VmmCore::ReadValue<FText>(NamePtr);
                        std::string Name = Utils::UnicodeToAnsi(NameText.buffer);
                        gameData.Myself.ClanName = Utils::AnalyzeClanName(Name);
                        gameData.Myself.Name = Utils::AnalyzeName(Name);
                        if (gameData.Myself.Name == "") {
                            gameData.Myself.Name = "Driver";
                        } else {
                            gameData.Myself.isGetPlayer = true;
                        }
                        gameData.Myself.Mesh = VmmCore::ReadValue<DWORD_PTR>(gameData.ViewTarget + Offset::Mesh);
                        gameData.Myself.AnimScript = VmmCore::ReadValue<DWORD_PTR>(gameData.Myself.Mesh + Offset::AnimScriptInstance);
                        gameData.Myself.TeamID = VmmCore::ReadValue<int>(gameData.ViewTarget + Offset::LastTeamNum);
                        gameData.Myself.TeamID = gameData.Myself.TeamID >= 100000 ? gameData.Myself.TeamID - 100000 : gameData.Myself.TeamID;
                    }
                }
                gameData.mapRadar.world_location = Vector3(VmmCore::ReadValue<int>(gameData.UWorld + Offset::WorldToMap),
                                                           VmmCore::ReadValue<int>(gameData.UWorld + Offset::WorldToMap + 0x4), 0.0f);
                {
                    // radar
                    if (gameData.mapRadar.map_size <= 0) {
                        if (!Radar::GetMap()) {
                            Sleep(10);
                            continue;
                        }
                    }
                    if (Utils::ValidPtr(gameData.mapRadar.small_map_radar)) {
                        if (!Radar::GetMiniMap()) {
                        }
                    }
                    if (Utils::ValidPtr(gameData.mapRadar.map_grid)) {
                        if (!Radar::GetMapGrid()) {
                        }
                    }

                    Radar::GetVisibility();
                    Radar::GetMinMapVisibility();
                    Radar::GetZoomFactor();
                    Radar::GetPosition();
                }
            }
        }
    }
}

void UpdateEntitys() {
    while (true) {
        if (gameData.Scene != Scene::Gameing) continue;
        DWORD_PTR Actor;
        int ActorCount;
        VmmCore::ScatterReadEx(0, gameData.Actor, (DWORD_PTR*)&Actor);
        VmmCore::ScatterReadEx(0, gameData.Actor + 0x8, (int*)&ActorCount);
        VmmCore::ScatterExecuteReadEx(0);
        gameData.ActorCount = ActorCount;
        if (ActorCount <= 0 || ActorCount > 80000) {
            continue;
        }

        std::vector<DWORD_PTR> entitys(ActorCount);
        auto CacheEntity = Data::GetCacheEntity();

        for (int i = 0; i < ActorCount; i++) {
            DWORD_PTR Actors = (Actor + (i * 0x8));
            VmmCore::ScatterReadEx(0, Actors, (DWORD_PTR*)&entitys[i]);
        }
        VmmCore::ScatterExecuteReadEx(0);

        entitys = GetEntitys((Actor));
        for (auto entity : entitys) {
            if (entity == 0) continue;
            if ((std::find_if(CacheEntity.begin(), CacheEntity.end(), [entity](const ActorEntityInfo& info) { return info.Entity == entity; })) ==
                CacheEntity.end()) {
                CacheEntity.push_back({entity, 0, 0, EntityType::Unknown});
            }
        }

        for (int i = 0; i < CacheEntity.size(); i++) {
            if (CacheEntity[i].ID == 0) {
                VmmCore::ScatterReadEx(0, CacheEntity[i].Entity + Offset::ObjID, (int*)&(CacheEntity[i].ID));
            }
        }
        VmmCore::ScatterExecuteReadEx(0);
        for (auto& entity : CacheEntity) {
            if (entity.decodeID == 0 && entity.ID != 0) {
                auto decodeID = Decrypt::CIndex(entity.ID);
                entity.decodeID = decodeID;
                entity.type = EntityType::Unknown;
            }
        }
        // std::vector<ActorEntityInfo> gEntitys;
        // for (auto entity : entitys) {
        //     if (entity == 0) continue;

        //     if (std::find_if(gEntitys.begin(), gEntitys.end(), [entity](const ActorEntityInfo& info) { return info.Entity == entity; }) ==
        //     gEntitys.end()) {
        //         gEntitys.push_back({entity, 0, 0, EntityType::Unknown});
        //         DWORD_PTR entityptr = entity + Offset::ObjID;
        //         SetScatterPrepare<DWORD_PTR>(entityptr);
        //     }
        // }
        // SetScatterExecute();
        // for (auto& entity : gEntitys) {
        //     int id = GetEntityID(entity.Entity + Offset::ObjID);
        //     entity.ID = id;
        //     entity.decodeID = Decrypt::CIndex(id);
        //     EntityInfo entityInfo = findEntityInfoByID(entity.decodeID);
        //     entity.type = entityInfo.Type;
        // }
        // SetScatterClear();

        std::vector<PlayerInfo> players;        // 玩家信息
        std::vector<ProjectInfo> projects;      // 投掷物品
        std::vector<ItemInfo> items;            // 掉落物品
        std::vector<ItemGroupInfo> itemGroups;  // 刷新物品
        players = Data::GetCachePlayers();
        projects = Data::GetCacheProjects();
        items = Data::GetCacheItems();
        itemGroups = Data::GetItemGroups();
        for (auto& entity : CacheEntity) {
            if (entity.decodeID == 0) continue;
            if (entity.type != EntityType::Unknown) continue;
            // EntityInfo entityInfo = findEntityInfoByID(entity.decodeID);
            EntityInfo entityInfo = GNames::GetName(entity.decodeID, 0);
            entity.type = entityInfo.Type;
            auto entityptr = entity.Entity;
            if ((entity.type == EntityType::Player || entity.type == EntityType::AI)) {
                if ((std::find_if(players.begin(), players.end(), [entityptr](const PlayerInfo& info) { return info.Entity == entityptr; })) == players.end()) {
                    players.push_back({entity.type, entity.Entity, entity.decodeID});
                }
            } else if (entity.type == EntityType::Project) {
                if ((std::find_if(projects.begin(), projects.end(), [entityptr](const ProjectInfo& info) { return info.Entity == entityptr; })) ==
                    projects.end()) {
                    ProjectInfo Project = {entity.type, entity.Entity, entity.decodeID, entityInfo.DisplayName, 0, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f},
                                           0.0f,        0.0f};
                    projects.push_back(Project);
                }
            }

            else if (entity.type == EntityType::DroppedItem) {
                if ((std::find_if(items.begin(), items.end(), [entityptr](const ItemInfo& info) { return info.Entity == entityptr; })) == items.end()) {
                    items.push_back({entity.type, entity.Entity, 0, 0, entity.decodeID, entityInfo.DisplayName});
                }
            }

            else if (entity.type == EntityType::DroppedItemGroup) {
                if ((std::find_if(itemGroups.begin(), itemGroups.end(), [entityptr](const ItemGroupInfo& info) { return info.Entity == entityptr; })) ==
                    itemGroups.end()) {
                    itemGroups.push_back({entity.type, entity.Entity, entity.decodeID});
                }
            }
        }
        Data::SetCacheEntity(CacheEntity);
        Data::SetCachePlayers(players);
        Data::SetCacheProjects(projects);
        Data::SetCacheItems(items);
        Data::SetItemGroups(itemGroups);
    }
}
void UpdateItem() {
    while (true) {
        if (gameData.Scene != Scene::Gameing) continue;

        // projects
        auto projects = Data::GetCacheProjects();  // 投掷物品
        if (projects.size() > 0) {
            // for (int i = 0; i < projects.size(); i++) {
            //     VmmCore::ScatterReadEx(4, projects[i].Entity + Offset::RootComponent, (DWORD_PTR*)&(projects[i].RootComponent));
            // }
            // VmmCore::ScatterExecuteReadEx(4);
            // for (int i = 0; i < projects.size(); i++) {
            //     VmmCore::ScatterReadEx(4, projects[i].RootComponent + Offset::ComponentLocation, (Vector3*)&(projects[i].Location));
            //     VmmCore::ScatterReadEx(4, projects[i].Entity + Offset::TimeTillExplosion, (float*)&(projects[i].TimeTillExplosion));
            // }
            // VmmCore::ScatterExecuteReadEx(4);
            // for (auto& project : projects) {
            //     project.Distance = gameData.Myself.Location.Distance(project.Location) / 100.0f;
            //     project.ScreenLocation = VectorHelper::WorldToScreen(project.Location);
            // }
        }
        Data::SetProjects(projects);
        // items
        auto items = Data::GetCacheItems();
        if (items.size() > 0) {
            // for (int i = 0; i < items.size(); i++) {
            //     VmmCore::ScatterReadEx(4, items[i].Entity + Offset::DroppedItem, (DWORD_PTR*)&(items[i].DroppedItem));
            //     VmmCore::ScatterReadEx(4, items[i].Entity + Offset::RootComponent, (DWORD_PTR*)&(items[i].RootComponent));
            // }
            // VmmCore::ScatterExecuteReadEx(4);
            // for (int i = 0; i < items.size(); i++) {
            //     VmmCore::ScatterReadEx(4, items[i].DroppedItem + Offset::ItemInformationComponent, (DWORD_PTR*)&(items[i].InformationComponent));
            //     VmmCore::ScatterReadEx(4, items[i].RootComponent + Offset::ComponentLocation, (Vector3*)&(items[i].Location));
            // }
            // VmmCore::ScatterExecuteReadEx(4);
            // for (auto& item : items) {
            //     item.Distance = gameData.Myself.Location.Distance(item.Location) / 100.0f;
            //     item.ScreenLocation = VectorHelper::WorldToScreen(item.Location);
            // }
        }
        Data::SetItems(items);
        // itemGroups
        auto itemGroups = Data::GetItemGroups();
        if (itemGroups.size() > 0) {
            // for (int i = 0; i < itemGroups.size(); i++) {
            //     VmmCore::ScatterReadEx(4, itemGroups[i].Entity + Offset::DroppedItemGroup, (DWORD_PTR*)&(itemGroups[i].Items));
            // }
            // VmmCore::ScatterExecuteReadEx(4);
        }
        Data::SetItemGroups(itemGroups);
    }
}

void UpdatePlayers() {
    while (true) {
        if (gameData.Scene != Scene::Gameing) continue;
        std::vector<PlayerInfo> players = Data::GetCachePlayers();
        if (players.size() == 0) continue;
        for (PlayerInfo& player : players) {
            VmmCore::ScatterReadEx(2, player.Entity + Offset::CharacterName, (DWORD_PTR*)&player.pCharacterName);
            VmmCore::ScatterReadEx(2, player.Entity + Offset::Mesh, (DWORD_PTR*)&player.MeshComponent);
            VmmCore::ScatterReadEx(2, player.Entity + Offset::AimOffsets, (Vector3*)&player.AimOffsets);
            VmmCore::ScatterReadEx(2, player.Entity + Offset::LastTeamNum, (int*)&player.TeamID);
            VmmCore::ScatterReadEx(2, player.Entity + Offset::Health, (float*)&player.Health);
            VmmCore::ScatterReadEx(2, player.Entity + Offset::HealthMax, (float*)&player.HealthMax);
            VmmCore::ScatterReadEx(2, player.Entity + Offset::GroggyHealth, (float*)&player.GroggyHealth);
            VmmCore::ScatterReadEx(2, player.Entity + Offset::GroggyHealthMax, (float*)&player.GroggyHealthMax);
            VmmCore::ScatterReadEx(2, player.Entity + Offset::PlayerState, (DWORD_PTR*)&player.PlayerState);
            VmmCore::ScatterReadEx(2, player.Entity + Offset::CharacterState, (ECharacterState*)&player.CharacterState);
            VmmCore::ScatterReadEx(2, player.Entity + Offset::SpectatedCount, (int*)&player.SpectatedCount);
            VmmCore::ScatterReadEx(2, player.Entity + Offset::WeaponProcessor, (DWORD_PTR*)&player.WeaponProcessor);
        }

        VmmCore::ScatterExecuteReadEx(2);

        for (PlayerInfo& player : players) {
            player.PlayerState = Decrypt::Xe(player.PlayerState);

            VmmCore::ScatterReadEx(2, player.pCharacterName, (FText*)&player.CharacterName);
            VmmCore::ScatterReadEx(2, player.MeshComponent + Offset::ComponentLocation, (Vector3*)&player.Location);
            VmmCore::ScatterReadEx(2, player.PlayerState + Offset::PlayerSatisitc, (int*)&player.KillCount);
            VmmCore::ScatterReadEx(2, player.PlayerState + Offset::SurvivalTier, (int*)&player.SurvivalTier);
            VmmCore::ScatterReadEx(2, player.PlayerState + Offset::SurvivalLevel, (int*)&player.SurvivalLevel);
            VmmCore::ScatterReadEx(2, player.PlayerState + Offset::PartnerLevel, (EPartnerLevel*)&player.PartnerLevel);
            VmmCore::ScatterReadEx(2, player.PlayerState + Offset::DamageDealtOnEnemy, (float*)&player.DamageDealtOnEnemy);

            VmmCore::ScatterReadEx(2, player.MeshComponent + Offset::ComponentToWorld, (FTransform*)&player.ComponentToWorld);
            VmmCore::ScatterReadEx(2, player.MeshComponent + Offset::StaticMesh, (DWORD_PTR*)&player.StaticMesh);

            VmmCore::ScatterReadEx(2, player.WeaponProcessor + Offset::EquippedWeapons, (DWORD_PTR*)&player.EquippedWeapons);
            VmmCore::ScatterReadEx(2, player.WeaponProcessor + Offset::CurrentWeaponIndex, (BYTE*)&player.CurrentWeaponIndex);
        }

        VmmCore::ScatterExecuteReadEx(2);

        for (PlayerInfo& player : players) {
            if (player.CurrentWeaponIndex >= 0 && player.CurrentWeaponIndex < 8) {
                VmmCore::ScatterReadEx(2, player.EquippedWeapons + player.CurrentWeaponIndex * 8, (DWORD_PTR*)&player.CurrentWeapon);
            }
        }

        VmmCore::ScatterExecuteReadEx(2);
        for (PlayerInfo& player : players) {
            if (player.CurrentWeapon > 0) {
                VmmCore::ScatterReadEx(2, player.CurrentWeapon + Offset::ObjID, (int*)&player.WeaponID);
            }
        }

        VmmCore::ScatterExecuteReadEx(2);

        for (PlayerInfo& player : players) {
            player.WeaponID = Decrypt::CIndex(player.WeaponID);
            player.WeaponName = findNameByID(player.WeaponID);
            if (player.TeamID >= 100000) {
                player.TeamID = player.TeamID - 100000;
            }
            player.IsMyTeam = player.TeamID == gameData.Myself.TeamID;
            std::string Name = Utils::UnicodeToAnsi(player.CharacterName.buffer);
            player.Name = Utils::AnalyzeName(Name);
            if (player.Name == "") player.Name = "Driver";
            player.TeamName = Utils::AnalyzeClanName(Name);
            if (player.Name == gameData.Myself.Name && player.Name != "Driver" || player.Entity == gameData.Myself.PlayerPtr) {
                gameData.Myself.PlayerPtr = player.Entity;
                gameData.Myself.Location = player.Location;
            }
            player.Distance = gameData.Myself.Location.Distance(player.Location) / 100.0f;

            if (player.SurvivalTier > 0) player.SurvivalLevel = (player.SurvivalTier - 1) * 500 + player.SurvivalLevel;

            // if (!gameData.AimBot.Lock) {
            //     if (player.Entity != gameData.LocalPlayerPawn && player.TeamID != gameData.LocalPlayerTeamID) {
            //         float screenDistance = std::sqrt(
            //             std::pow(player.Skeleton.ScreenBones[EBoneIndex::forehead].x - gameData.ScreenCenter.X, 2) +
            //             std::pow(player.Skeleton.ScreenBones[EBoneIndex::forehead].y - gameData.ScreenCenter.Y, 2));

            //         if (screenDistance < gameData.Config.AimBot.AR.FOV) {
            //             if (screenDistance < gameData.AimBot.ScreenDistance) {
            //                 gameData.AimBot.TargetEntity = player.Entity;
            //                 gameData.AimBot.ScreenDistance = player.Distance;
            //             }
            //         }
            //     }
            // }
        }

        // std::sort(players.begin(), players.end(), [](const PlayerInfo& a, const PlayerInfo& b) { return a.Distance > b.Distance; });
        Data::SetCachePlayers2(players);
    }
}

void UpdateCamera() {
    float FOV;
    Vector3 Location;
    Vector3 Rotation;

    VmmCore::ScatterReadEx(3, gameData.PlayerCameraManager + Offset::FOV, (float*)&FOV);
    VmmCore::ScatterReadEx(3, gameData.PlayerCameraManager + Offset::Location, (Vector3*)&Location);
    VmmCore::ScatterReadEx(3, gameData.PlayerCameraManager + Offset::Rotation, (Vector3*)&Rotation);

    VmmCore::ScatterExecuteReadEx(3);

    gameData.FOV = FOV;
    gameData.Location = Location;
    gameData.Rotation = Rotation;
}

// 比较函数，用于std::sort
bool compareByDistance(const PlayerInfo& a, const PlayerInfo& b) { return a.Distance > b.Distance; }

Vector3 PlayerToRadarPosition(Vector3 position) {
    // Vector3 value;
    auto value = position + gameData.mapRadar.world_location;
    if (value.x < 0) value = {-value.x, value.y, value.z};
    if (value.y < 0) value = {value.x, -value.y, value.z};
    if (value.z < 0) value = {value.x, value.y, -value.z};
    return value;
}

void UpdateSkeleton() {
    while (true) {
        if (gameData.Scene != Scene::Gameing) continue;
        std::vector<PlayerInfo> players;
        std::vector<PlayerInfo> Cacheplayers = Data::GetCachePlayers2();

        for (PlayerInfo& player : Cacheplayers) {
            if (player.Distance > 1000.0f || player.Distance < 0.0f) {
                continue;
            } else if (player.TeamID < 0 || player.TeamID > 299) {
                continue;
            } else if (player.Entity == 0 || player.PlayerState == 0 || player.MeshComponent == 0 || player.StaticMesh == 0) {
                continue;
            } else {
                players.push_back(player);
            }
        }
        if (players.size() == 0) continue;

        for (PlayerInfo& player : players) {
            for (EBoneIndex bone : SkeletonLists::skeleton_bones) {
                VmmCore::ScatterReadEx(3, player.StaticMesh + (bone * sizeof(FTransform)), (FTransform*)&player.Skeleton.Bones[bone]);
            }
        }
        VmmCore::ScatterExecuteReadEx(3);
        UpdateCamera();
        float ImageMapSize = gameData.mapRadar.radar_size;
        for (PlayerInfo& player : players) {
            for (EBoneIndex bone : SkeletonLists::skeleton_bones) {
                player.Skeleton.LocationBones[bone] = VectorHelper::GetBoneWithRotation(player.Skeleton.Bones[bone], player.ComponentToWorld);
                player.Skeleton.ScreenBones[bone] = VectorHelper::WorldToScreen(player.Skeleton.LocationBones[bone]);
            }
            const Vector3 WorldLocation = PlayerToRadarPosition(player.Location);
            Vector3 pos = {WorldLocation.x / ImageMapSize, WorldLocation.y / ImageMapSize, WorldLocation.z};
            gameData.mapRadar.rader_players[player.Entity] = pos;
        }
        std::sort(players.begin(), players.end(), compareByDistance);

        const Vector3 WorldLocations = PlayerToRadarPosition(gameData.Myself.Location);
        gameData.Myself.AtlaseLocation = {WorldLocations.x / ImageMapSize, WorldLocations.y / ImageMapSize};
        Data::SetPlayers(players);
    }
}

void HackStart() {
    std::thread thread1(ListenGameProcessState);
    thread1.join();
    std::thread thread2(UpdateAddress);
    thread2.detach();
    std::thread thread3(UpdateEntitys);
    thread3.detach();
    std::thread thread4(UpdatePlayers);
    thread4.detach();
    std::thread thread5(UpdateSkeleton);
    thread5.detach();
    std::thread thread6(Radar::Update);
    thread6.detach();
    // std::thread thread7(UpdateItem);
    // thread7.detach();

    while (true) {
        if (gameData.Scene == Scene::Gameing) {
        } else {
            // Data::Clears();
        }
    }
}
