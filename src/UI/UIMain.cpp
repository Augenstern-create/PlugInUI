#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#include <tchar.h>

#include "UIMain.h"
#include "Include.h"
#include "imgui_radar.h"
#include "imgui_module.h"

// imgui
namespace ImGui {
ImTextureID Map = nullptr;
}  // namespace ImGui

struct WindowData {
    WNDCLASSEXW wc;
    HWND hwnd;
    ID3D11Device* device;
    ID3D11DeviceContext* deviceContext;
    IDXGISwapChain* swapChain;
    ID3D11RenderTargetView* RenderTargetView;
    float width;
    float height;
    float clear_color_with_alpha[4];

    WindowData()
        : hwnd(nullptr),
          device(nullptr),
          deviceContext(nullptr),
          swapChain(nullptr),
          RenderTargetView(nullptr),
          width((float)GetSystemMetrics(SM_CXSCREEN)),
          height((float)GetSystemMetrics(SM_CYSCREEN)),
          clear_color_with_alpha{0.45f, 0.55f, 0.60f, 1.00f} {
        ZeroMemory(&wc, sizeof(wc));
    }
    void Release() {
        if (RenderTargetView) {
            RenderTargetView->Release();
            RenderTargetView = nullptr;
        }
        if (swapChain) {
            swapChain->Release();
            swapChain = nullptr;
        }
        if (deviceContext) {
            deviceContext->Release();
            deviceContext = nullptr;
        }
        if (device) {
            device->Release();
            device = nullptr;
        }
        if (hwnd) {
            ::DestroyWindow(hwnd);
            hwnd = nullptr;
        }
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        width = 0;
        height = 0;
    }
};

// 初始化窗体数据
WindowData mainWndData;
WindowData detachedWndData;

bool CreateRenderTargetForWindow(WindowData* WndData) {
    ID3D11Texture2D* pBackBuffer = nullptr;
    if (SUCCEEDED(WndData->swapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer)))) {
        if (FAILED(WndData->device->CreateRenderTargetView(pBackBuffer, nullptr, &WndData->RenderTargetView))) {
            LOGE("Window rendering failure...\n");
            return false;
        }
        pBackBuffer->Release();
        return true;
    }
    return false;
}

bool CreateDeviceD3D(WindowData* WndData) {
    // Setup swap chain
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
    sd.OutputWindow = WndData->hwnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    // createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_0,
    };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags,
                                                featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &WndData->swapChain,
                                                &WndData->device, &featureLevel, &WndData->deviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED)  // Try high-performance WARP software driver if hardware is not available.
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags,
                                            featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &WndData->swapChain,
                                            &WndData->device, &featureLevel, &WndData->deviceContext);
    if (res != S_OK) return false;

    return CreateRenderTargetForWindow(WndData);
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_SYSCOMMAND:
            if ((wParam & 0xfff0) == SC_KEYMENU)  // Disable ALT application menu
                return 0;
            break;
        case WM_DESTROY:
            ::PostQuitMessage(0);
            return 0;
        case WM_CLOSE:
            DestroyWindow(hWnd);
            return 0;
    }

    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) return true;

    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

LRESULT WINAPI DetachedWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_SYSCOMMAND:
            if ((wParam & 0xfff0) == SC_KEYMENU)  // Disable ALT application menu
                return 0;
            break;
        case WM_DESTROY:
            ::PostQuitMessage(0);
            return 0;
        case WM_CLOSE:
            DestroyWindow(hWnd);
            return 0;
    }

    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) return true;

    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

bool InitMainWindow(WindowData* WindData) {
    WindData->wc = {sizeof(WindData->wc),     CS_CLASSDC, MainWndProc, 0L,      0L,
                    GetModuleHandle(nullptr), nullptr,    nullptr,     nullptr, nullptr,
                    L"MainWindowClass",       nullptr};
    ::RegisterClassExW(&WindData->wc);
    WindData->hwnd = ::CreateWindowW(WindData->wc.lpszClassName, L"ImGui Tool", WS_OVERLAPPEDWINDOW,
                                     (WindData->width - WindData->height) * 0.5f, 0, WindData->height, WindData->height,
                                     NULL, NULL, WindData->wc.hInstance, NULL);
    if (!CreateDeviceD3D(WindData)) {
        WindData->Release();
        LOGE("Window hwndMain initialization failed. Procedure...\n");
        return false;
    }
    // Disable resizing for the main window
    LONG_PTR style = GetWindowLongPtr(WindData->hwnd, GWL_STYLE);
    style &= ~WS_THICKFRAME;  // Disable resizing
    SetWindowLongPtr(WindData->hwnd, GWL_STYLE, style);

    // Show the main window
    ::ShowWindow(WindData->hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(WindData->hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

    ImGui::StyleColorsDark();
    ImGui_ImplWin32_Init(WindData->hwnd);
    ImGui_ImplDX11_Init(WindData->device, WindData->deviceContext);
    return true;
}

bool InitDetachedWindow(WindowData* DetachedWindData) {
    DetachedWindData->wc = {sizeof(DetachedWindData->wc), CS_CLASSDC, DetachedWndProc, 0L,      0L,
                            GetModuleHandle(nullptr),     nullptr,    nullptr,         nullptr, nullptr,
                            L"DetachedWindowClass",       nullptr};
    ::RegisterClassExW(&DetachedWindData->wc);
    DetachedWindData->hwnd = ::CreateWindowW(DetachedWindData->wc.lpszClassName, L"ImGui Debug", WS_OVERLAPPEDWINDOW, 0,
                                             0, DetachedWindData->width / 2, DetachedWindData->height, NULL, NULL,
                                             DetachedWindData->wc.hInstance, NULL);
    if (!CreateDeviceD3D(DetachedWindData)) {
        DetachedWindData->Release();
        LOGE("Window hwndMain initialization failed. Procedure...\n");
        return false;
    }
    // Disable resizing for the main window
    LONG_PTR style = GetWindowLongPtr(DetachedWindData->hwnd, GWL_STYLE);
    style &= ~WS_THICKFRAME;  // Disable resizing
    SetWindowLongPtr(DetachedWindData->hwnd, GWL_STYLE, style);

    // Show the main window
    ::ShowWindow(DetachedWindData->hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(DetachedWindData->hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

    ImGui::StyleColorsDark();
    ImGui_ImplWin32_Init(DetachedWindData->hwnd);
    ImGui_ImplDX11_Init(DetachedWindData->device, DetachedWindData->deviceContext);
    float clearColor[] = {0.0f, 0.0f, 0.0f, 0.0f};
    memcpy(DetachedWindData->clear_color_with_alpha, clearColor, sizeof(DetachedWindData->clear_color_with_alpha));

    return true;
}

bool InitImGui() {
    // // Create main window
    if (!InitMainWindow(&mainWndData)) return false;
    // if (!InitDetachedWindow(&detachedWndData)) return false;
    return true;
}

// 渲染 ImGui 内容
void MainRenderImGui(WindowData* WindData) {
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    // Main ImGui content for the main window
    ImGui::Begin("Main Window");
    ImGui::Text("Hello from Main Window!");
    ImGui::End();

    ImGui::Render();
    // Render ImGui for the main window
    WindData->deviceContext->OMSetRenderTargets(1, &WindData->RenderTargetView, nullptr);
    WindData->deviceContext->ClearRenderTargetView(WindData->RenderTargetView,
                                                   (float*)&WindData->clear_color_with_alpha);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    WindData->swapChain->Present(1, 0);  // Present with vsync
    //  WindData->swapChain->Present(0, 0); // Present without vsync
}
void DetachedRenderImGui(WindowData* WindData) {
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    // Main ImGui content for the main window
    ImGui::Begin("Detached Window");
    ImGui::Text("Hello from Detached Window!");
    ImGui::End();

    ImGui::Render();
    // Render ImGui for the main window
    WindData->deviceContext->OMSetRenderTargets(1, &WindData->RenderTargetView, nullptr);
    WindData->deviceContext->ClearRenderTargetView(WindData->RenderTargetView,
                                                   (float*)&WindData->clear_color_with_alpha);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    WindData->swapChain->Present(1, 0);  // Present with vsync
    // WindData->swapChain->Present(0, 0);  // Present without vsync
}

void UpdateImGui() {
    // Main loop
    bool done = false;
    ImGuiIO& io = ImGui::GetIO();
    // Message loop
    while (!done) {
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT) done = true;
        }

        MainRenderImGui(&mainWndData);
        // DetachedRenderImGui(&detachedWndData);
    }
}

void ReleaseImGui() {
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    mainWndData.Release();
    // detachedWndData.Release();
}

void UIPlay() {
    InitImGui();
    UpdateImGui();
    ReleaseImGui();
}