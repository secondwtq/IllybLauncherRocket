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

#include <stdio.h>

#include <locale>
#include <codecvt>

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
    ret.setCursorPosition(x, y);
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
    ret.setCursorPosition(xpos, ypos);
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

InputEvent createEventMouseWheel(GLFWwindow *window, double xoffset, double yoffset) {
    InputEvent ret = createEvent(InputEvent::MouseWheel);
    double xpos = 0, ypos = 0;
    glfwGetCursorPos(window, &xpos, &ypos);
    ret.setCursorPosition(xpos, ypos);
    ret.modifierState = generateModifierState(window);

    return ret;
}

InputEvent createEventInputText(GLFWwindow *window, int charpoint) {
    InputEvent ret = createEvent(InputEvent::InputText);

    double xpos = 0, ypos = 0;
    glfwGetCursorPos(window, &xpos, &ypos);
    ret.setCursorPosition(xpos, ypos);
    ret.modifierState = generateModifierState(window);

    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> cv;
    std::string r = cv.to_bytes(charpoint);
    ret.input = r;

    return ret;
}

InputKeyCode getKeyCode(int key) {

    if (key >= GLFW_KEY_A && key <= GLFW_KEY_Z) {
        return static_cast<InputKeyCode>(KLetterCapA + key - GLFW_KEY_A); }

    if (key >= GLFW_KEY_0 && key <= GLFW_KEY_9) {
        return static_cast<InputKeyCode>(KNumber0 + key - GLFW_KEY_0); }

#define _FACER_DEFKEY_(GLFW_KEY, FACER_KEY) case GLFW_KEY: return FACER_KEY;

    switch (key) {
        _FACER_DEFKEY_(GLFW_KEY_SPACE, KSpace);
        _FACER_DEFKEY_(GLFW_KEY_APOSTROPHE, KQuoteSingle);
        _FACER_DEFKEY_(GLFW_KEY_COMMA, KComma)
        _FACER_DEFKEY_(GLFW_KEY_MINUS, KHyphen);
        _FACER_DEFKEY_(GLFW_KEY_PERIOD, KPeriod);
        _FACER_DEFKEY_(GLFW_KEY_SLASH, KSlash);
        _FACER_DEFKEY_(GLFW_KEY_SEMICOLON, KSemicolon);
        _FACER_DEFKEY_(GLFW_KEY_EQUAL, KEqual);
        _FACER_DEFKEY_(GLFW_KEY_LEFT_BRACKET, KBracketLeft);
        _FACER_DEFKEY_(GLFW_KEY_BACKSLASH, KBackslash);
        _FACER_DEFKEY_(GLFW_KEY_RIGHT_BRACKET, KBracketRight);
        _FACER_DEFKEY_(GLFW_KEY_GRAVE_ACCENT, KGraveAccent);

        _FACER_DEFKEY_(GLFW_KEY_ESCAPE, KEscape);
        _FACER_DEFKEY_(GLFW_KEY_ENTER, KReturn);
        _FACER_DEFKEY_(GLFW_KEY_TAB, KTab);
        _FACER_DEFKEY_(GLFW_KEY_BACKSPACE, KBackspace);
        _FACER_DEFKEY_(GLFW_KEY_DELETE, KDelete);

        default:
            return KNull;
    }

#undef _FACER_DEFKEY_
}

InputEvent createEventKey(GLFWwindow *window, int key, int scancode, int action, int mods) {
    InputEvent::EventType type = InputEvent::NoType;
    if (action == GLFW_PRESS) {
        type = InputEvent::KeyPress;
    } else if (action == GLFW_RELEASE) {
        type = InputEvent::KeyRelease; }
    InputEvent ret = createEvent(type);

    double xpos = 0, ypos = 0;
    glfwGetCursorPos(window, &xpos, &ypos);
    ret.setCursorPosition(xpos, ypos);
    ret.modifierState = generateModifierState(mods);

    ret.keyCode = getKeyCode(key);
    if (ret.keyCode == KNull) {
        printf("FacerInputEvent (port source GLFW): Unknown source keycode %d!\n", key); }

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

::Rocket::Core::Input::KeyIdentifier getKeyCode(InputKeyCode code) {
    using ::Rocket::Core::Input::KeyIdentifier;

    if (code >= KLetterCapA && code <= KLetterCapZ) {
        return static_cast<KeyIdentifier>(KeyIdentifier::KI_A + code - KLetterCapA); }
    if (code >= KNumber0 && code <= KNumber9) {
        return static_cast<KeyIdentifier>(KeyIdentifier::KI_0 + code - KNumber0); }

#define _FACER_DEFKEY_(FACER_KEY, ROCKEY) case FACER_KEY: return KeyIdentifier::ROCKEY;

    switch (code) {
        _FACER_DEFKEY_(KSpace, KI_SPACE);

        _FACER_DEFKEY_(KSemicolon, KI_OEM_1);
        _FACER_DEFKEY_(KHyphen, KI_OEM_MINUS);
        _FACER_DEFKEY_(KPeriod, KI_OEM_PERIOD);
        _FACER_DEFKEY_(KSlash, KI_OEM_2);
        _FACER_DEFKEY_(KGraveAccent, KI_OEM_3);
        _FACER_DEFKEY_(KBracketLeft, KI_OEM_4);
        _FACER_DEFKEY_(KBackslash, KI_OEM_5);
        _FACER_DEFKEY_(KBracketRight, KI_OEM_6);
        _FACER_DEFKEY_(KQuoteSingle, KI_OEM_7);

        _FACER_DEFKEY_(KBackspace, KI_BACK);
        _FACER_DEFKEY_(KTab, KI_TAB);
        _FACER_DEFKEY_(KReturn, KI_RETURN);

        _FACER_DEFKEY_(KEscape, KI_ESCAPE);

        _FACER_DEFKEY_(KDelete, KI_DELETE);

        default:
            printf("FacerInputEvent (port target libRocket): Unknown event keycode %d!\n", code);
            return KeyIdentifier::KI_UNKNOWN;
            break;
    }

#undef _FACER_DEFKEY_
}

namespace {

template<class faceT>
class usable_faceT : public faceT {
public:
    using faceT::faceT;

    ~usable_faceT() { }
};

template<typename internT, typename externT, typename stateT>
using usable_codecvt = usable_faceT<std::codecvt<internT, externT, stateT>>;

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

        case InputEvent::MouseWheel:
            ctx->ProcessMouseWheel(10, generateModifierState(event));
            break;

        case InputEvent::KeyPress:
            ctx->ProcessKeyDown(getKeyCode(event.keyCode), generateModifierState(event));
            break;
        case InputEvent::KeyRelease:
            ctx->ProcessKeyUp(getKeyCode(event.keyCode), generateModifierState(event));
            break;

        case InputEvent::InputText: {
            std::wstring_convert<usable_codecvt<char16_t, char, std::mbstate_t>, char16_t> cv;
            std::u16string u16 = cv.from_bytes(event.input);
            ctx->ProcessTextInput(::Rocket::Core::String(u16.length() * 2, reinterpret_cast<const char *>(u16.c_str())));
        }
            break;

        default:
            break;
    }
}

}
}
}
