#include "UIMain.h"
#include "imconfig.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "imgui_internal.h"
#include "imgui_module.h"
#include "imstb_rectpack.h"
#include "imstb_textedit.h"
#include "imstb_truetype.h"
#include <d3d11.h>
#include "imgui_menu.h"
#include "imgui_radar.h"
#include <stb_image.h>
#include "version.h"
#include <filesystem>
#include "game/Constant.h"
#include "game/Data.h"

#pragma comment(lib, "d3d11.lib")

const std::string BalticMainPath = "\\photograph\\Baltic_Main.jpg";

static ID3D11Device* g_pd3dDevice = NULL;
static ID3D11DeviceContext* g_pd3dDeviceContext = NULL;
static IDXGISwapChain* g_pSwapChain = NULL;
static ID3D11RenderTargetView* g_mainRenderTargetView = NULL;
static WNDCLASSEX wc = {sizeof(WNDCLASSEX)};
static HWND hwnd = NULL;
static std::string Path;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
namespace ImGui {
ImTextureID Map = nullptr;
}  // namespace ImGui

void CreateRenderTarget() {
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget() {
    if (g_mainRenderTargetView) {
        g_mainRenderTargetView->Release();
        g_mainRenderTargetView = NULL;
    }
}

bool CreateDeviceD3D(HWND hWnds) {
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnds;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_0,
    };
    if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2,
                                      D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel,
                                      &g_pd3dDeviceContext) != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D() {
    CleanupRenderTarget();
    if (g_pSwapChain) {
        g_pSwapChain->Release();
        g_pSwapChain = NULL;
    }
    if (g_pd3dDeviceContext) {
        g_pd3dDeviceContext->Release();
        g_pd3dDeviceContext = NULL;
    }
    if (g_pd3dDevice) {
        g_pd3dDevice->Release();
        g_pd3dDevice = NULL;
    }
}

#ifndef WM_DPICHANGED
#define WM_DPICHANGED 0x02E0  // From Windows SDK 8.1+ headers
#endif

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnds, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnds, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnds, msg, wParam, lParam)) return true;

    switch (msg) {
        case WM_SIZE:
            if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED) {
                CleanupRenderTarget();
                g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
                CreateRenderTarget();
            }
            return 0;
        case WM_SYSCOMMAND:
            if ((wParam & 0xfff0) == SC_KEYMENU)  // Disable ALT application menu
                return 0;
            break;
        case WM_DESTROY:
            ::PostQuitMessage(0);
            return 0;
        case WM_DPICHANGED:
            if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports) {
                const RECT* suggested_rect = (RECT*)lParam;
                ::SetWindowPos(hWnds, NULL, suggested_rect->left, suggested_rect->top,
                               suggested_rect->right - suggested_rect->left,
                               suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
            }
            break;
    }
    return ::DefWindowProc(hWnds, msg, wParam, lParam);
}

void InitImGui() {
    wc = {sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
          _T("ImGui Tool"),   NULL};
    ::RegisterClassEx(&wc);
    hwnd = ::CreateWindow(wc.lpszClassName, _T("ImGui Tool"), WS_OVERLAPPEDWINDOW, 100, 100, 1, 1, NULL, NULL,
                          wc.hInstance, NULL);

    if (!CreateDeviceD3D(hwnd)) {
        CleanupDeviceD3D();
        ::UnregisterClass(wc.lpszClassName, wc.hInstance);
        return;
    }

    ::ShowWindow(hwnd, SW_HIDE);
    ::UpdateWindow(hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    io.IniFilename = nullptr;
    ImFontConfig Font_cfg;
    Font_cfg.FontDataOwnedByAtlas = false;

    std::filesystem::path currentPath = std::filesystem::current_path();
    std::string run_path = currentPath.string();
    std::string font_path = run_path + "\\DouyinSansBold.otf";
    float fontSize = 14.0f;
    ImFont* font =
        io.Fonts->AddFontFromFileTTF(font_path.c_str(), fontSize, &Font_cfg, io.Fonts->GetGlyphRangesChineseFull());
    if (font == NULL) {
        font = io.Fonts->AddFontDefault();
    }
    Path = run_path;
    ImGui::ShowExampleAppMenuInitializelist();

    // ImGui::maptextures.Baltic_Main = CreateTextureFromImage((run_path + BalticMainPath).c_str(), g_pd3dDevice);
    // std::cout << "Full Path: " << (run_path + BalticMainPath).c_str() << std::endl;

    // std::cout << std::string(Path + "\\photograph\\" + "Range_Main" + ".png").c_str() << std::endl;
}

void ReleaseImGui() {
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);
}

void UpdateImGui() {
    bool done = false;
    ImGuiIO& io = ImGui::GetIO();
    while (!done) {
        MSG msg;
        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT) done = true;
        }
        if (done) break;

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        {
            ImGuiStyle& Style = ImGui::GetStyle();
            float Screen_Width{(float)GetSystemMetrics(SM_CXSCREEN)};   // 获取显示器的宽
            float Screen_Heigth{(float)GetSystemMetrics(SM_CYSCREEN)};  // 获取显示器的高
            static bool show_menu_window = true;
            static bool show_radar_window = true;
            if (show_menu_window) {
                ImGui::ShowMenuWindow(&show_menu_window, ImVec2((float)Screen_Width, (float)Screen_Heigth));
            } else {
                break;
            }
            if (gameData.Scene == Scene::Gameing) {
                if (!ImGui::Map) {
                    ImGui::Map = ImGui::CreateTextureFromImage(
                        std::string(Path + "\\photograph\\" + gameData.Map.MapName + ".png").c_str(), g_pd3dDevice);
                    std::cout << "Map->init();gameData.MapName" << std::endl;
                }
                ImGui::ShowRadarWindow(&show_radar_window, ImVec2((float)Screen_Width, (float)Screen_Heigth));
            }
            // std::cout << (int)gameData.Scene << std::endl;
            ImGui::SetNextWindowPos({Screen_Width / 4, 20}, ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowSize(ImVec2(Screen_Width * 0.5f, Screen_Width * 0.5f + 30), ImGuiCond_FirstUseEver);
            ImGui::Begin("DeBug", NULL, ImGuiWindowFlags_None);
            if (ImGui::BeginTabBar("DeBugTabs", ImGuiTabBarFlags_None)) {
                if (ImGui::BeginTabItem("Main")) {
                    ImGui::Text("PID: %d", gameData.PID);
                    ImGui::Text("GameBase: %lld", gameData.GameBase);
                    ImGui::Text("UWorld: %lld", gameData.UWorld);
                    ImGui::Text("CurrentLevel: %lld", gameData.CurrentLevel);
                    ImGui::Text("GNames: %lld", gameData.GNames);
                    ImGui::Text("GameInstance: %lld", gameData.GameInstance);
                    ImGui::Text("GameState: %lld", gameData.GameState);
                    ImGui::Text("LocalPlayer: %lld", gameData.LocalPlayer);
                    ImGui::Text("PlayerController: %lld", gameData.PlayerController);
                    ImGui::Text("Actor: %lld", gameData.Actor);
                    ImGui::Text("MyHUD: %lld", gameData.MyHUD);
                    ImGui::Text("Scene: %d", (int)gameData.Scene);
                    ImGui::Text("Windows Form Width: %d  Height: %d", gameData.Map.ScreenWidth,
                                gameData.Map.ScreenHeight);
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("entity")) {
                    if (gameData.Scene == Scene::Gameing) {
                        ImGui::Text("ActorCount: %d", gameData.ActorCount);
                        auto Players = Data::GetPlayers();
                        ImGui::Columns(3, nullptr, false);
                        for (auto entity : Players) {
                            ImGui::Text("Name: %s", entity.Name.c_str());
                            ImGui::NextColumn();
                            ImGui::Text("AimOffsets: %f", entity.AimOffsets.y);
                            ImGui::NextColumn();
                            ImGui::Text("Location x: %f  y: %f  z: %f", entity.Location.x, entity.Location.y,
                                        entity.Location.z);
                            ImGui::NextColumn();
                        }
                        ImGui::Columns(1);
                        ImGui::Text("end");
                    }
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Radar")) {
                    if (gameData.Scene == Scene::Gameing) {
                        ImGui::Text("Map name: %s", gameData.Map.MapName.c_str());
                        ImGui::Text("MapWidget: %lld", gameData.Map.MapWidget);
                        ImGui::Text("MapGrid: %lld", gameData.Map.MapGrid);
                        ImGui::Text("Slot: %lld", gameData.Map.Slot);
                        ImGui::Text("size: %f", gameData.Map.MapSize);
                        ImGui::Text("zoomSize: %f", gameData.Map.MapZoomValue);
                        ImGui::Text("ImageMapSize: %f", gameData.Radar.ImageMapSize);
                        ImGui::Text("ZoomFactor: %f", gameData.Map.MapZoomValue);
                        ImGui::Text("Visibility: %d", (int)gameData.Map.Visibility);
                        ImGui::Text("Position x: %f  y: %f", gameData.Map.Position.X, gameData.Map.Position.Y);
                        ImGui::Text("Layout Left: %f  Top: %f  Right: %f  Bottom: %f", gameData.Map.Layout.Left,
                                    gameData.Map.Layout.Top, gameData.Map.Layout.Right, gameData.Map.Layout.Bottom);
                        ImGui::Text("WorldOriginLocation x: %f  y: %f", gameData.Map.WorldOriginLocation.x,
                                    gameData.Map.WorldOriginLocation.y);
                        ImGui::Text("ImageMapSize: %f", gameData.Radar.ImageMapSize);
                        ImGui::Text("ZoomFactor: %f", gameData.Radar.ZoomFactor);
                        ImGui::Text("ScreenCenter x: %f  y: %f", gameData.Radar.ScreenCenter.X,
                                    gameData.Radar.ScreenCenter.Y);
                    }
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("MaxMapLocation")) {
                    if (gameData.Scene == Scene::Gameing) {
                        auto location = gameData.Map.Players;
                        ImGui::Columns(2, nullptr, false);
                        for (auto lisepos : location) {
                            auto pos = lisepos.second;
                            ImGui::Text("entity: %lld", lisepos.first);
                            ImGui::NextColumn();
                            ImGui::Text("RadarScreenLocation X: %f  Y: %f", pos.RadarScreenLocation.X,
                                        pos.RadarScreenLocation.Y);
                            ImGui::NextColumn();
                        }
                        ImGui::Columns(1);
                    }
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("MinMapLocation")) {
                    if (gameData.Scene == Scene::Gameing) {
                        auto location = gameData.Radar.Players;
                        ImGui::Columns(2, nullptr, false);
                        for (auto lisepos : location) {
                            auto pos = lisepos.second;
                            ImGui::Text("entity: %lld", lisepos.first);
                            ImGui::NextColumn();
                            ImGui::Text("RadarLocation X: %f  Y: %f  Z: %f", pos.x, pos.y, pos.z);
                            ImGui::NextColumn();
                        }
                        ImGui::Columns(1);
                    }
                    ImGui::EndTabItem();
                }
            }
            ImGui::End();
        }

        ImGui::Render();
        const float clear_color_with_alpha[4] = {clear_color.x * clear_color.w, clear_color.y * clear_color.w,
                                                 clear_color.z * clear_color.w, clear_color.w};
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }

        g_pSwapChain->Present(1, 0);
    }
}

void UIPlay() {
    InitImGui();
    UpdateImGui();
    ReleaseImGui();
}
