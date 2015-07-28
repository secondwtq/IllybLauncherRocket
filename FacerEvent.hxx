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

#include <GLFW/glfw3.h>
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
        KeyRelease
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
    unsigned short cursorPosition[2];
    MouseButtonState buttonState;
    ModifierState modifierState;
    long long timestamp = 0;
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

InputEvent::ModifierState generateModifierState(int mods);
InputEvent::ModifierState generateModifierState(GLFWwindow *window);

}
}
}

namespace Rocket {
namespace Core {
class Context;
}
}

namespace Facer {
namespace Middlewares {
namespace Rocket {

int getMouseButtonIdentifier(InputEvent::MouseButton button);

int generateModifierState(const InputEvent::ModifierState& state);
inline int generateModifierState(const InputEvent& state) {
    return generateModifierState(state.modifierState); }

void processEvent(::Rocket::Core::Context *ctx, const InputEvent& event);

}
}
}

#endif // ILLYBLAUNCHER_FACEREVENT_HXX
