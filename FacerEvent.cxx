//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/07/28.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#include "FacerEvent.hxx"

#include <time.h>

namespace Facer {
namespace Middlewares {

InputEvent createEvent(InputEvent::EventType type) {
    InputEvent ret;
    ret.type = type;
    ret.timestamp = time(nullptr);
    return ret;
}

}
}

#include <GLFW/glfw3.h>

namespace Facer {
namespace Middlewares {
namespace GLFW {

InputEvent::ModifierState generateModifierState(int mods) {
    InputEvent::ModifierState ret;

    ret.alt = (mods & GLFW_MOD_ALT) != 0;
    ret.shift = (mods & GLFW_MOD_SHIFT) != 0;
    ret.meta = (mods & GLFW_MOD_SUPER) != 0;
    ret.control = (mods & GLFW_MOD_CONTROL) != 0;

    return ret;
}

InputEvent::ModifierState generateModifierState(GLFWwindow *window) {
    InputEvent::ModifierState ret;

    // TODO: we lost the flexibility of left/right modifier keys
#define _FACER_PRESSED_(name) ((glfwGetKey(window, GLFW_KEY_LEFT_##name) == GLFW_PRESS) || \
    (glfwGetKey(window, GLFW_KEY_RIGHT_##name) == GLFW_PRESS))

    ret.alt = _FACER_PRESSED_(ALT);
    ret.shift = _FACER_PRESSED_(SHIFT);
    ret.meta = _FACER_PRESSED_(SUPER);
    ret.control = _FACER_PRESSED_(CONTROL);

#undef _FACER_PRESSED_

    return ret;
}

InputEvent createEventMouseMove(GLFWwindow *window, int x, int y) {
    InputEvent ret = createEvent(InputEvent::MouseMove);
    ret.cursorPosition[0] = static_cast<unsigned short>(x);
    ret.cursorPosition[1] = static_cast<unsigned short>(y);
    ret.modifierState = generateModifierState(window);
    return ret;
}

InputEvent createEventMouseButton(GLFWwindow *window, int button, int action, int mods) {
    InputEvent::EventType type = InputEvent::NoType;
    if (action == GLFW_PRESS) {
        type = InputEvent::MousePress;
    } else if (action == GLFW_RELEASE) {
        type = InputEvent::MouseRelease; }
    InputEvent ret = createEvent(type);

    double xpos = 0, ypos = 0;
    glfwGetCursorPos(window, &xpos, &ypos);
    ret.cursorPosition[0] = static_cast<unsigned short>(xpos);
    ret.cursorPosition[1] = static_cast<unsigned short>(ypos);
    ret.modifierState = generateModifierState(mods);

    switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            ret.primaryButton = InputEvent::LEFT;
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            ret.primaryButton = InputEvent::RIGHT;
            break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            ret.primaryButton = InputEvent::MIDDLE;
            break;
        default:
            break;
    }

    return ret;
}

}
}
}

#include <Rocket/Core.h>

namespace Facer {
namespace Middlewares {
namespace Rocket {

int generateModifierState(const InputEvent::ModifierState& state) {
    int ret = 0;
    if (state.shift) {
        ret |= ::Rocket::Core::Input::KM_SHIFT; }
    if (state.alt) {
        ret |= ::Rocket::Core::Input::KM_ALT; }
    if (state.meta) {
        ret |= ::Rocket::Core::Input::KM_META; }
    if (state.control) {
        ret |= ::Rocket::Core::Input::KM_CTRL; }

    return ret;
}

int getMouseButtonIdentifier(InputEvent::MouseButton button) {
    switch (button) {
        case InputEvent::LEFT:
            return 0;
        case InputEvent::RIGHT:
            return 1;
        case InputEvent::MIDDLE:
            return 2;
        default:
            return 0;
    }
}

void processEvent(::Rocket::Core::Context *ctx, const InputEvent& event) {
    switch (event.type) {
        case InputEvent::MouseMove:
            ctx->ProcessMouseMove(event.cursorPosition[0], event.cursorPosition[1], generateModifierState(event));
            break;

        case InputEvent::MousePress:
            ctx->ProcessMouseButtonDown(getMouseButtonIdentifier(event.primaryButton), generateModifierState(event));
            break;
        case InputEvent::MouseRelease:
            ctx->ProcessMouseButtonUp(getMouseButtonIdentifier(event.primaryButton), generateModifierState(event));
            break;

        default:
            break;
    }
}

}
}
}
