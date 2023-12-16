#include "Process.h"
#include "GNames.h"
#include "Radar.h"

Scene PreviousScene = Scene::FindProcess;

bool IsLobby(const std::string& mapName) {
    return (mapName.find("fail") != std::string::npos || mapName.find("None") != std::string::npos ||
            mapName.find("TslLobby") != std::string::npos);
}

bool FindGameProcess() {
    gameData.PID = VmmCore::GetGamePID();
    gameData.GameBase = VmmCore::ProcessGetModuleBase((LPSTR) "TslGame.exe");
    if (gameData.PID == 0) {
        return false;
    }
    return true;
}

void UpdateGameScene() {
    if (gameData.PID == 0) {
        gameData.Scene = Scene::FindProcess;
    } else {
        int MapID = Decrypt::CIndex(VmmCore::ReadValue<DWORD_PTR>(gameData.UWorld + Offset::ObjID));
        gameData.Map.MapName = GNames::GetNameByID(MapID);

        if (IsLobby(gameData.Map.MapName)) {
            gameData.Scene = Scene::Lobby;
            Data::Clears();
        } else {
            // gameData.hasEnteredGame = true;
            if (VmmCore::GetScatterHandleSize() > 0) {
                gameData.Scene = Scene::Gameing;
            }
        }
    }

    if (gameData.Scene != PreviousScene) {
        PreviousScene = gameData.Scene;
        if (gameData.Scene == Scene::Gameing) {
            GNames::GetGNameLists();
            CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)UpdateEntitys2, NULL, NULL, NULL);
            CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)UpdatePlayers2, NULL, NULL, NULL);
            CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Radar::Update, NULL, NULL, NULL);
        }
    }
}

void ListenGameProcessState() {
    bool gameProcessFound = false;
    bool prevGameProcessFound = false;
    while (true) {
        if (gameData.PID != 0) return;
        if (FindGameProcess()) {
            gameProcessFound = true;
            if (gameProcessFound != prevGameProcessFound) {
                VmmCore::InitScatterHandles();
                Utils::Log(1, "Game process successfully found\n");
            }
        } else {
            gameData.Scene = Scene::FindProcess;
            gameData.GameBase = 0;
            gameData.PID = 0;
            gameProcessFound = false;
            EntityLists["PlayerMale_A_C"].ID = 0;
            // gameData.hasEnteredGame = false;
            Decrypt::DestroyXe();

            if (gameProcessFound != prevGameProcessFound) {
                Utils::Log(0, "Wait for the game to start");
            }
        }
        prevGameProcessFound = gameProcessFound;
        Sleep(5000);
    }
    return;
}