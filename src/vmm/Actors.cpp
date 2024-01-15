#include "Actors.h"

std::vector<DWORD_PTR> GetEntitys(DWORD_PTR Actor) {
    std::vector<DWORD_PTR> entityAddresses;
    std::vector<DWORD_PTR> entitys;

    for (int i = 0; i < gameData.ActorCount; i++) {
        entityAddresses.push_back(Actor + (i * 0x8));
    }

    entitys = VmmCore::ReadScatterValues<std::vector<DWORD_PTR>>(VmmCore::GetScatterHandle(0), entityAddresses);

    return entitys;
}

std::vector<int> GetEntityIDs(std::vector<DWORD_PTR> entitys) {
    std::vector<DWORD_PTR> entitysIDAddresses;
    std::vector<int> entityIDs;
    std::vector<int> decryptedEntityIDs;

    for (int i = 0; i < entitys.size(); ++i) {
        entitysIDAddresses.push_back(entitys[i] + Offset::ObjID);
    }

    entityIDs = VmmCore::ReadScatterValues<std::vector<int>>(VmmCore::GetScatterHandle(0), entitysIDAddresses);

    for (int i = 0; i < entityIDs.size(); ++i) {
        int entityID = Decrypt::CIndex(entityIDs[i]);
        decryptedEntityIDs.push_back(entityID);
    }

    return decryptedEntityIDs;
}
std::vector<DWORD_PTR> GetEntityRootComponents(std::vector<DWORD_PTR> entitys) {
    std::vector<DWORD_PTR> rootComponentAddresses;
    std::vector<DWORD_PTR> rootComponents;
    std::vector<DWORD_PTR> decodeRootComponents;

    for (int i = 0; i < entitys.size(); ++i) {
        rootComponentAddresses.push_back(entitys[i] + Offset::RootComponent);
    }

    rootComponents = VmmCore::ReadScatterValues<std::vector<DWORD_PTR>>(VmmCore::GetScatterHandle(0), rootComponentAddresses);

    for (int i = 0; i < rootComponents.size(); ++i) {
        decodeRootComponents.push_back(Decrypt::Xe(rootComponents[i]));
    }

    return decodeRootComponents;
}

std::vector<DWORD_PTR> GetPlayerMeshComponents(std::vector<DWORD_PTR> entitys) {
    std::vector<DWORD_PTR> playerMeshComponentAddresses;
    std::vector<DWORD_PTR> playerMeshComponents;

    for (int i = 0; i < entitys.size(); ++i) {
        playerMeshComponentAddresses.push_back(entitys[i] + Offset::Mesh);
    }

    playerMeshComponents = VmmCore::ReadScatterValues<std::vector<DWORD_PTR>>(VmmCore::GetScatterHandle(0), playerMeshComponentAddresses);
    return playerMeshComponents;
}

std::vector<DWORD_PTR> GetPlayerStates(std::vector<DWORD_PTR> ptr) {
    std::vector<DWORD_PTR> addresses;
    std::vector<DWORD_PTR> relusts;
    std::vector<DWORD_PTR> deRelusts;

    for (int i = 0; i < ptr.size(); ++i) {
        addresses.push_back(ptr[i] + Offset::PlayerState);
    }

    relusts = VmmCore::ReadScatterValues<std::vector<DWORD_PTR>>(VmmCore::GetScatterHandle(0), addresses);

    for (int i = 0; i < relusts.size(); ++i) {
        deRelusts.push_back(Decrypt::Xe(relusts[i]));
    }

    return deRelusts;
}

std::vector<DWORD_PTR> GetDroppedItems(std::vector<DWORD_PTR> ptr) {
    std::vector<DWORD_PTR> addresses;
    std::vector<DWORD_PTR> relusts;
    std::vector<DWORD_PTR> deRelusts;

    for (int i = 0; i < ptr.size(); ++i) {
        addresses.push_back(ptr[i] + Offset::DroppedItem);
    }

    relusts = VmmCore::ReadScatterValues<std::vector<DWORD_PTR>>(VmmCore::GetScatterHandle(0), addresses);

    for (int i = 0; i < relusts.size(); ++i) {
        deRelusts.push_back(Decrypt::Xe(relusts[i]));
    }

    return deRelusts;
}

std::vector<DWORD_PTR> GetItemInformationComponents(std::vector<DWORD_PTR> ptr) {
    std::vector<DWORD_PTR> addresses;
    std::vector<DWORD_PTR> relusts;

    for (int i = 0; i < ptr.size(); ++i) {
        addresses.push_back(ptr[i] + Offset::ItemInformationComponent);
    }

    relusts = VmmCore::ReadScatterValues<std::vector<DWORD_PTR>>(VmmCore::GetScatterHandle(0), addresses);

    return relusts;
}

std::vector<int> GetItemIDs(std::vector<DWORD_PTR> ptr) {
    std::vector<DWORD_PTR> addresses;
    std::vector<int> relusts;

    for (int i = 0; i < ptr.size(); ++i) {
        addresses.push_back(ptr[i] + Offset::ItemID);
    }

    relusts = VmmCore::ReadScatterValues<std::vector<int>>(VmmCore::GetScatterHandle(0), addresses);

    return relusts;
}

void SetScatterExecute() { VmmCore::ScatterExecuteRead(VmmCore::GetScatterHandle(0)); }

void SetScatterClear() { VmmCore::ScatterClear(VmmCore::GetScatterHandle(0)); }

int GetEntityID(DWORD_PTR entity) {
    int entityID;
    VmmCore::ScatterRead(VmmCore::GetScatterHandle(0), entity, (int *)&entityID);
    return entityID;
    // return Decrypt::CIndex(entityID);
}

std::vector<FTransform> GetPlayerComponentToWorlds(std::vector<DWORD_PTR> ptr) {
    std::vector<DWORD_PTR> addresses;
    std::vector<FTransform> relusts;

    for (int i = 0; i < ptr.size(); ++i) {
        addresses.push_back(ptr[i] + Offset::ComponentToWorld);
    }

    relusts = VmmCore::ReadScatterValues<std::vector<FTransform>>(VmmCore::GetScatterHandle(1), addresses);

    return relusts;
}

std::vector<Vector3> GetEntityLocations(std::vector<DWORD_PTR> rootComponents) {
    std::vector<DWORD_PTR> locationAddresses;
    std::vector<Vector3> locations;

    for (int i = 0; i < rootComponents.size(); ++i) {
        locationAddresses.push_back(rootComponents[i] + Offset::ComponentLocation);
    }

    locations = VmmCore::ReadScatterValues<std::vector<Vector3>>(VmmCore::GetScatterHandle(1), locationAddresses);
    return locations;
}
std::vector<DWORD_PTR> GetPlayerBones(std::vector<DWORD_PTR> ptr) {
    std::vector<DWORD_PTR> addresses;
    std::vector<DWORD_PTR> relusts;

    for (int i = 0; i < ptr.size(); ++i) {
        addresses.push_back(ptr[i] + Offset::StaticMesh);
    }

    relusts = VmmCore::ReadScatterValues<std::vector<DWORD_PTR>>(VmmCore::GetScatterHandle(1), addresses);

    return relusts;
}
std::vector<FTransform> GetPlayerIndexBones(std::vector<DWORD_PTR> ptr, EBoneIndex index) {
    std::vector<DWORD_PTR> addresses;
    std::vector<FTransform> relusts;

    for (int i = 0; i < ptr.size(); ++i) {
        addresses.push_back(ptr[i] + (index * 0x30));
    }

    relusts = VmmCore::ReadScatterValues<std::vector<FTransform>>(VmmCore::GetScatterHandle(1), addresses);

    return relusts;
}

std::vector<PlayerSkeletons> GetPlayerAllBones(std::vector<DWORD_PTR> ptr, std::vector<FTransform> playerComponentToWorlds) {
    std::vector<PlayerSkeletons> relusts;
    // PlayerSkeletons playerSkeletons;

    // for (int i = 0; i < ptr.size(); ++i) {
    //     relusts.push_back(playerSkeletons);
    // }

    // for (auto bone : SkeletonLists::skeleton_bones) {
    //     std::vector<FTransform> boneRelusts = GetPlayerIndexBones(ptr, bone);
    //     for (int i = 0; i < ptr.size(); ++i) {
    //         relusts[i].bones[bone] = VectorHelper::GetBoneWithRotation(boneRelusts[i], playerComponentToWorlds[i]);
    //     }
    // }

    // VmmCore::ScatterClear(VmmCore::GetScatterHandle(1));

    // VmmCore::ScatterPrepare(VmmCore::GetScatterHandle(1), gameData.PlayerCameraManager + Offset::FOV, sizeof(float));
    // VmmCore::ScatterPrepare(VmmCore::GetScatterHandle(1), gameData.PlayerCameraManager + Offset::Location,
    //                         sizeof(Vector3));
    // VmmCore::ScatterPrepare(VmmCore::GetScatterHandle(1), gameData.PlayerCameraManager + Offset::Rotation,
    //                         sizeof(Vector3));

    // VmmCore::ScatterExecuteRead(VmmCore::GetScatterHandle(1));

    // gameData.FOV =
    //     VmmCore::ScatterReadValue<float>(VmmCore::GetScatterHandle(1), gameData.PlayerCameraManager + Offset::FOV);
    // gameData.Location = VmmCore::ScatterReadValue<Vector3>(VmmCore::GetScatterHandle(1),
    //                                                        gameData.PlayerCameraManager + Offset::Location);
    // gameData.Rotation = VmmCore::ScatterReadValue<Vector3>(VmmCore::GetScatterHandle(1),
    //                                                        gameData.PlayerCameraManager + Offset::Rotation);

    // for (int i = 0; i < relusts.size(); ++i) {
    //     for (auto bone : SkeletonLists::skeleton_bones) {
    //         relusts[i].bones[bone] = VectorHelper::WorldToScreen(relusts[i].bones[bone]);
    //     }
    // }

    return relusts;
}

std::vector<PlayerSkeletons> GetPlayerAllBones2(std::vector<DWORD_PTR> ptr, std::vector<FTransform> playerComponentToWorlds) {
    std::vector<DWORD_PTR> addresses;
    std::vector<PlayerSkeletons> relusts;

    // VmmCore::ScatterClear(VmmCore::GetScatterHandle(1));

    // VmmCore::ScatterPrepare(VmmCore::GetScatterHandle(1), gameData.PlayerCameraManager + Offset::FOV, sizeof(float));
    // VmmCore::ScatterPrepare(VmmCore::GetScatterHandle(1), gameData.PlayerCameraManager + Offset::Location,
    //                         sizeof(Vector3));
    // VmmCore::ScatterPrepare(VmmCore::GetScatterHandle(1), gameData.PlayerCameraManager + Offset::Rotation,
    //                         sizeof(Vector3));

    // VmmCore::ScatterExecuteRead(VmmCore::GetScatterHandle(1));

    // gameData.FOV =
    //     VmmCore::ScatterReadValue<float>(VmmCore::GetScatterHandle(1), gameData.PlayerCameraManager + Offset::FOV);
    // gameData.Location = VmmCore::ScatterReadValue<Vector3>(VmmCore::GetScatterHandle(1),
    //                                                        gameData.PlayerCameraManager + Offset::Location);
    // gameData.Rotation = VmmCore::ScatterReadValue<Vector3>(VmmCore::GetScatterHandle(1),
    //                                                        gameData.PlayerCameraManager + Offset::Rotation);

    // VmmCore::ScatterClear(VmmCore::GetScatterHandle(1));

    // for (int i = 0; i < ptr.size(); ++i) {
    //     for (auto bone : SkeletonLists::skeleton_bones) {
    //         VmmCore::ScatterPrepare(VmmCore::GetScatterHandle(1), ptr[i] + (bone * 0x30), sizeof(FTransform));
    //     }
    // }

    // VmmCore::ScatterExecuteRead(VmmCore::GetScatterHandle(1));

    // for (int i = 0; i < ptr.size(); ++i) {
    //     PlayerSkeletons playerSkeletons;
    //     for (EBoneIndex bone : SkeletonLists::skeleton_bones) {
    //         playerSkeletons.bones[bone] = VectorHelper::WorldToScreen(VectorHelper::GetBoneWithRotation(
    //             VmmCore::ScatterReadValue<FTransform>(VmmCore::GetScatterHandle(1), ptr[i] + (bone * 0x30)),
    //             playerComponentToWorlds[i]));
    //     }
    //     relusts.push_back(playerSkeletons);
    // }

    return relusts;
}

std::vector<std::string> GetPlayerNames(std::vector<DWORD_PTR> entitys) {
    std::vector<DWORD_PTR> characterNameAddresses;
    std::vector<DWORD_PTR> characterNames;

    for (int i = 0; i < entitys.size(); ++i) {
        characterNameAddresses.push_back(entitys[i] + Offset::CharacterName);
    }

    characterNames = VmmCore::ReadScatterValues<std::vector<DWORD_PTR>>(VmmCore::GetScatterHandle(2), characterNameAddresses);

    std::vector<FText> playerNames;
    std::vector<std::string> decodePlayerNames;

    playerNames = VmmCore::ReadScatterValues<std::vector<FText>>(VmmCore::GetScatterHandle(2), characterNames);

    for (int i = 0; i < playerNames.size(); ++i) {
        decodePlayerNames.push_back(Utils::UnicodeToAnsi(playerNames[i].buffer));
    }

    return decodePlayerNames;
}

std::vector<float> GetPlayerHealths(std::vector<DWORD_PTR> entitys) {
    std::vector<DWORD_PTR> playerHealthAddresses;
    std::vector<float> playerHealths;

    for (int i = 0; i < entitys.size(); ++i) {
        playerHealthAddresses.push_back(entitys[i] + Offset::Health);
    }

    playerHealths = VmmCore::ReadScatterValues<std::vector<float>>(VmmCore::GetScatterHandle(2), playerHealthAddresses);
    return playerHealths;
}

std::vector<float> GetPlayerGroggyHealths(std::vector<DWORD_PTR> entitys) {
    std::vector<DWORD_PTR> playerGroggyHealthAddresses;
    std::vector<float> playerGroggyHealths;

    for (int i = 0; i < entitys.size(); ++i) {
        playerGroggyHealthAddresses.push_back(entitys[i] + Offset::GroggyHealth);
    }

    playerGroggyHealths = VmmCore::ReadScatterValues<std::vector<float>>(VmmCore::GetScatterHandle(2), playerGroggyHealthAddresses);
    return playerGroggyHealths;
}

std::vector<int> GetPlayerTeamNumbers(std::vector<DWORD_PTR> entitys) {
    std::vector<DWORD_PTR> playerTeamAddresses;
    std::vector<int> playerTeamNumbers;

    for (int i = 0; i < entitys.size(); ++i) {
        playerTeamAddresses.push_back(entitys[i] + Offset::TeamNumber);
    }

    playerTeamNumbers = VmmCore::ReadScatterValues<std::vector<int>>(VmmCore::GetScatterHandle(2), playerTeamAddresses);
    return playerTeamNumbers;
}

std::vector<int> GetPlayerKillCounts(std::vector<DWORD_PTR> ptr) {
    std::vector<DWORD_PTR> addresses;
    std::vector<int> relusts;

    for (int i = 0; i < ptr.size(); ++i) {
        addresses.push_back(ptr[i] + Offset::PlayerSatisitc);
    }

    relusts = VmmCore::ReadScatterValues<std::vector<int>>(VmmCore::GetScatterHandle(2), addresses);

    return relusts;
}

std::vector<int> GetPlayerPartnerLevels(std::vector<DWORD_PTR> ptr) {
    std::vector<DWORD_PTR> addresses;
    std::vector<int> relusts;

    for (int i = 0; i < ptr.size(); ++i) {
        addresses.push_back(ptr[i] + Offset::PartnerLevel);
    }

    relusts = VmmCore::ReadScatterValues<std::vector<int>>(VmmCore::GetScatterHandle(2), addresses);

    return relusts;
}

std::vector<FTransform> GetPlayerHeadBones(std::vector<DWORD_PTR> ptr) {
    std::vector<DWORD_PTR> addresses;
    std::vector<FTransform> relusts;

    for (int i = 0; i < ptr.size(); ++i) {
        addresses.push_back(ptr[i] + (EBoneIndex::forehead * 0x30));
    }

    relusts = VmmCore::ReadScatterValues<std::vector<FTransform>>(VmmCore::GetScatterHandle(2), addresses);

    return relusts;
}
