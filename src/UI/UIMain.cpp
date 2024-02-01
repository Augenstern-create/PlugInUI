#define GLFW_EXPOSE_NATIVE_WIN32
#include "Include.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"

#include "UIMain.h"
#include "imgui_menu.h"
#include "imgui_radar.h"
#include "imgui_module.h"
#include <ShellScalingApi.h>

#include "game/Data.h"
#include "imgui_debug.h"
#include "imgui_skeleton.h"

static std::string CurrentDirectory;
// imgui
namespace ImGui {
unsigned int Map = 0;
}  // namespace ImGui

class WindowData {
   public:
    GLFWwindow* windows_;
    HWND hwnd_;
    LONG style_;
    ImGuiContext* context_;
    ImGuiIO* io_;
    ImFont* font_;
    int width_;
    int height_;
    std::string font_path_;
    std::string font_name_;
    std::string windows_name_;
    WindowData()
        : windows_(nullptr),
          hwnd_(nullptr),
          style_(0),
          context_(nullptr),
          io_(nullptr),
          width_((int)GetSystemMetrics(SM_CXSCREEN)),
          height_((int)GetSystemMetrics(SM_CYSCREEN)) {
        std::filesystem::path currentPath = std::filesystem::current_path();
        std::string run_path = currentPath.string();
        font_path_ = run_path + "\\DouyinSansBold.otf";
    }
    ~WindowData() = default;
    bool InitForm(bool mes) {
        windows_ = glfwCreateWindow(width_, height_, windows_name_.c_str(), NULL, NULL);
        if (!windows_) {
            LOGE("Failed to create GLFW window 1\n");
            return false;
        }
        IMGUI_CHECKVERSION();
        context_ = ImGui::CreateContext();
        io_ = &ImGui::GetIO();
        (void)io_;
        io_->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
        io_->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
        ImGui::SetCurrentContext(context_);
        glfwMakeContextCurrent(windows_);
        ImGui_ImplGlfw_InitForOpenGL(windows_, mes);
        ImGui_ImplOpenGL3_Init();
        hwnd_ = glfwGetWin32Window(windows_);
        style_ = GetWindowLong(hwnd_, GWL_EXSTYLE);
        glfwSwapInterval(0);

        io_->IniFilename = nullptr;
        ImFontConfig Font_cfg;
        Font_cfg.FontDataOwnedByAtlas = false;
        float fontSize = 18.0f;
        font_ = io_->Fonts->AddFontFromFileTTF(font_path_.c_str(), fontSize, &Font_cfg, io_->Fonts->GetGlyphRangesChineseFull());
        if (font_ == NULL) {
            LOGE("Failed to load font from file: %s\n", font_path_.c_str());
            font_ = io_->Fonts->AddFontDefault();
        }
        return true;
    }
    bool SwitchContext() {
        if (context_ && windows_) {
            ImGui::SetCurrentContext(context_);
            glfwMakeContextCurrent(windows_);
            return true;
        }
        return false;
    }
    void Release() {
        if (context_ && windows_) {
            ImGui::SetCurrentContext(context_);
            glfwMakeContextCurrent(windows_);
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext(context_);
            glfwDestroyWindow(windows_);
        } else if (windows_) {
            glfwMakeContextCurrent(windows_);
            glfwDestroyWindow(windows_);
        }
    }
};

static WindowData g_main_form;
static WindowData g_detached_form;
static std::string g_Path = "\\photograph\\Weapon";

int UIPlay() {
    if (!glfwInit()) {
        LOGE("Failed to initialize GLFW\n");
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_FALSE);

    // Create the first window
    int borderAndTitleHeight = (GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYFIXEDFRAME)) * 3;
    g_main_form.width_ = g_main_form.height_ - borderAndTitleHeight;
    g_main_form.height_ -= borderAndTitleHeight;
    g_main_form.windows_name_ = "Read";
    g_main_form.font_name_ = "\\DouyinSansBold.otf";
    if (!g_main_form.InitForm(true)) {
        glfwTerminate();
        return -1;
    }
    g_detached_form.windows_name_ = "Skeleton";
    g_detached_form.font_name_ = "\\DouyinSansBold2.otf";
    if (!g_detached_form.InitForm(false)) {
        g_main_form.Release();
        glfwTerminate();
        return -1;
    }
    g_main_form.SwitchContext();
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        LOGE("Failed to initialize GLAD for window 2\n");
        g_main_form.Release();
        g_detached_form.Release();
        glfwTerminate();
        return -1;
    }
    glfwHideWindow(g_detached_form.windows_);
    glfwFocusWindow(g_main_form.windows_);
    std::filesystem::path currentPath = std::filesystem::current_path();
    std::string CurrentDirectory = currentPath.string();
    g_detached_form.SwitchContext();
    std::string texture_path = CurrentDirectory + g_Path;
    Weapon::LoadTexture(texture_path);
    gameData.ScreenWidth = g_detached_form.width_;
    gameData.ScreenHeight = g_detached_form.height_;
    SetWindowLong(g_detached_form.hwnd_, GWL_STYLE, WS_POPUP);
    // SetWindowLongPtr(g_detached_form.hwnd_, GWL_EXSTYLE, WS_EX_TRANSPARENT);
    while (!glfwWindowShouldClose(g_main_form.windows_) && !glfwWindowShouldClose(g_detached_form.windows_)) {
        //
        g_main_form.SwitchContext();
        glfwPollEvents();
        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();
        bool show_menu_window = true;
        bool show_radar_window = true;
        bool show_debug_window = true;
        bool show_skeleton_window = true;
        ImGui::ShowDebugWindow(&show_debug_window, ImVec2((float)g_main_form.width_, (float)g_main_form.height_));
        if (show_menu_window) {
            ImGui::ShowMenuWindow(&show_menu_window, ImVec2((float)g_main_form.width_, (float)g_main_form.height_));
        }
        if (gameData.Scene == Scene::Gameing) {
            if (ImGui::Map == 0) {
                // gameData.Map.MapName  "Savage_Main"
                std::string Path = CurrentDirectory + "\\photograph\\" + gameData.Map.MapName + ".png";
                // std::string Path = "C:\\Users\\25026\\Pictures\\GenshinImpactCloudGame\\1-1.png";
                ImGui::Map = ImGui::CreateTextureFromImage(Path.c_str());
            }
            ImGui::ShowRadarWindow(&show_radar_window, ImVec2((float)g_main_form.width_, (float)g_main_form.height_));
        }

        ImGui::Render();
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(g_main_form.windows_);

        if (gameData.Scene == Scene::Gameing) {
            if (!glfwGetWindowAttrib(g_detached_form.windows_, GLFW_VISIBLE)) {
                glfwShowWindow(g_detached_form.windows_);
                glfwFocusWindow(g_main_form.windows_);
            }
            g_detached_form.SwitchContext();
            ImGui_ImplGlfw_NewFrame();
            ImGui_ImplOpenGL3_NewFrame();
            ImGui::NewFrame();
            ImGui::ShowSkeletonWindow(&show_skeleton_window, {(float)g_detached_form.width_, (float)g_detached_form.height_});
            ImGui::Render();
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            glfwSwapBuffers(g_detached_form.windows_);

        } else {
            if (glfwGetWindowAttrib(g_detached_form.windows_, GLFW_VISIBLE)) {
                glfwHideWindow(g_detached_form.windows_);
                glfwFocusWindow(g_main_form.windows_);
            }
        }
    }
    g_main_form.Release();
    g_detached_form.Release();
    glfwTerminate();
    return 0;
}

// int UIPlay() {
//     int screenWidth = GetSystemMetrics(SM_CXSCREEN);
//     int screenHeight = GetSystemMetrics(SM_CYSCREEN);
//     int borderAndTitleHeight = (GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYFIXEDFRAME)) * 3;
//     std::cout << screenWidth << "    " << screenHeight << "    " << borderAndTitleHeight << std::endl;
//     bool isShow = false;
//     if (!glfwInit()) {
//         LOGE("Failed to initialize GLFW\n");
//         return -1;
//     }

//     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

//     // Create the first window
//     GLFWwindow* window1 = glfwCreateWindow(screenWidth, screenHeight, "Window 1", NULL, NULL);
//     if (!window1) {
//         LOGE("Failed to create GLFW window 1\n");
//         glfwTerminate();
//         return -1;
//     }
//     IMGUI_CHECKVERSION();
//     ImGuiContext* ctx1 = ImGui::CreateContext();
//     ImGui::SetCurrentContext(ctx1);
//     glfwMakeContextCurrent(window1);
//     ImGui_ImplGlfw_InitForOpenGL(window1, false);
//     ImGui_ImplOpenGL3_Init();

//     // Create the second window
//     GLFWwindow* window2 = glfwCreateWindow(screenHeight - borderAndTitleHeight, screenHeight - borderAndTitleHeight, "Window 2", NULL, NULL);
//     if (!window2) {
//         LOGE("Failed to create GLFW window 2\n");
//         glfwDestroyWindow(window1);
//         glfwTerminate();
//         return -1;
//     }
//     IMGUI_CHECKVERSION();
//     ImGuiContext* ctx2 = ImGui::CreateContext();
//     ImGui::SetCurrentContext(ctx2);
//     glfwMakeContextCurrent(window2);
//     ImGui_ImplGlfw_InitForOpenGL(window2, true);
//     ImGui_ImplOpenGL3_Init();

//     if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
//         LOGE("Failed to initialize GLAD for window 2\n");
//         glfwDestroyWindow(window1);
//         glfwDestroyWindow(window2);
//         glfwTerminate();
//         return -1;
//     }

//     HWND hwnd1 = glfwGetWin32Window(window1);
//     HWND hwnd2 = glfwGetWin32Window(window2);
//     LONG style1 = GetWindowLong(hwnd1, GWL_EXSTYLE);
//     LONG style2 = GetWindowLong(hwnd2, GWL_EXSTYLE);
//     SetWindowPos(hwnd2, NULL, (screenWidth - screenHeight + borderAndTitleHeight) / 2, 2, 0, 0,
//                  SWP_NOSIZE | SWP_NOZORDER);  // 居中显示

//     bool done = false;
//     bool show_skeleton_window = true;
//     while (!done) {
//         done = glfwWindowShouldClose(window1) || glfwWindowShouldClose(window2);
//         if (done) break;
//         glfwPollEvents();
//         if (isShow) {
//             if (!glfwGetWindowAttrib(window1, GLFW_VISIBLE)) {
//                 glfwShowWindow(window1);
//                 glfwFocusWindow(window2);  // 切换焦点到另一个窗口
//             }
//             ImGui::SetCurrentContext(ctx1);
//             glfwMakeContextCurrent(window1);
//             ImGui_ImplGlfw_NewFrame();
//             ImGui_ImplOpenGL3_NewFrame();
//             ImGui::NewFrame();
//             ImGui::ShowSkeletonWindow(&show_skeleton_window, {(float)screenWidth, (float)screenHeight});
//             ImGui::Render();
//             glClear(GL_COLOR_BUFFER_BIT);
//             ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//             glfwSwapBuffers(window1);
//         } else {
//             glfwMakeContextCurrent(NULL);
//             if (glfwGetWindowAttrib(window1, GLFW_VISIBLE)) {
//                 glfwHideWindow(window1);
//             }
//         }

//         ImGui::SetCurrentContext(ctx2);
//         glfwMakeContextCurrent(window2);
//         ImGui_ImplGlfw_NewFrame();
//         ImGui_ImplOpenGL3_NewFrame();
//         ImGui::NewFrame();

//         ImGui::Begin("BEGIN 2 !");
//         ImGui::Text(u8"text2");
//         if (ImGui::Button("full screen")) {
//             // 获取当前监视器
//             GLFWmonitor* currentMonitor = glfwGetWindowMonitor(window1);
//             // 获取主监视器
//             GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
//             // 判断当前窗口是否是全屏模式
//             if (currentMonitor == NULL) {
//                 isShow = true;
//                 // 切换到全屏模式
//                 glfwSetWindowMonitor(window1, primaryMonitor, 0, 0, screenWidth, screenHeight, GLFW_DONT_CARE);
//             } else {
//                 // 切换到非全屏模式
//                 glfwSetWindowMonitor(window1, NULL, 0, 0, 640, 480, GLFW_DONT_CARE);
//             }
//         }
//         if (ImGui::Button("Hide form")) {
//             isShow = !isShow;
//             const char* booln = isShow ? "true" : "false";
//             std::cout << booln << std::endl;
//         }
//         ImGui::End();
//         ImGui::Render();
//         glClear(GL_COLOR_BUFFER_BIT);
//         ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//         glfwSwapBuffers(window2);
//     }

//     // Cleanup for window1
//     ImGui::SetCurrentContext(ctx1);
//     glfwMakeContextCurrent(window1);
//     ImGui_ImplOpenGL3_Shutdown();
//     ImGui_ImplGlfw_Shutdown();
//     ImGui::DestroyContext(ctx1);
//     glfwDestroyWindow(window1);

//     // Cleanup for window2
//     ImGui::SetCurrentContext(ctx2);
//     glfwMakeContextCurrent(window2);
//     ImGui_ImplOpenGL3_Shutdown();
//     ImGui_ImplGlfw_Shutdown();
//     ImGui::DestroyContext(ctx2);
//     glfwDestroyWindow(window2);

//     glfwTerminate();

//     return 0;
// }