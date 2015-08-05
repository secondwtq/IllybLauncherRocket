//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/07/28.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#ifndef ILLYBLAUNCHER_FACEREVENT_HXX
#define ILLYBLAUNCHER_FACEREVENT_HXX

#include <string>

#include "FacerEventKey.hxx"

namespace Facer {

class InputEvent {

public:

    enum EventType {
        NoType = 0,

        MouseMove,
        MousePress,
        MouseRelease,
        MouseWheel,

        KeyPress,
        KeyRelease,

        InputText
    };

    // WebKit the wise!
    enum MouseButton {
        NOBUTTON, LEFT, RIGHT, MIDDLE
    };

    enum ButtonState {
        RELEASED, PRESSED
    };

    struct MouseButtonState {
        ButtonState left = RELEASED;
        ButtonState right = RELEASED;
        ButtonState middle = RELEASED;
    };

    struct ModifierState {
        bool alt = false;
        bool shift = false;
        bool meta = false;
        bool control = false;
    };

    EventType type = EventType::NoType;
    MouseButton primaryButton = MouseButton::NOBUTTON;
    InputKeyCode keyCode = InputKeyCode::KNull;
    unsigned short cursorPosition[2];
    MouseButtonState buttonState;
    ModifierState modifierState;
    std::string input { };
    long long timestamp = 0;

    // just a helper
    template <typename T>
    InputEvent& setCursorPosition(T xpos, T ypos) {
        this->cursorPosition[0] = static_cast<unsigned short>(xpos);
        this->cursorPosition[1] = static_cast<unsigned short>(ypos);
        return *this;
    }
};
}

namespace Facer {
namespace Middlewares {

InputEvent createEvent(InputEvent::EventType type);

}
}

class GLFWwindow;

namespace Facer {
namespace Middlewares {
namespace GLFW {

InputEvent createEventMouseMove(GLFWwindow *window, int x, int y);
InputEvent createEventMouseButton(GLFWwindow *window, int button, int action, int mods);
InputEvent createEventMouseWheel(GLFWwindow *window, double xoffset, double yoffset);
InputEvent createEventKey(GLFWwindow *window, int key, int scancode, int action, int mods);
InputEvent createEventInputText(GLFWwindow *window, int charpoint);

InputEvent::ModifierState generateModifierState(int mods);
InputEvent::ModifierState generateModifierState(GLFWwindow *window);
InputKeyCode getKeyCode(int key);

}
}
}

namespace Rocket {
namespace Core {
class Context;
}
}

#include <Rocket/Core/Input.h>

namespace Facer {
namespace Middlewares {
namespace Rocket {

int getMouseButtonIdentifier(InputEvent::MouseButton button);

int generateModifierState(const InputEvent::ModifierState& state);
inline int generateModifierState(const InputEvent& state) {
    return generateModifierState(state.modifierState); }
::Rocket::Core::Input::KeyIdentifier getKeyCode(InputKeyCode code);

void processEvent(::Rocket::Core::Context *ctx, const InputEvent& event);

}
}
}

#endif // ILLYBLAUNCHER_FACEREVENT_HXX
