
#include <GLFW/glfw3.h>
#include <Rocket/Core.h>
#include <Rocket/Controls.h>
#include <Rocket/Debugger.h>
#include <assert.h>
#include <stdio.h>

#include "ShellRenderInterfaceOpenGL.h"
#include "ShellRenderInterfaceExtensions.h"
#include "ShellSystemInterface.h"
#include "ShellFileInterface.h"

ShellRenderInterfaceExtensions *shell_renderer_ext = nullptr;
Rocket::Core::Context *rocket_ctx = nullptr;

void error_callback(int error, const char *desc) {
    printf("%s\n", desc); }

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

void glfw_cursorcb(GLFWwindow *window, double x, double y) {
    rocket_ctx->ProcessMouseMove(x, y, 0); }

void glfw_mousecb(GLFWwindow *window, int button, int action, int mods) {
    int button_rocket = 0;
    switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            button_rocket = 0; break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            button_rocket = 1; break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            button_rocket = 2; break;
    }
    if (action == GLFW_PRESS) {
        rocket_ctx->ProcessMouseButtonDown(button_rocket, 0);
    } else if (action == GLFW_RELEASE) {
        rocket_ctx->ProcessMouseButtonUp(button_rocket, 0);
    }
}

void main_loop() {
    rocket_ctx->Update();

    glClear(GL_COLOR_BUFFER_BIT);
    rocket_ctx->Render();
}

int main() {

    assert(glfwInit());
    glfwSetErrorCallback(error_callback);
    GLFWwindow *window = glfwCreateWindow(800, 600, "IllybLauncher", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        exit(-1);
    }
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, glfw_cursorcb);
    glfwSetMouseButtonCallback(window, glfw_mousecb);
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

    rocket_ctx = Rocket::Core::CreateContext("main", Rocket::Core::Vector2i(800, 600));
    assert(rocket_ctx);
    Rocket::Debugger::Initialise(rocket_ctx);
//    shell_renderer_ext->SetContext(rocket_ctx);

    Rocket::Core::String font_names[4] = {
        "Delicious-Roman.otf", "Delicious-Italic.otf",
        "Delicious-Bold.otf", "Delicious-BoldItalic.otf",
    };
    for (int i = 0; i < sizeof(font_names) / sizeof(Rocket::Core::String); i++) {
        Rocket::Core::FontDatabase::LoadFontFace(font_names[i]); }

    Rocket::Core::ElementDocument *document = rocket_ctx->LoadDocument("main.rml");
    assert(document);
    document->Show();
    document->RemoveReference();

    glClearColor(0, 0, 0, 1);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 800, 600, 0, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glViewport(0, 0, 1600, 1200);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 800, 600, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    rocket_ctx->SetDimensions(Rocket::Core::Vector2i(800, 600));

    while (!glfwWindowShouldClose(window)) {
        main_loop();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    rocket_ctx->RemoveReference();
    Rocket::Core::Shutdown();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}