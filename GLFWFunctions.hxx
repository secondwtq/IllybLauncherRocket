//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/09/02.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#ifndef ILLYBLAUNCHER_GLFWFUNCTIONS_HXX
#define ILLYBLAUNCHER_GLFWFUNCTIONS_HXX

struct GLFWwindow;

namespace Illyb {
namespace GLFW {

void errorcb(int error, const char *desc);
void keycb(GLFWwindow *window, int key, int scancode, int action, int mods);
void cursorcb(GLFWwindow *window, double x, double y);
void mousecb(GLFWwindow *window, int button, int action, int mods);
void wheelcb(GLFWwindow *window, double xoffset, double yoffset);
void charcb(GLFWwindow *window, unsigned int codepoint);
void centerWindow(GLFWwindow *window);

void setUpWindowEvents(GLFWwindow *window);

}
}

#endif // ILLYBLAUNCHER_GLFWFUNCTIONS_HXX
