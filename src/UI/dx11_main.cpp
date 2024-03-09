#include "Include.h"
#include "IncludeImGui.h"
#include "SrcVmm.h"
#include "SrcGame.h"
#include "UIMain.h"
#include <windows.h>
#include "game/LanguageSwitch.h"
#include "game/offset.h"
#include "vmmdll.h"
#include "leechcore.h"

#include "game/loading_files.h"
static ID3D11Device* g_pd3dDevice = NULL;
static ID3D11DeviceContext* g_pd3dDeviceContext = NULL;
static IDXGISwapChain* g_pSwapChain = NULL;
static ID3D11RenderTargetView* g_mainRenderTargetView = NULL;
GameData gameData;
static std::string g_Path = "\\photograph\\Weapon";

int Refresh() {
    int refreshCounter = 0;

    while (true) {
        if (gameData.Scene == Scene::Lobby) {
            refreshCounter++;
            if (refreshCounter >= (int)(120.0f)) {
                refreshCounter = 0;
                VmmCore::Clear();
                VmmCore::RefreshAll();
            }

        } else if (gameData.Scene == Scene::Gameing) {
            VmmCore::Refresh();
        }
        Sleep(300);
    }
}
void init() {
    ComponentsLists::LoadSet();
    Data::OffsetInit();
    InitLoadingFile();
}

int main(int, char**) {
    // std::cout << "Version : " << PROJECT_VERSION_MAJOR << "." << PROJECT_VERSION_MINOR << "." << PROJECT_VERSION_PATCH << std::endl;
    SetConsoleOutputCP(CP_UTF8);
    init();
    if (!VmmCore::Initialize()) {
        Utils::Log(2, "Initialization failure.");
        return -1;
    } else {
        std::thread refreshThread(Refresh);
        refreshThread.detach();
        std::thread hackThread(HackStart);
        // hackThread.join();
        hackThread.detach();
    }
    std::cout << "Initializing Imgui" << std::endl;

    UIPlay();
    exit(0);
    return 0;
}
