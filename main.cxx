
#include <stdio.h>
#include <GLFW/glfw3.h>
#include <Rocket/Core.h>
#include <Rocket/Controls.h>
#include <Rocket/Debugger.h>
#include <assert.h>
#include <luajit-2.0/lua.hpp>

#include "ShellRenderInterfaceOpenGL.h"
#include "ShellSystemInterface.h"
#include "CABALPortRocket.hxx"
#include "RocketEventInstancer.hxx"

#include <Rocket/Core/Lua/Interpreter.h>
#include <Rocket/Controls/Lua/Controls.h>

#include "INIBinding.hxx"
#include "FacerEvent.hxx"
#include "FacerEventPortGLFW.hxx"
#include "FacerEventPortRocket.hxx"

#include "CABAL.hxx"

#include "config.hxx"

ShellRenderInterfaceExtensions *shell_renderer_ext = nullptr;
Rocket::Core::Context *rocket_ctx = nullptr;

void error_callback(int error, const char *desc) {
    printf("%s\n", desc); }

void glfw_keycb(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_ESCAPE) {
            glfwSetWindowShouldClose(window, GL_TRUE);
            return;
        }
    }
    Facer::InputEvent e = Facer::Port::GLFW::createEventKey(window, key, scancode, action, mods);
    Facer::Port::Rocket::processEvent(rocket_ctx, e);
}

void glfw_cursorcb(GLFWwindow *window, double x, double y) {
    Facer::InputEvent e = Facer::Port::GLFW::createEventMouseMove(window, x, y);
    Facer::Port::Rocket::processEvent(rocket_ctx, e);
}

void glfw_mousecb(GLFWwindow *window, int button, int action, int mods) {
    Facer::InputEvent e = Facer::Port::GLFW::createEventMouseButton(window, button, action, mods);
    Facer::Port::Rocket::processEvent(rocket_ctx, e);
}

void glfw_wheelcb(GLFWwindow *window, double xoffset, double yoffset) {
    Facer::InputEvent e = Facer::Port::GLFW::createEventMouseWheel(window, xoffset, yoffset);
    Facer::Port::Rocket::processEvent(rocket_ctx, e);
}

void glfw_charcb(GLFWwindow *window, unsigned int codepoint) {
    Facer::InputEvent e = Facer::Port::GLFW::createEventInputText(window, codepoint);
    Facer::Port::Rocket::processEvent(rocket_ctx, e);
}

void main_loop() {
    rocket_ctx->Update();

    glClear(GL_COLOR_BUFFER_BIT);
    rocket_ctx->Render();
}

int main(int argc, const char *argv[]) {

    assert(glfwInit());

    assert(CABAL::init(argv[0]));
    CABAL::addSearchPath(".");
    CABAL::addSearchPath("main.7z");
    CABAL::addSearchPath("fonts.7z");
    CABAL::addSearchPath("images.7z");

    glfwSetErrorCallback(error_callback);
    GLFWwindow *window = glfwCreateWindow(LauncherConfig::instance().width,
          LauncherConfig::instance().height, "IllybLauncher", nullptr, nullptr);
    if (!window) {
        glfwTerminate(); exit(-1); }

    glfwSetKeyCallback(window, glfw_keycb);
    glfwSetCursorPosCallback(window, glfw_cursorcb);
    glfwSetMouseButtonCallback(window, glfw_mousecb);
    glfwSetScrollCallback(window, glfw_wheelcb);
    glfwSetCharCallback(window, glfw_charcb);
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

    glViewport(0, 0, 1600, 1200);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, LauncherConfig::instance().width,
            LauncherConfig::instance().height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

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
