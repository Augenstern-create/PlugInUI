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
// #include <unordered_set>

// #include "gui/OverlayWindow.h"

#include <thread>
#include <iostream>
#include <algorithm>

// void UpdateAddress() {
//     // std::cout << "PID: " << gameData.PID << std::endl;
//     while (true) {
//         if (gameData.PID > 0) {
//             if (gameData.GameBase == 0) continue;
//             gameData.UWorld = Decrypt::Xe(VmmCore::ReadValue<DWORD_PTR>(gameData.GameBase + Offset::UWorld));
//             gameData.GameState = Decrypt::Xe(VmmCore::ReadValue<DWORD_PTR>(gameData.UWorld + Offset::GameState));
//             gameData.GameInstance = Decrypt::Xe(VmmCore::ReadValue<DWORD_PTR>(gameData.UWorld +
//             Offset::GameInstance)); gameData.LocalPlayer = Decrypt::Xe(VmmCore::ReadValue<DWORD_PTR>(
//                 VmmCore::ReadValue<DWORD_PTR>(gameData.GameInstance + Offset::LocalPlayer)));
//             gameData.PlayerController =
//                 Decrypt::Xe(VmmCore::ReadValue<DWORD_PTR>(gameData.LocalPlayer + Offset::PlayerController));
//             // gameData.LocalPlayerPawn = Decrypt::Xe(VmmCore::ReadValue<DWORD_PTR>(gameData.PlayerController +
//             // Offset::AcknowledgedPawn));

//             auto cacheLocalPlayerPawn =
//                 VmmCore::ReadValue<DWORD_PTR>(gameData.PlayerCameraManager + Offset::ViewTarget);

//             gameData.PlayerCameraManager =
//                 VmmCore::ReadValue<DWORD_PTR>(gameData.PlayerController + Offset::PlayerCameraManager);
//             gameData.MyHUD = VmmCore::ReadValue<DWORD_PTR>(gameData.PlayerController + Offset::MyHUD);

//             gameData.CurrentLevel = Decrypt::Xe(VmmCore::ReadValue<DWORD_PTR>(gameData.UWorld +
//             Offset::CurrentLevel)); gameData.ActorArray =
//                 Decrypt::Xe(VmmCore::ReadValue<DWORD_PTR>(gameData.CurrentLevel + Offset::ActorArray));

//             if (cacheLocalPlayerPawn) {
//                 int teamID = VmmCore::ReadValue<int>(cacheLocalPlayerPawn + Offset::TeamNumber);
//                 if (teamID > 0 && teamID < 150000) {
//                     gameData.LocalPlayerPawn = cacheLocalPlayerPawn;
//                     gameData.LocalPlayerTeamID = (teamID >= 100000) ? (teamID - 100000) : teamID;
//                     gameData.LocalPlayerMesh = VmmCore::ReadValue<DWORD_PTR>(gameData.LocalPlayerPawn +
//                     Offset::Mesh);
//                 }
//             }

//             gameData.GNames = GNames::GetGNamesPtr();
//             UpdateGameScene();
//             if (gameData.Scene == Scene::Gameing) {
//                 // int AlivePlayers = VmmCore::ReadValue<int>(gameData.GameState + Offset::AlivePlayers);
//                 // int AliveTeams = VmmCore::ReadValue<int>(gameData.GameState + Offset::AliveTeams);
//                 gameData.WorldOriginLocation =
//                     Vector3(VmmCore::ReadValue<int>(gameData.UWorld + Offset::WorldToMap),
//                             VmmCore::ReadValue<int>(gameData.UWorld + Offset::WorldToMap + 4), 0);

//                 gameData.PlayerCount = VmmCore::ReadValue<int>(gameData.GameState + Offset::PlayerArray + 0x8);

//                 // auto HostName = VmmCore::ReadValue<FText>(VmmCore::ReadValue<DWORD_PTR>(gameData.GameState +
//                 // 0x0478)).buffer; Utils::Log(1, "%s", HostName);
//                 std::vector<GamePlayerInfo> playerLists;
//                 DWORD_PTR* playerArrayBuffer = new DWORD_PTR[gameData.PlayerCount];
//                 VmmCore::ReadByte(VmmCore::ReadValue<DWORD_PTR>(gameData.GameState + Offset::PlayerArray),
//                                   playerArrayBuffer, sizeof(DWORD_PTR) * gameData.PlayerCount);
//                 std::vector<DWORD_PTR> playerArray(playerArrayBuffer, playerArrayBuffer + gameData.PlayerCount);

//                 for (auto& pPlayerInfo : playerArray) {
//                     GamePlayerInfo player;
//                     player.pPlayerInfo = pPlayerInfo;
//                     playerLists.push_back(player);
//                 }

//                 for (GamePlayerInfo& player : playerLists) {
//                     VmmCore::ScatterReadEx(3, player.pPlayerInfo + Offset::PlayerName,
//                     (DWORD_PTR*)&player.pPlayerName); VmmCore::ScatterReadEx(3, player.pPlayerInfo +
//                     Offset::AccountId, (DWORD_PTR*)&player.pAccountId); VmmCore::ScatterReadEx(3, player.pPlayerInfo
//                     + Offset::PlayerTeamId, (int*)&player.TeamID); VmmCore::ScatterReadEx(3, player.pPlayerInfo +
//                     Offset::PartnerLevel,
//                                            (EPartnerLevel*)&player.PartnerLevel);
//                     VmmCore::ScatterReadEx(3, player.pPlayerInfo + Offset::PubgIdData,
//                                            (FWuPubgIdData*)&player.PubgIdData);
//                     VmmCore::ScatterReadEx(3, player.pPlayerInfo + Offset::PubgIdData,
//                     (DWORD_PTR*)&player.pPubgIdData);
//                     // VmmCore::ScatterReadEx(3, player.pPlayerInfo + Offset::CharacterClanInfo,
//                     // (FWuCharacterClanInfo*)&player.CharacterClanInfo);
//                     VmmCore::ScatterReadEx(3, player.pPlayerInfo + 0x0410, (int*)&player.E);
//                 }

//                 VmmCore::ScatterExecuteReadEx(3);

//                 for (GamePlayerInfo& player : playerLists) {
//                     VmmCore::ScatterReadEx(3, player.pPlayerName, (FText*)&player.FPlayerName);
//                     VmmCore::ScatterReadEx(3, player.pAccountId, (FText*)&player.FAccountId);
//                     VmmCore::ScatterReadEx(3, player.pPlayerInfo + 0x0A90 + 0x0020, (DWORD_PTR*)&player.pClanName);
//                 }

//                 VmmCore::ScatterExecuteReadEx(3);

//                 for (GamePlayerInfo& player : playerLists) {
//                     VmmCore::ScatterReadEx(3, player.pClanName, (FText*)&player.FClanName);
//                 }

//                 VmmCore::ScatterExecuteReadEx(3);

//                 for (GamePlayerInfo& player : playerLists) {
//                     player.ClanName = Utils::UnicodeToAnsi(player.FClanName.buffer);
//                     player.AccountId = Utils::UnicodeToAnsi(player.FAccountId.buffer);
//                     player.PlayerName = Utils::UnicodeToAnsi(player.FPlayerName.buffer);
//                 }

//                 for (GamePlayerInfo& player : playerLists) {
//                     gameData.PlayerLists[player.PlayerName] = player;
//                     // Utils::Log(1, "%d", player.CharacterClanInfo.ClanLevel);
//                     // Utils::Log(1, "%s - %s", player.PlayerName, player.FClanName.buffer);
//                     if (player.PlayerName == "0x97") {
//                         std::cout << player.ClanName << std::endl;
//                     }
//                 }
//             }
//         }
//         Sleep(1000);
//     }
// }

// void UpdateItmes() {
//     std::vector<ItemInfo> items = gameData.Actors.CacheItems;
//     std::vector<DWORD_PTR> itemEntitys;
//     std::vector<DWORD_PTR> itemRootComponents;
//     std::vector<Vector3> itemLocations;

//     int count = items.size();

//     if (count == 0) {
//         gameData.Actors.Items = {};
//         return;
//     }

//     for (int i = 0; i < count; ++i) {
//         itemEntitys.push_back(items[i].Entity);
//         itemRootComponents.push_back(items[i].RootComponent);
//     }

//     itemLocations = VmmCore::BatchRead<std::vector<Vector3>>(itemRootComponents, Offset::ComponentLocation, 1);

//     for (int i = 0; i < count; ++i) {
//         items[i].Location = itemLocations[i];
//         items[i].ScreenLocation = VectorHelper::WorldToScreen(items[i].Location);
//         items[i].Distance = gameData.Location.Distance(items[i].Location) / 100.0f;
//     }

//     gameData.Actors.Items = items;
// }

void UpdateProjects() {
    // std::vector<ProjectInfo> items = gameData.Actors.CacheProjects;
    // std::vector<DWORD_PTR> entitys;
    // std::vector<DWORD_PTR> rootComponents;
    // std::vector<Vector3> locations;
    // std::vector<float> timeTillExplosions;

    // int count = items.size();

    // if (count == 0) {
    //     gameData.Actors.Projects = {};
    //     return;
    // }

    // for (int i = 0; i < count; ++i) {
    //     entitys.push_back(items[i].Entity);
    //     rootComponents.push_back(items[i].RootComponent);
    // }

    // locations = VmmCore::BatchRead<std::vector<Vector3>>(rootComponents, Offset::ComponentLocation, 1);
    // timeTillExplosions = VmmCore::BatchRead<std::vector<float>>(entitys, Offset::TimeTillExplosion, 1);

    // std::vector<int> discardProjectIndexs;

    // for (int i = 0; i < count; ++i) {
    //     for (auto project : gameData.Actors.Projects) {
    //         if (project.Entity == items[i].Entity) {
    //             items[i] = project;
    //             break;
    //         }
    //     }

    //     items[i].Location = locations[i];
    //     items[i].ScreenLocation = VectorHelper::WorldToScreen(items[i].Location);
    //     items[i].Distance = gameData.Location.Distance(items[i].Location) / 100.0f;

    //     items[i].TimeTillExplosion = timeTillExplosions[i];
    // }

    // gameData.Actors.Projects = items;
}

void UpdateCamera() {
    float FOV;
    Vector3 Location;
    Vector3 Rotation;

    VmmCore::ScatterReadEx(2, gameData.PlayerCameraManager + Offset::FOV, (float*)&FOV);
    VmmCore::ScatterReadEx(2, gameData.PlayerCameraManager + Offset::Location, (Vector3*)&Location);
    VmmCore::ScatterReadEx(2, gameData.PlayerCameraManager + Offset::Rotation, (Vector3*)&Rotation);

    VmmCore::ScatterExecuteReadEx(2);

    gameData.FOV = FOV;
    gameData.Location = Location;
    gameData.Rotation = Rotation;
}

void UpdatePlayers() {
    // while (gameData.Scene == Scene::Gameing) {
    //     // Timer timer("UpdatePlayers");
    //     if (gameData.Actors.CachePlayers.size() == 0) continue;

    //     std::vector<PlayerInfo> players = Data::GetCachePlayers();

    //     for (PlayerInfo& player : players) {
    //         VmmCore::ScatterReadEx(2, player.Entity + Offset::RootComponent, (DWORD_PTR*)&player.RootComponent);
    //         VmmCore::ScatterReadEx(2, player.Entity + Offset::Mesh, (DWORD_PTR*)&player.MeshComponent);

    //         VmmCore::ScatterReadEx(2, player.Entity + Offset::PlayerState, (DWORD_PTR*)&player.PlayerState);
    //         VmmCore::ScatterReadEx(2, player.Entity + Offset::TeamNumber, (int*)&player.TeamID);
    //         VmmCore::ScatterReadEx(2, player.Entity + Offset::CharacterName, (DWORD_PTR*)&player.pCharacterName);
    //         VmmCore::ScatterReadEx(2, player.Entity + Offset::Health, (float*)&player.Health);
    //         VmmCore::ScatterReadEx(2, player.Entity + Offset::GroggyHealth, (float*)&player.GroggyHealth);

    //         VmmCore::ScatterReadEx(2, player.Entity + Offset::CharacterState,
    //         (ECharacterState*)&player.CharacterState); VmmCore::ScatterReadEx(2, player.Entity +
    //         Offset::SpectatedCount, (int*)&player.SpectatedCount);

    //         VmmCore::ScatterReadEx(2, player.Entity + Offset::WeaponProcessor, (DWORD_PTR*)&player.WeaponProcessor);
    //     }

    //     VmmCore::ScatterExecuteReadEx(2);

    //     for (PlayerInfo& player : players) {
    //         player.RootComponent = Decrypt::Xe(player.RootComponent);
    //         player.PlayerState = Decrypt::Xe(player.PlayerState);

    //         VmmCore::ScatterReadEx(2, player.RootComponent + Offset::ComponentLocation, (Vector3*)&player.Location);
    //         VmmCore::ScatterReadEx(2, player.MeshComponent + Offset::ComponentToWorld,
    //                                (FTransform*)&player.ComponentToWorld);
    //         VmmCore::ScatterReadEx(2, player.MeshComponent + Offset::StaticMesh, (DWORD_PTR*)&player.StaticMesh);

    //         VmmCore::ScatterReadEx(2, player.PlayerState + Offset::PlayerSatisitc, (int*)&player.KillCount);
    //         VmmCore::ScatterReadEx(2, player.PlayerState + Offset::DamageDealtOnEnemy,
    //                                (float*)&player.DamageDealtOnEnemy);
    //         VmmCore::ScatterReadEx(2, player.PlayerState + Offset::SurvivalTier, (int*)&player.SurvivalTier);
    //         VmmCore::ScatterReadEx(2, player.PlayerState + Offset::SurvivalLevel, (int*)&player.SurvivalLevel);
    //         VmmCore::ScatterReadEx(2, player.PlayerState + Offset::PartnerLevel,
    //         (EPartnerLevel*)&player.PartnerLevel);

    //         VmmCore::ScatterReadEx(2, player.pCharacterName, (FText*)&player.CharacterName);

    //         VmmCore::ScatterReadEx(2, player.WeaponProcessor + Offset::EquippedWeapons,
    //                                (DWORD_PTR*)&player.EquippedWeapons);
    //         VmmCore::ScatterReadEx(2, player.WeaponProcessor + Offset::CurrentWeaponIndex,
    //                                (BYTE*)&player.CurrentWeaponIndex);
    //     }

    //     VmmCore::ScatterExecuteReadEx(2);

    //     for (PlayerInfo& player : players) {
    //         if (player.CurrentWeaponIndex >= 0 && player.CurrentWeaponIndex < 8) {
    //             VmmCore::ScatterReadEx(2, player.EquippedWeapons + player.CurrentWeaponIndex * 8,
    //                                    (DWORD_PTR*)&player.CurrentWeapon);
    //         }
    //     }

    //     VmmCore::ScatterExecuteReadEx(2);

    //     for (PlayerInfo& player : players) {
    //         if (player.CurrentWeapon > 0) {
    //             VmmCore::ScatterReadEx(2, player.CurrentWeapon + Offset::ObjID, (int*)&player.WeaponID);
    //         }

    //         for (EBoneIndex bone : SkeletonLists::skeleton_bones) {
    //             VmmCore::ScatterReadEx(2, player.StaticMesh + (bone * sizeof(FTransform)),
    //                                    (FTransform*)&player.Skeleton.Bones[bone]);
    //         }
    //     }

    //     VmmCore::ScatterExecuteReadEx(2);

    //     if (!gameData.AimBot.Lock) {
    //         gameData.AimBot.TargetEntity = 0;
    //         gameData.AimBot.ScreenDistance = 1000.0f;
    //     }

    //     UpdateCamera();

    //     for (PlayerInfo& player : players) {
    //         // auto AimOffsets = VmmCore::BatchRead<std::vector<FRotator>>(playerEntitys, Offset::AimOffsets, 2);
    //         player.WeaponID = Decrypt::CIndex(player.WeaponID);

    //         player.WeaponName = findEntityInfoByID(player.WeaponID).DisplayName;

    //         for (EBoneIndex bone : SkeletonLists::skeleton_bones) {
    //             player.Skeleton.LocationBones[bone] =
    //                 VectorHelper::GetBoneWithRotation(player.Skeleton.Bones[bone], player.ComponentToWorld);
    //             player.Skeleton.ScreenBones[bone] = VectorHelper::WorldToScreen(player.Skeleton.LocationBones[bone]);
    //         }

    //         if (player.TeamID >= 100000) {
    //             player.TeamID = player.TeamID - 100000;
    //         }
    //         player.Distance = gameData.Location.Distance(player.Location) / 100.0f;
    //         player.IsMyTeam = player.TeamID == gameData.LocalPlayerTeamID;
    //         player.Name = Utils::UnicodeToAnsi(player.CharacterName.buffer);

    //         if (player.SurvivalTier > 0) player.SurvivalLevel = (player.SurvivalTier - 1) * 500 +
    //         player.SurvivalLevel;

    //         if (!gameData.AimBot.Lock) {
    //             if (player.Entity != gameData.LocalPlayerPawn && player.TeamID != gameData.LocalPlayerTeamID) {
    //                 float screenDistance = std::sqrt(
    //                     std::pow(player.Skeleton.ScreenBones[EBoneIndex::forehead].x - gameData.ScreenCenter.X, 2) +
    //                     std::pow(player.Skeleton.ScreenBones[EBoneIndex::forehead].y - gameData.ScreenCenter.Y, 2));

    //                 if (screenDistance < gameData.Config.AimBot.AR.FOV) {
    //                     if (screenDistance < gameData.AimBot.ScreenDistance) {
    //                         gameData.AimBot.TargetEntity = player.Entity;
    //                         gameData.AimBot.ScreenDistance = player.Distance;
    //                     }
    //                 }
    //             }
    //         }
    //     }

    //     std::sort(players.begin(), players.end(),
    //               [](const PlayerInfo& a, const PlayerInfo& b) { return a.Distance > b.Distance; });

    //     Data::SetPlayers(players);
    // }
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

FVector GetPredictPlayerLocation(FVector EntityLocation, FVector EntitySpeed, FLOAT Distance,
                                 FVector WeaponTrajectory) {
    FLOAT BulletFlightTime = FLOAT();

    FLOAT FlyRatio = FLOAT();

    FLOAT tmp = (WeaponTrajectory.X / 1000.f);

    FlyRatio = (FLOAT)(WeaponTrajectory.Y / tmp);

    BulletFlightTime = (Distance / WeaponTrajectory.Y) * FlyRatio;

    return FVector(EntityLocation.X + EntitySpeed.X * BulletFlightTime,
                   EntityLocation.Y + EntitySpeed.Y * BulletFlightTime,
                   EntityLocation.Z + (0.5f * (Distance * WeaponTrajectory.Y) * powf(BulletFlightTime, 2)));
}

float GetBulletDrop(float step, FWeaponTrajectoryConfig CurrentWeaponConfig) {
    float TGZ = 9.8;
    float SST = CurrentWeaponConfig.SimulationSubstepTime;
    float VDC = CurrentWeaponConfig.VDragCoefficient;
    float BDS = CurrentWeaponConfig.BDS;
    float STS = 100.f;
    if (BDS <= 0.0 || SST <= 0.0 || VDC <= 0.0) {
        BDS = 1.0;
        VDC = 0.75;
        SST = 0.016;
    }
    float DP = 0;
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

            DWORD_PTR WeaponData =
                VmmCore::ReadValue<DWORD_PTR>(localPlayer.CurrentWeapon + Offset::WeaponTrajectoryData);
            FWeaponTrajectoryConfig TrajectoryConfig =
                VmmCore::ReadValue<FWeaponTrajectoryConfig>(WeaponData + Offset::TrajectoryConfig);
            FRichCurve RichCurve =
                VmmCore::ReadValue<FRichCurve>(TrajectoryConfig.BallisticCurve + Offset::FloatCurves + Offset::Keys);

            std::vector<FRichCurveKey> Keys;
            DWORD_PTR KeysArray =
                VmmCore::ReadValue<DWORD_PTR>(WeaponData + Offset::BallisticCurve + Offset::FloatCurves + Offset::Keys);
            auto KeysArrayCount =
                VmmCore::ReadValue<ULONG32>(TrajectoryConfig.BallisticCurve + Offset::FloatCurves + Offset::Keys + 8);

            Utils::Log(2, "TrajectoryConfig.BallisticCurve: 0x%11x", TrajectoryConfig.BallisticCurve);

            /*for (auto i = 0; i < KeysArrayCount; i++)
            {
                    FRichCurveKey RichCurveKey = Read<FRichCurveKey>(KeysArray + (0x1C * i));
                    Keys.push_back(RichCurveKey);
            }*/
            // Reading keys (I don't know why but I can't read TArray from UE4)
            /*float TimeScale = 0.0f;
            Vector3 TargetActorS = VmmCore::ReadValue<Vector3>(targetPlayer.Entity + Offset::Veloctity);
            float flyTime = targetPlayer.Distance / TrajectoryConfig.InitialSpeed;
            Vector3 PredictedPos = targetPlayer.Skeleton.LocationBones[EBoneIndex::forehead] + TargetActorS * flyTime;
            flyTime /= 0,1;
            if (TrajectoryConfig.InitialSpeed > 0) {
                    PredictedPos.z = PredictedPos.z + (GetBulletDrop(flyTime / 10.0, TrajectoryConfig) -
            (targetPlayer.Distance >= 320.0f ? 0 : 15.6f) + (TargetActorS.z * flyTime * 0.0360f));

                    Utils::Log(2, "Distance: %f, TrajectoryConfig: %f", targetPlayer.Distance, PredictedPos.z);
            }*/
            // Utils::Log(2, "TrajectoryConfig.BallisticCurve: 0x%11x", TrajectoryConfig.BallisticCurve);
            // Utils::Log(2, "RichCurve: %f", RichCurve.DefaultValue);
            // Utils::Log(2, "RichCurve: %d", KeysArrayCount);

            // auto WeaponProcessor = VmmCore::ReadValue<DWORD_PTR>(gameData.LocalPlayerPawn + Offset::WeaponProcessor);

            // auto EquippedWeapons = VmmCore::ReadValue<DWORD_PTR>(WeaponProcessor + Offset::EquippedWeapons);

            // auto CurrentWeaponIndex = VmmCore::ReadValue<BYTE>(WeaponProcessor + Offset::CurrentWeaponIndex);
            // if (CurrentWeaponIndex >= 0 && CurrentWeaponIndex < 8)
            //{
            //	auto pWeap = VmmCore::ReadValue<DWORD_PTR>(EquippedWeapons + CurrentWeaponIndex * 8);
            //	if (pWeap > 0)
            //	{
            //		ULONG CurrentWeaponID = Decrypt::CIndex(VmmCore::ReadValue<DWORD_PTR>(pWeap + Offset::ObjID));
            //		DWORD_PTR WeaponData = VmmCore::ReadValue<DWORD_PTR>(pWeap + Offset::WeaponTrajectoryData);
            //		FWeaponTrajectoryConfig TrajectoryConfig =
            // VmmCore::ReadValue<FWeaponTrajectoryConfig>(WeaponData + Offset::TrajectoryConfig); 		float
            // Gravity = VmmCore::ReadValue<float>(WeaponData + Offset::TrajectoryConfig + 0x18);

            //		FVector velocity{};
            //		velocity = GetVihecleSpeed(player.Entity);
            //		if (velocity.X == 0.f && velocity.Y == 0.f && velocity.Z == 0.f) {
            //			velocity = VmmCore::ReadValue<FVector>(player.RootComponent +
            // Offset::ComponentVelocity);
            //		}

            //		FVector WeaponTrajectory{};
            //		WeaponTrajectory.X = TrajectoryConfig.InitialSpeed;

            //		WeaponTrajectory.Y = TrajectoryConfig.WeaponSpread;

            //		WeaponTrajectory.Y = Gravity;

            //		auto pos = player.Skeleton.LocationBones[EBoneIndex::forehead];
            //		Vector3 screenPos;
            //		float dist;
            //		WorldToScreen(pos, &screenPos, &dist);
            //		FVector ObjePos = GetPredictPlayerLocation(FVector(pos.x, pos.y, pos.z), velocity, 1,
            // WeaponTrajectory); 		Vector3 aimPOS = WorldToScreenAim(Vector3(ObjePos.X, ObjePos.Y,
            // ObjePos.Z)); 		Utils::Log(2, "AimBot Target: %s | %f | %f | %f | %f", player.Name, dist,
            // aimPOS.x,
            // aimPOS.y, screenPos.z);
            //		//KmBox::Move(aimPOS.x, aimPOS.y);
            //		//Utils::Log(2, "AimBot Target: %s | %d | %f | %f", player.Name, CurrentWeaponID,
            // TrajectoryConfig.InitialSpeed, Gravity);
            //	}
            //}
        } else {
            gameData.AimBot.Lock = false;
        }

        auto endTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> frameTime = endTime - startTime;

        double delayTime = targetFrameTime - frameTime.count();
        if (delayTime > 0.0) {
            std::this_thread::sleep_for(std::chrono::duration<double>(delayTime));
        }
    }
}

void UpdateAddress() {
    while (true) {
        if (gameData.PID > 0) {
            gameData.UWorld = Decrypt::Xe(VmmCore::ReadValue<DWORD_PTR>(gameData.GameBase + Offset::UWorld));
            gameData.CurrentLevel = Decrypt::Xe(VmmCore::ReadValue<DWORD_PTR>(gameData.UWorld + Offset::CurrentLevel));
            gameData.GNames = GNames::GetGNamesPtr();
            gameData.GameInstance = Decrypt::Xe(VmmCore::ReadValue<DWORD_PTR>(gameData.UWorld + Offset::GameInstance));
            gameData.GameState = Decrypt::Xe(VmmCore::ReadValue<DWORD_PTR>(gameData.UWorld + Offset::GameState));
            gameData.LocalPlayer = Decrypt::Xe(VmmCore::ReadValue<DWORD_PTR>(
                VmmCore::ReadValue<DWORD_PTR>(gameData.GameInstance + Offset::LocalPlayer)));
            gameData.PlayerController =
                Decrypt::Xe(VmmCore::ReadValue<DWORD_PTR>(gameData.LocalPlayer + Offset::PlayerController));
            gameData.PlayerCameraManager =
                VmmCore::ReadValue<DWORD_PTR>(gameData.PlayerController + Offset::PlayerCameraManager);
            gameData.MyHUD = VmmCore::ReadValue<DWORD_PTR>(gameData.PlayerController + Offset::MyHUD);
            gameData.Map.WorldOriginLocation =
                Vector3(VmmCore::ReadValue<int>(gameData.UWorld + Offset::WorldToMap),
                        VmmCore::ReadValue<int>(gameData.UWorld + Offset::WorldToMap + 4), 0);
            // gameData.MapZoomValue = VmmCore::ScatterReadEx(0, gameData.Actor + 0x8, (int*)&gameData.ActorCount);
            UpdateGameScene();
        }
    }
}
std::vector<ActorEntityInfo> gEntitys;
void UpdateEntitys2() {
    while (gameData.Scene == Scene::Gameing) {
        gameData.Actor = Decrypt::Xe(VmmCore::ReadValue<DWORD_PTR>(gameData.CurrentLevel + Offset::Actor));
        VmmCore::ScatterReadEx(0, gameData.Actor + 0x8, (int*)&gameData.ActorCount);
        VmmCore::ScatterReadEx(0, gameData.Actor, (DWORD_PTR*)&gameData.Actor);
        VmmCore::ScatterExecuteReadEx(0);
        if (gameData.ActorCount <= 0 || gameData.ActorCount > 80000) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        std::vector<DWORD_PTR> entitys(gameData.ActorCount);
        for (int i = 0; i < gameData.ActorCount; i++) {
            DWORD_PTR address = gameData.Actor + (i * 0x8);
            VmmCore::ScatterReadEx(0, address, (DWORD_PTR*)&entitys[i]);
        }
        VmmCore::ScatterExecuteReadEx(0);

        for (auto entity : entitys) {
            if (entity == 0) continue;

            if (std::find_if(gEntitys.begin(), gEntitys.end(), [entity](const ActorEntityInfo& info) {
                    return info.Entity == entity;
                }) == gEntitys.end()) {
                gEntitys.push_back({entity, 0, 0, EntityType::Unknown});
                DWORD_PTR entityptr = entity + Offset::ObjID;
                SetScatterPrepare<DWORD_PTR>(entityptr);
            }
        }
        SetScatterExecute();
        for (auto& entity : gEntitys) {
            int id = GetEntityID(entity.Entity + Offset::ObjID);
            entity.ID = id;

            entity.decodeID = Decrypt::CIndex(id);
        }
        SetScatterClear();

        std::vector<PlayerInfo> players;        // 玩家信息
        std::vector<ProjectInfo> projects;      // 投掷物品
        std::vector<ItemInfo> items;            // 掉落物品
        std::vector<ItemGroupInfo> itemGroups;  // 刷新物品
        players = Data::GetCachePlayers();
        projects = Data::GetCacheProjects();
        items = Data::GetCacheItems();
        itemGroups = Data::GetItemGroups();
        for (auto entity : gEntitys) {
            if (entity.decodeID == 0) continue;
            EntityInfo entityInfo = findEntityInfoByID(entity.decodeID);
            entity.type = entityInfo.Type;
            auto entityptr = entity.Entity;

            if ((entity.type == EntityType::Player || entity.type == EntityType::AI)) {
                if ((std::find_if(players.begin(), players.end(), [entityptr](const PlayerInfo& info) {
                        return info.Entity == entityptr;
                    })) == players.end()) {
                    players.push_back({entity.type, entity.Entity, entity.decodeID});
                }
            } else if (entityInfo.Type == EntityType::Project) {
                if ((std::find_if(projects.begin(), projects.end(), [entityptr](const ProjectInfo& info) {
                        return info.Entity == entityptr;
                    })) == projects.end()) {
                    projects.push_back({entity.type, entity.Entity, entity.decodeID, entityInfo.DisplayName});
                }
            }

            else if (entityInfo.Type == EntityType::DroppedItem) {
                if ((std::find_if(items.begin(), items.end(), [entityptr](const ItemInfo& info) {
                        return info.Entity == entityptr;
                    })) == items.end()) {
                    items.push_back({entityInfo.Type, entity.Entity});
                }
            }

            else if (entityInfo.Type == EntityType::DroppedItemGroup) {
                if ((std::find_if(itemGroups.begin(), itemGroups.end(), [entityptr](const ItemGroupInfo& info) {
                        return info.Entity == entityptr;
                    })) == itemGroups.end()) {
                    itemGroups.push_back({entityInfo.Type, entity.Entity, entity.decodeID});
                }
            }
        }
        Data::SetCachePlayers(players);
        Data::SetCacheProjects(projects);
        Data::SetCacheItems(items);
        Data::SetItemGroups(itemGroups);

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void UpdatePlayers2() {
    while (gameData.Scene == Scene::Gameing) {
        std::vector<PlayerInfo> players = Data::GetCachePlayers();
        if (players.size() == 0) continue;

        for (PlayerInfo& player : players) {
            VmmCore::ScatterReadEx(2, player.Entity + Offset::CharacterName, (DWORD_PTR*)&player.pCharacterName);
            VmmCore::ScatterReadEx(2, player.Entity + Offset::Mesh, (DWORD_PTR*)&player.MeshComponent);
            VmmCore::ScatterReadEx(2, player.Entity + Offset::AimOffsets, (Vector3*)&player.AimOffsets);
            // VmmCore::ScatterReadEx(2, player.Entity + Offset::RootComponent, (DWORD_PTR*)&player.RootComponent);

            // VmmCore::ScatterReadEx(2, player.Entity + Offset::PlayerState, (DWORD_PTR*)&player.PlayerState);
            // VmmCore::ScatterReadEx(2, player.Entity + Offset::TeamNumber, (int*)&player.TeamID);

            // VmmCore::ScatterReadEx(2, player.Entity + Offset::Health, (float*)&player.Health);
            // VmmCore::ScatterReadEx(2, player.Entity + Offset::GroggyHealth, (float*)&player.GroggyHealth);

            // VmmCore::ScatterReadEx(2, player.Entity + Offset::CharacterState,
            // (ECharacterState*)&player.CharacterState); VmmCore::ScatterReadEx(2, player.Entity +
            // Offset::SpectatedCount, (int*)&player.SpectatedCount);

            // VmmCore::ScatterReadEx(2, player.Entity + Offset::WeaponProcessor, (DWORD_PTR*)&player.WeaponProcessor);
        }

        VmmCore::ScatterExecuteReadEx(2);

        for (PlayerInfo& player : players) {
            VmmCore::ScatterReadEx(2, player.pCharacterName, (FText*)&player.CharacterName);
            VmmCore::ScatterReadEx(2, player.MeshComponent + Offset::ComponentLocation, (Vector3*)&player.Location);
            // player.RootComponent = Decrypt::Xe(player.RootComponent);
            // player.PlayerState = Decrypt::Xe(player.PlayerState);

            // VmmCore::ScatterReadEx(2, player.MeshComponent + Offset::ComponentToWorld,
            //                        (FTransform*)&player.ComponentToWorld);
            // VmmCore::ScatterReadEx(2, player.MeshComponent + Offset::StaticMesh, (DWORD_PTR*)&player.StaticMesh);

            // VmmCore::ScatterReadEx(2, player.PlayerState + Offset::PlayerSatisitc, (int*)&player.KillCount);
            // VmmCore::ScatterReadEx(2, player.PlayerState + Offset::DamageDealtOnEnemy,
            //                        (float*)&player.DamageDealtOnEnemy);
            // VmmCore::ScatterReadEx(2, player.PlayerState + Offset::SurvivalTier, (int*)&player.SurvivalTier);
            // VmmCore::ScatterReadEx(2, player.PlayerState + Offset::SurvivalLevel, (int*)&player.SurvivalLevel);
            // VmmCore::ScatterReadEx(2, player.PlayerState + Offset::PartnerLevel,
            // (EPartnerLevel*)&player.PartnerLevel);

            // VmmCore::ScatterReadEx(2, player.WeaponProcessor + Offset::EquippedWeapons,
            //                        (DWORD_PTR*)&player.EquippedWeapons);
            // VmmCore::ScatterReadEx(2, player.WeaponProcessor + Offset::CurrentWeaponIndex,
            //                        (BYTE*)&player.CurrentWeaponIndex);
        }

        VmmCore::ScatterExecuteReadEx(2);

        // for (PlayerInfo& player : players) {
        //     if (player.CurrentWeaponIndex >= 0 && player.CurrentWeaponIndex < 8) {
        //         VmmCore::ScatterReadEx(2, player.EquippedWeapons + player.CurrentWeaponIndex * 8,
        //                                (DWORD_PTR*)&player.CurrentWeapon);
        //     }
        // }

        // VmmCore::ScatterExecuteReadEx(2);

        // for (PlayerInfo& player : players) {
        //     if (player.CurrentWeapon > 0) {
        //         VmmCore::ScatterReadEx(2, player.CurrentWeapon + Offset::ObjID, (int*)&player.WeaponID);
        //     }

        //     for (EBoneIndex bone : SkeletonLists::skeleton_bones) {
        //         VmmCore::ScatterReadEx(2, player.StaticMesh + (bone * sizeof(FTransform)),
        //                                (FTransform*)&player.Skeleton.Bones[bone]);
        //     }
        // }

        // VmmCore::ScatterExecuteReadEx(2);

        // if (!gameData.AimBot.Lock) {
        //     gameData.AimBot.TargetEntity = 0;
        //     gameData.AimBot.ScreenDistance = 1000.0f;
        // }

        // UpdateCamera();

        for (PlayerInfo& player : players) {
            // auto AimOffsets = VmmCore::BatchRead<std::vector<FRotator>>(playerEntitys, Offset::AimOffsets, 2);
            // player.WeaponID = Decrypt::CIndex(player.WeaponID);

            // player.WeaponName = findEntityInfoByID(player.WeaponID).DisplayName;

            // for (EBoneIndex bone : SkeletonLists::skeleton_bones) {
            //     player.Skeleton.LocationBones[bone] =
            //         VectorHelper::GetBoneWithRotation(player.Skeleton.Bones[bone], player.ComponentToWorld);
            //     player.Skeleton.ScreenBones[bone] = VectorHelper::WorldToScreen(player.Skeleton.LocationBones[bone]);
            // }

            // if (player.TeamID >= 100000) {
            //     player.TeamID = player.TeamID - 100000;
            // }
            // player.Distance = gameData.Location.Distance(player.Location) / 100.0f;
            // player.IsMyTeam = player.TeamID == gameData.LocalPlayerTeamID;
            player.Name = Utils::UnicodeToAnsi(player.CharacterName.buffer);

            // if (player.SurvivalTier > 0) player.SurvivalLevel = (player.SurvivalTier - 1) * 500 +
            // player.SurvivalLevel;

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

        // std::sort(players.begin(), players.end(),
        //           [](const PlayerInfo& a, const PlayerInfo& b) { return a.Distance > b.Distance; });

        Data::SetPlayers(players);
    }
}

void UpdateItem() {
    // std::vector<PlayerInfo> players;          // 玩家信息
    // std::vector<ProjectInfo> projects;        // 投掷物品
    // std::vector<DWORD_PTR> projectEntitys;    // 投掷物品实体
    // std::vector<ItemInfo> items;              // 掉落物品
    // std::vector<DWORD_PTR> itemEntitys;       // 掉落物品实体
    // std::vector<ItemGroupInfo> itemGroups;    // 刷新物品
    // std::vector<DWORD_PTR> itemGroupEntitys;  // 刷新物品实体

    // std::vector<DWORD_PTR> projectRootComponents;
    // // projects
    // [&] {
    //     if (projects.size() > 0) {
    //         projectRootComponents = GetEntityRootComponents(projectEntitys);
    //         for (int i = 0; i < projects.size(); i++) {
    //             projects[i].RootComponent = projectRootComponents[i];
    //         }
    //     }

    //     gameData.Actors.CacheProjects = projects;
    // }();

    // std::vector<ItemInfo> cacheItems;
    // // itemGroups
    // [&] {
    //     if (itemGroups.size() > 0) {
    //         std::vector<DWORD_PTR> itemGroupLists =
    //             VmmCore::BatchRead<std::vector<DWORD_PTR>>(itemGroupEntitys, Offset::DroppedItemGroup, 0);
    //         std::vector<int> itemGroupCounts =
    //             VmmCore::BatchRead<std::vector<int>>(itemGroupEntitys, Offset::DroppedItemGroup + 0x8, 0);
    //         std::vector<DWORD_PTR> pItemGroupComponents;

    //         for (int i = 0; i < itemGroups.size(); i++) {
    //             for (int ii = 0; ii < itemGroupCounts[i]; ii++) {
    //                 DWORD_PTR item = VmmCore::ReadValue<DWORD_PTR>(itemGroupLists[i] + ii * 0x10);
    //                 pItemGroupComponents.push_back(itemGroupLists[i] + ii * 0x10);
    //             }
    //         }

    //         std::vector<DWORD_PTR> itemGroupComponents =
    //             VmmCore::BatchRead<std::vector<DWORD_PTR>>(pItemGroupComponents, 0x0, 0);
    //         std::vector<DWORD_PTR> itemGroupUItems =
    //             VmmCore::BatchRead<std::vector<DWORD_PTR>>(itemGroupComponents, Offset::DroppedItemGroupUItem, 0);
    //         std::vector<DWORD_PTR> itemGroupInformationComponents =
    //             VmmCore::BatchRead<std::vector<DWORD_PTR>>(itemGroupUItems, Offset::ItemInformationComponent, 0);
    //         std::vector<int> itemGroupIDs =
    //             VmmCore::BatchRead<std::vector<int>>(itemGroupInformationComponents, Offset::ItemID, 0);

    //         for (int i = 0; i < itemGroupIDs.size(); i++) {
    //             if (itemGroupIDs[i] < 1) continue;

    //             ItemInfo item;
    //             item.ID = itemGroupIDs[i];
    //             EntityInfo itemInfo = findEntityInfoByID(item.ID);

    //             if (itemInfo.Type != EntityType::Item) continue;

    //             item.RootComponent = itemGroupComponents[i];
    //             item.InformationComponent = itemGroupInformationComponents[i];
    //             item.Name = itemInfo.DisplayName;

    //             cacheItems.push_back(item);
    //         }
    //     }
    // }();

    // std::vector<DWORD_PTR> itemRootComponents;
    // std::vector<DWORD_PTR> itemDroppedItems;
    // std::vector<DWORD_PTR> itemInformationComponents;
    // std::vector<int> itemIDs;
    // // items
    // [&] {
    //     if (items.size() > 0) {
    //         itemDroppedItems = GetDroppedItems(itemEntitys);
    //         itemInformationComponents = GetItemInformationComponents(itemDroppedItems);
    //         itemIDs = GetItemIDs(itemInformationComponents);
    //         itemRootComponents = GetEntityRootComponents(itemEntitys);

    //         for (int i = 0; i < items.size(); i++) {
    //             items[i].RootComponent = itemRootComponents[i];
    //             items[i].InformationComponent = itemInformationComponents[i];
    //             items[i].ID = itemIDs[i];

    //             EntityInfo itemInfo = findEntityInfoByID(items[i].ID);
    //             items[i].Name = itemInfo.DisplayName;

    //             if (itemInfo.Type == EntityType::Item) {
    //                 cacheItems.push_back(items[i]);
    //             }
    //         }
    //     }

    //     gameData.Actors.CacheItems = cacheItems;
    // }();
}

void UpdateGameingDate() {
    while (true) {
        if (gameData.PID > 0) {
            if (gameData.Scene == Scene::Gameing) {
                gameData.PlayerCount = VmmCore::ReadValue<int>(gameData.GameState + Offset::PlayerArray + 0x8);

                std::vector<GamePlayerInfo> playerLists;
                DWORD_PTR* playerArrayBuffer = new DWORD_PTR[gameData.PlayerCount];
                VmmCore::ReadByte(VmmCore::ReadValue<DWORD_PTR>(gameData.GameState + Offset::PlayerArray),
                                  playerArrayBuffer, sizeof(DWORD_PTR) * gameData.PlayerCount);

                std::vector<DWORD_PTR> playerArray(playerArrayBuffer, playerArrayBuffer + gameData.PlayerCount);

                for (auto& pPlayerInfo : playerArray) {
                    GamePlayerInfo player;
                    player.pPlayerInfo = pPlayerInfo;
                    playerLists.push_back(player);
                }

                for (GamePlayerInfo& player : playerLists) {
                    VmmCore::ScatterReadEx(3, player.pPlayerInfo + Offset::PlayerName, (DWORD_PTR*)&player.pPlayerName);
                    VmmCore::ScatterReadEx(3, player.pPlayerInfo + Offset::AccountId, (DWORD_PTR*)&player.pAccountId);
                    VmmCore::ScatterReadEx(3, player.pPlayerInfo + Offset::PlayerTeamId, (int*)&player.TeamID);
                    VmmCore::ScatterReadEx(3, player.pPlayerInfo + Offset::PartnerLevel,
                                           (EPartnerLevel*)&player.PartnerLevel);
                    VmmCore::ScatterReadEx(3, player.pPlayerInfo + Offset::PubgIdData,
                                           (FWuPubgIdData*)&player.PubgIdData);
                    VmmCore::ScatterReadEx(3, player.pPlayerInfo + Offset::PubgIdData, (DWORD_PTR*)&player.pPubgIdData);
                    VmmCore::ScatterReadEx(3, player.pPlayerInfo + 0x0410, (int*)&player.E);
                }
                VmmCore::ScatterExecuteReadEx(3);
                for (GamePlayerInfo& player : playerLists) {
                    VmmCore::ScatterReadEx(3, player.pPlayerName, (FText*)&player.FPlayerName);
                    VmmCore::ScatterReadEx(3, player.pAccountId, (FText*)&player.FAccountId);
                    VmmCore::ScatterReadEx(3, player.pPlayerInfo + 0x0A90 + 0x0020, (DWORD_PTR*)&player.pClanName);
                }

                VmmCore::ScatterExecuteReadEx(3);

                for (GamePlayerInfo& player : playerLists) {
                    VmmCore::ScatterReadEx(3, player.pClanName, (FText*)&player.FClanName);
                }
                VmmCore::ScatterExecuteReadEx(3);

                for (GamePlayerInfo& player : playerLists) {
                    player.ClanName = Utils::UnicodeToAnsi(player.FClanName.buffer);
                    player.AccountId = Utils::UnicodeToAnsi(player.FAccountId.buffer);
                    player.PlayerName = Utils::UnicodeToAnsi(player.FPlayerName.buffer);
                }
                for (GamePlayerInfo& player : playerLists) {
                    gameData.PlayerLists[player.PlayerName] = player;
                    if (player.PlayerName == "0x97") {
                        // std::cout << player.ClanName << std::endl;
                    }
                }
            }
        }
    }
}

void HackStart() {
    // CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)RenderOverlayWindow, NULL, NULL, NULL);
    // CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ListenGameProcessState, NULL, NULL, NULL);
    // CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)UpdateAddress, NULL, NULL, NULL);
    // CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)AimBot, NULL, NULL, NULL);
    std::thread thread1(ListenGameProcessState);
    thread1.join();
    std::thread thread2(UpdateAddress);
    std::thread thread3(UpdateGameingDate);
    thread2.detach();
    thread3.detach();

    while (true) {
        if (gameData.Scene == Scene::Gameing) {
            // auto ddd = VmmCore::ReadValue<DWORD_PTR>(gameData.PlayerCameraManager + 0x16C0);
            // Utils::Log(2, "0x%11X", gameData.LocalPlayerPawn);
            // Utils::Log(1, "0x%11X", ddd);
            // UpdateCamera();
            // UpdateProjects();
            // UpdateItmes();

            Sleep(1000);
        } else {
            Sleep(10);
        }
    }
}
