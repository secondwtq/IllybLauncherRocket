//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/08/05.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#include "FacerEvent.hxx"
#include "FacerEventPortRocket.hxx"

#include "platform.h"
#include <Rocket/Core.h>

#include <locale>
#include "thirdpt/codecvt.hxx"

namespace Facer {
namespace Port {
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

#if CUBE_HAS_CODECVT
// stackoverflow.com/questions/11086183/encode-decode-stdstring-to-utf-16
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
#endif

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
            // why negative ...
            ctx->ProcessMouseWheel(-event.wheelDelta[1], generateModifierState(event));
            break;

        case InputEvent::KeyPress:
            ctx->ProcessKeyDown(getKeyCode(event.keyCode), generateModifierState(event));
            break;
        case InputEvent::KeyRelease:
            ctx->ProcessKeyUp(getKeyCode(event.keyCode), generateModifierState(event));
            break;

        case InputEvent::InputText: {
#if CUBE_HAS_CODECVT
            std::wstring_convert<usable_codecvt<char16_t, char, std::mbstate_t>, char16_t> cv;
            std::u16string u16 = cv.from_bytes(event.input);
            ctx->ProcessTextInput(::Rocket::Core::String(u16.length() * 2, reinterpret_cast<const char *>(u16.c_str())));
#endif
        }
            break;

        default:
            break;
    }
}

}
}
}
