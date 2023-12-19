#include "Include.h"
#include "IncludeImGui.h"
#include "SrcVmm.h"
#include "SrcGame.h"
#include "UIMain.h"
#include <windows.h>

static ID3D11Device* g_pd3dDevice = NULL;
static ID3D11DeviceContext* g_pd3dDeviceContext = NULL;
static IDXGISwapChain* g_pSwapChain = NULL;
static ID3D11RenderTargetView* g_mainRenderTargetView = NULL;
GameData gameData;

int Refresh() {
    int refreshCounter = 0;

    while (true) {
        if (gameData.Scene == Scene::Lobby) {
            refreshCounter++;
            if (refreshCounter >= 120) {
                refreshCounter = 0;
                VmmCore::Clear();
                VmmCore::RefreshAll();
            }

        } else if (gameData.Scene == Scene::Gameing) {
            VmmCore::Refresh();
        }

        Sleep(500);
    }
}
void init() {
    gameData.Radar.ZoomFactor = 1.0f;
    // gameData.Map.ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
    // gameData.Map.ScreenHeight = GetSystemMetrics(SM_CYSCREEN);
    // gameData.Map.ScreenCenter = {gameData.Map.ScreenWidth * 0.5f, gameData.Map.ScreenHeight * 0.5f};
    // gameData.Radar.ScreenCenter = {gameData.Map.ScreenWidth * 0.5f, gameData.Map.ScreenHeight * 0.5f};
}

int main(int, char**) {
    std::cout << "Version : " << PROJECT_VERSION_MAJOR << "." << PROJECT_VERSION_MINOR << "." << PROJECT_VERSION_PATCH
              << std::endl;
    // SetConsoleOutputCP(CP_UTF8);
    init();
    // if (!VmmCore::Initialize()) {
    //     Utils::Log(2, "Initialization failure.");
    //     return -1;
    // } else {
    //     std::thread hackThread(HackStart);
    //     // hackThread.join();
    //     hackThread.detach();
    //     std::thread refreshThread(Refresh);
    //     refreshThread.detach();
    // }
    // std::cout << "Initializing Imgui" << std::endl;
    UIPlay();
    exit(0);
    return 0;
}
