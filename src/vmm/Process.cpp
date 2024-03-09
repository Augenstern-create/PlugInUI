#include "Process.h"
#include "GNames.h"
#include "Radar.h"

Scene PreviousScene = Scene::FindProcess;

bool IsLobby(const std::string& mapName) {
    return (mapName.find("fail") != std::string::npos || mapName.find("None") != std::string::npos || mapName.find("TslLobby") != std::string::npos);
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
        gameData.mapRadar.map_id = Decrypt::CIndex(VmmCore::ReadValue<int>(gameData.UWorld + gameData.Offset["ObjID"]));
        gameData.mapRadar.map_name = GNames::GetNameByID(gameData.mapRadar.map_id);
        if (IsLobby(gameData.mapRadar.map_name)) {
            gameData.Scene = Scene::Lobby;
            Data::Clears();
        } else {
            if (VmmCore::GetScatterHandleSize() > 0) {
                gameData.Scene = Scene::Gameing;
            }
        }
    }

    if (gameData.Scene != PreviousScene) {
        PreviousScene = gameData.Scene;
        if (gameData.Scene == Scene::Gameing) {
            GNames::GetGNameLists();
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
            Decrypt::DestroyXe();

            if (gameProcessFound != prevGameProcessFound) {
                Utils::Log(0, "Wait for the game to start");
            }
        }
        prevGameProcessFound = gameProcessFound;
        Sleep(1000);
    }
    return;
}