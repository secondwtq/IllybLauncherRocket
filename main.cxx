
#include "main.hxx"

#include <Rocket/Core.h>
#include <Rocket/Controls.h>
#include <Rocket/Debugger.h>
#include <assert.h>

#include "thirdpt/glfw.hxx"
#include "thirdpt/luajit.hxx"

#include "ShellRenderInterfaceOpenGL.h"
#include "ShellSystemInterface.h"
#include "CABALPortRocket.hxx"
//#include "RocketEventInstancer.hxx"

#include <Rocket/Core/Lua/Interpreter.h>
#include <Rocket/Controls/Lua/Controls.h>

#include "INIBinding.hxx"

#include "GLFWFunctions.hxx"
#include "CABAL.hxx"

#include "platform.h"
#include "config.hxx"

ShellRenderInterfaceExtensions *shell_renderer_ext = nullptr;
Rocket::Core::Context *rocket_ctx = nullptr;

void main_loop() {
    rocket_ctx->Update();

    glClear(GL_COLOR_BUFFER_BIT);
    rocket_ctx->Render();
}

int entryPoint(int argc, const char *argv[]) {

    assert(CABAL::init(argv[0]));
    CABAL::addSearchPath(".");
    CABAL::addSearchPath("main.7z");
    CABAL::addSearchPath("fonts.7z");
    CABAL::addSearchPath("images.7z");

    assert(glfwInit());
    glfwSetErrorCallback(Illyb::GLFW::errorcb);
    if (LauncherConfig::instance().noBorder) {
        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_DECORATED, GL_FALSE);
    }
    GLFWwindow *window = glfwCreateWindow(LauncherConfig::instance().width,
          LauncherConfig::instance().height, "IllybLauncher", nullptr, nullptr);
    assert(window);
    if (LauncherConfig::instance().autoCenter) {
        Illyb::GLFW::centerWindow(window); }
    Illyb::GLFW::setUpWindowEvents(window);
    glfwMakeContextCurrent(window);

    ShellRenderInterfaceOpenGL gl_renderer;
    shell_renderer_ext = &gl_renderer;
//    gl_renderer.SetViewport(800, 600);
    Rocket::Core::SetRenderInterface(&gl_renderer);
    ShellSystemInterface sys_interface;
    Rocket::Core::SetSystemInterface(&sys_interface);
    ShellFileInterface file_interface;
    Rocket::Core::SetFileInterface(&file_interface);

    Rocket::Core::Initialise();
    Rocket::Controls::Initialise();
    Rocket::Core::Lua::Interpreter::Initialise();
    Rocket::Controls::Lua::RegisterTypes(Rocket::Core::Lua::Interpreter::GetLuaState());
    registerInterfaceINI(Rocket::Core::Lua::Interpreter::GetLuaState());

    rocket_ctx = Rocket::Core::CreateContext("main", Rocket::Core::Vector2i(
            LauncherConfig::instance().width, LauncherConfig::instance().height));
    assert(rocket_ctx);
    Rocket::Debugger::Initialise(rocket_ctx);

    // Rocket::Core::Factory::RegisterEventListenerInstancer(new RocketEventInstancer());

    Rocket::Core::Lua::Interpreter::LoadFile(Rocket::Core::String("main_script.lua"));

    glClearColor(0, 0, 0, 1);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#ifdef CUBE_PLATFORM_OS_X
    glViewport(0, 0, 1600, 1200);
#else
    glViewport(0, 0, LauncherConfig::instance().width,
            LauncherConfig::instance().height);
#endif
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glOrtho(0, LauncherConfig::instance().width,
            LauncherConfig::instance().height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();

    while (!glfwWindowShouldClose(window)) {
        main_loop();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    rocket_ctx->RemoveReference();
    Rocket::Core::Lua::Interpreter::Shutdown();
    Rocket::Core::Shutdown();
    glfwDestroyWindow(window);
    CABAL::dispose();
    glfwTerminate();
    return 0;
}

#ifndef CUBE_PLATFORM_WINDOWS

int main(int argc, const char *argv[]) {
    return entryPoint(argc, argv); }

#else

#include <vector>
#include <string>

// stackoverflow.com/questions/27363851/pass-winmain-or-wwinmain-arguments-to-normal-main
#include <windows.h>
#include <shellapi.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hpInstance, LPSTR nCmdLine, int iCmdShow) {
    int w_argc = 0;
    LPWSTR* w_argv = CommandLineToArgvW(GetCommandLineW(), &w_argc);
    int ret = -1;
    if (w_argv) {
        std::vector<std::string> argv_buf;
        argv_buf.reserve(w_argc);

        for (int i = 0; i < w_argc; ++i) {
            int w_len = lstrlenW(w_argv[i]);
            int len = WideCharToMultiByte(CP_ACP, 0, w_argv[i], w_len, NULL, 0, NULL, NULL);
            std::string s;
            s.resize(len);
            WideCharToMultiByte(CP_ACP, 0, w_argv[i], w_len, &s[0], len, NULL, NULL);
            argv_buf.push_back(s);
        }

        std::vector<const char *> argv;
        argv.reserve(argv_buf.size());
        for (auto i : argv_buf) {
            argv.push_back(i.c_str()); }

        ret = entryPoint(argv.size(), &argv[0]);
        LocalFree(w_argv);
    }
    return ret;
}

#endif
