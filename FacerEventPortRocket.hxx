//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/08/05.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#ifndef ILLYBLAUNCHER_FACEREVENTPORTROCKET_HXX
#define ILLYBLAUNCHER_FACEREVENTPORTROCKET_HXX

namespace Rocket {
namespace Core {
class Context;
}
}

#include <Rocket/Core/Input.h>

namespace Facer {
namespace Port {
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

#endif // ILLYBLAUNCHER_FACEREVENTPORTROCKET_HXX
