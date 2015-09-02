//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/09/02.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#include "GLFWFunctions.hxx"

#include "main.hxx"
#include "FacerEvent.hxx"
#include "FacerEventPortGLFW.hxx"
#include "FacerEventPortRocket.hxx"
#include "thirdpt/glfw.hxx"

#include <stdio.h>

namespace Illyb {
namespace GLFW {

void errorcb(int error, const char *desc) {
    printf("%s\n", desc); }

void keycb(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_ESCAPE) {
            glfwSetWindowShouldClose(window, GL_TRUE);
            return;
        }
    }
    Facer::InputEvent e = Facer::Port::GLFW::createEventKey(window, key, scancode, action, mods);
    Facer::Port::Rocket::processEvent(rocket_ctx, e);
}

void cursorcb(GLFWwindow *window, double x, double y) {
    Facer::InputEvent e = Facer::Port::GLFW::createEventMouseMove(window, x, y);
    Facer::Port::Rocket::processEvent(rocket_ctx, e);
}

void mousecb(GLFWwindow *window, int button, int action, int mods) {
    Facer::InputEvent e = Facer::Port::GLFW::createEventMouseButton(window, button, action, mods);
    Facer::Port::Rocket::processEvent(rocket_ctx, e);
}

void wheelcb(GLFWwindow *window, double xoffset, double yoffset) {
    Facer::InputEvent e = Facer::Port::GLFW::createEventMouseWheel(window, xoffset, yoffset);
    Facer::Port::Rocket::processEvent(rocket_ctx, e);
}

void charcb(GLFWwindow *window, unsigned int codepoint) {
    Facer::InputEvent e = Facer::Port::GLFW::createEventInputText(window, codepoint);
    Facer::Port::Rocket::processEvent(rocket_ctx, e);
}

// github.com/glfw/glfw/issues/310
void centerWindow(GLFWwindow *window) {
    int window_x, window_y;
    glfwGetWindowPos(window, &window_x, &window_y);
    int window_width, window_height;
    glfwGetWindowSize(window, &window_width, &window_height);

    // Halve the window size and use it to adjust the window position to the center of the window
    window_width *= 0.5;
    window_height *= 0.5;

    window_x += window_width;
    window_y += window_height;

    // Get the list of monitors
    int monitors_length;
    GLFWmonitor **monitors = glfwGetMonitors(&monitors_length);

    if (!monitors) {
        // Got no monitors back
        return;
    }

    // Figure out which monitor the window is in
    GLFWmonitor *owner = nullptr;
    int owner_x, owner_y, owner_width, owner_height;

    for (int i = 0; i < monitors_length; i++) {
        // Get the monitor position
        int monitor_x, monitor_y;
        glfwGetMonitorPos(monitors[i], &monitor_x, &monitor_y);

        // Get the monitor size from its video mode
        int monitor_width, monitor_height;
        GLFWvidmode *monitor_vidmode = (GLFWvidmode *) glfwGetVideoMode(monitors[i]);

        if (monitor_vidmode == NULL) {
            // Video mode is required for width and height, so skip this monitor
            continue;
        } else {
            monitor_width = monitor_vidmode->width;
            monitor_height = monitor_vidmode->height;
        }

        // Set the owner to this monitor if the center of the window is within its bounding box
        if ((window_x > monitor_x && window_x < (monitor_x + monitor_width))
            && (window_y > monitor_y && window_y < (monitor_y + monitor_height))) {
            owner = monitors[i];

            owner_x = monitor_x;
            owner_y = monitor_y;

            owner_width = monitor_width;
            owner_height = monitor_height;
        }
    }

    if (owner) {
        // Set the window position to the center of the owner monitor
        glfwSetWindowPos(window, owner_x + (owner_width * 0.5) - window_width,
                owner_y + (owner_height * 0.5) - window_height);
    }
}

/*
HWND hwnd = glfwGetWin32Window(window);
LONG lStyle = GetWindowLong(hwnd, GWL_STYLE);
lStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZE | WS_MAXIMIZE | WS_SYSMENU);
SetWindowLong(hwnd, GWL_STYLE, lStyle);
LONG lExStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
lExStyle &= ~(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
SetWindowLong(hwnd, GWL_EXSTYLE, lExStyle);
SetWindowPos(hwnd, NULL, 0,0,0,0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER);
*/

void setUpWindowEvents(GLFWwindow *window) {
    glfwSetKeyCallback(window, keycb);
    glfwSetCursorPosCallback(window, cursorcb);
    glfwSetMouseButtonCallback(window, mousecb);
    glfwSetScrollCallback(window, wheelcb);
    glfwSetCharCallback(window, charcb);
}

}
}
