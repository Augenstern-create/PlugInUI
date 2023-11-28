// // Dear ImGui: standalone example application for Win32 + OpenGL 3

// // Learn about Dear ImGui:
// // - FAQ                  https://dearimgui.com/faq
// // - Getting Started      https://dearimgui.com/getting-started
// // - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// // - Introduction, links and more at the top of imgui.cpp

// // This is provided for completeness, however it is strongly recommended you use OpenGL with SDL or GLFW.

// #include "imgui_menu.h"
// #include "imgui_impl_opengl3.h"
// #include "imgui_impl_win32.h"
// #include "imgui_impl_glfw.h"
// #ifndef WIN32_LEAN_AND_MEAN
// #define WIN32_LEAN_AND_MEAN
// #endif
// #include <windows.h>
// #include "glad/glad.h"
// #define GLFW_INCLUDE_NONE
// #include "GLFW/glfw3.h"
// #include <tchar.h>
// #include <stb_image.h>
// #include <filesystem>

// #include "vmmdll.h"

// #include <cstdlib>  // 包含 std::system 函数的头文件

// // Data stored per platform window
// struct WGL_WindowData {
//     HDC hDC;
// };

// // Data
// static HGLRC g_hRC;
// static WGL_WindowData g_MainWindow;
// static int g_Width;
// static int g_Height;

// // Main code
// int main(int, char**) {
//     if (!glfwInit()) {
//         fprintf(stderr, "Failed to initialize GLFW\n");
//         return -1;
//     }
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
//     GLFWwindow* window = glfwCreateWindow(1280, 800, "Dear ImGui GLFW+OpenGL3 Example", NULL, NULL);
//     if (!window) {
//         fprintf(stderr, "Failed to create GLFW window\n");
//         glfwTerminate();
//         return -1;
//     }
//     glfwMakeContextCurrent(window);

//     // VMMDLL_Initialize();
//     // Initialize glad
//     if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
//         fprintf(stderr, "Failed to initialize GLAD\n");
//         glfwTerminate();
//         return -1;
//     }

// LPSTR arguments[] = {
//     "-norefresh", "-device", "FPGA"  // 例如，指定设备为 usb3380
//     // 添加其他参数...
// };

// // 计算参数个数
// DWORD argc = sizeof(arguments) / sizeof(arguments[0]);
// if (argc == 0) {
//     argc = 1;
//     fprintf(stderr, "Initialization arge is: %d\n", argc);
// }

// VMM_HANDLE vmmHandle = VMMDLL_Initialize(argc, arguments);

// if (vmmHandle != NULL) {
//     fprintf(stderr, "Initialization successful\n");
// } else {
//     fprintf(stderr, "Initialization failure error \n");
// }
//     IMGUI_CHECKVERSION();
//     ImGui::CreateContext();
//     ImGuiIO& io = ImGui::GetIO();
//     (void)io;
//     io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
//     io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
//     ImGui::StyleColorsDark();
//     ImGui_ImplGlfw_InitForOpenGL(window, true);
//     ImGui_ImplOpenGL3_Init("#version 330");
//     std::filesystem::path currentPath = std::filesystem::current_path();
//     std::string run_path = currentPath.string();
//     std::string font_path = run_path + "\\DouyinSansBold.otf";
//     float fontSize = 14.0f;
//     ImFont* font =
//         io.Fonts->AddFontFromFileTTF(font_path.c_str(), fontSize, NULL, io.Fonts->GetGlyphRangesChineseFull());
//     if (font == NULL) {
//         font = io.Fonts->AddFontDefault();
//     }
//     // Our state
//     bool show_menu_window = true;
//     bool show_another_window = false;
//     ImVec4 clear_color = ImVec4(0.0f, 1.0f, 0.00f, 1.00f);
//     ImGui::ShowExampleAppMenuInitializelist();
//     std::string but_img_path = run_path + "\\photograph\\but_player.png";
//     ImGui::SetButPlayerImg(ImGui::LoadTexture(but_img_path.c_str()));
//     bool done = false;
//     while (!done) {
//         done = glfwWindowShouldClose(window);
//         if (done) break;
//         glfwPollEvents();

//         // ImGui::SetNextWindowSize(ImVec2(1, 1));  // 设置到一个极小的大小
//         ImGui_ImplOpenGL3_NewFrame();
//         ImGui_ImplGlfw_NewFrame();
//         ImGui::NewFrame();
//         if (show_menu_window) ImGui::ShowMenuWindow(&show_menu_window);
//         ImGui::Render();
//         int display_w, display_h;
//         glfwGetFramebufferSize(window, &display_w, &display_h);
//         glViewport(0, 0, display_w, display_h);
//         glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
//         glClear(GL_COLOR_BUFFER_BIT);
//         ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//         glfwSwapBuffers(window);
//     }
//     ImGui_ImplOpenGL3_Shutdown();
//     ImGui_ImplGlfw_Shutdown();
//     ImGui::DestroyContext();
//     glfwDestroyWindow(window);
//     glfwTerminate();
//     return 0;
// }
