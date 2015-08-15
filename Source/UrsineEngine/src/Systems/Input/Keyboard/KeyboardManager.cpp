/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** KeyboardManager.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "KeyboardManager.h"

#include <SDL_keyboard.h>

namespace Ursine
{   
    KeyboardManager *gKeyboardManager = nullptr;

    // virtual keyboard keys mapped to scan codes
    static uint key_to_scan[KEY_NUM_KEYS];

    // scan codes mapped to virtual keyboard
    static uint scan_to_key[SDL_NUM_SCANCODES];

    KeyboardManager::KeyboardManager(void)
        : EventDispatcher(this)
        , _state_current(SDL_GetKeyboardState(nullptr))
    {
        gKeyboardManager = this;

        // initialize to zero
        memset(_state_queue, 0, SDL_NUM_SCANCODES);
        memset(_state_last, 0, SDL_NUM_SCANCODES);

#pragma region Generated Key Mapping
        key_to_scan[KEY_BACK] = SDL_SCANCODE_BACKSPACE;
        key_to_scan[KEY_TAB] = SDL_SCANCODE_TAB;
        key_to_scan[KEY_CLEAR] = SDL_SCANCODE_CLEAR;
        key_to_scan[KEY_RETURN] = SDL_SCANCODE_RETURN;
        key_to_scan[KEY_PAUSE] = SDL_SCANCODE_PAUSE;
        key_to_scan[KEY_ESCAPE] = SDL_SCANCODE_ESCAPE;
        key_to_scan[KEY_SPACE] = SDL_SCANCODE_SPACE;
        key_to_scan[KEY_OEM_COMMA] = SDL_SCANCODE_COMMA;
        key_to_scan[KEY_OEM_MINUS] = SDL_SCANCODE_MINUS;
        key_to_scan[KEY_OEM_PERIOD] = SDL_SCANCODE_PERIOD;
        key_to_scan[KEY_OEM_2] = SDL_SCANCODE_SLASH;
        key_to_scan[KEY_0] = SDL_SCANCODE_0;
        key_to_scan[KEY_1] = SDL_SCANCODE_1;
        key_to_scan[KEY_2] = SDL_SCANCODE_2;
        key_to_scan[KEY_3] = SDL_SCANCODE_3;
        key_to_scan[KEY_4] = SDL_SCANCODE_4;
        key_to_scan[KEY_5] = SDL_SCANCODE_5;
        key_to_scan[KEY_6] = SDL_SCANCODE_6;
        key_to_scan[KEY_7] = SDL_SCANCODE_7;
        key_to_scan[KEY_8] = SDL_SCANCODE_8;
        key_to_scan[KEY_9] = SDL_SCANCODE_9;
        key_to_scan[KEY_OEM_1] = SDL_SCANCODE_SEMICOLON;
        key_to_scan[KEY_OEM_PLUS] = SDL_SCANCODE_EQUALS;
        key_to_scan[KEY_OEM_4] = SDL_SCANCODE_LEFTBRACKET;
        key_to_scan[KEY_OEM_5] = SDL_SCANCODE_BACKSLASH;
        key_to_scan[KEY_OEM_6] = SDL_SCANCODE_RIGHTBRACKET;
        key_to_scan[KEY_TILDE] = SDL_SCANCODE_GRAVE;
        key_to_scan[KEY_A] = SDL_SCANCODE_A;
        key_to_scan[KEY_B] = SDL_SCANCODE_B;
        key_to_scan[KEY_C] = SDL_SCANCODE_C;
        key_to_scan[KEY_D] = SDL_SCANCODE_D;
        key_to_scan[KEY_E] = SDL_SCANCODE_E;
        key_to_scan[KEY_F] = SDL_SCANCODE_F;
        key_to_scan[KEY_G] = SDL_SCANCODE_G;
        key_to_scan[KEY_H] = SDL_SCANCODE_H;
        key_to_scan[KEY_I] = SDL_SCANCODE_I;
        key_to_scan[KEY_J] = SDL_SCANCODE_J;
        key_to_scan[KEY_K] = SDL_SCANCODE_K;
        key_to_scan[KEY_L] = SDL_SCANCODE_L;
        key_to_scan[KEY_M] = SDL_SCANCODE_M;
        key_to_scan[KEY_N] = SDL_SCANCODE_N;
        key_to_scan[KEY_O] = SDL_SCANCODE_O;
        key_to_scan[KEY_P] = SDL_SCANCODE_P;
        key_to_scan[KEY_Q] = SDL_SCANCODE_Q;
        key_to_scan[KEY_R] = SDL_SCANCODE_R;
        key_to_scan[KEY_S] = SDL_SCANCODE_S;
        key_to_scan[KEY_T] = SDL_SCANCODE_T;
        key_to_scan[KEY_U] = SDL_SCANCODE_U;
        key_to_scan[KEY_V] = SDL_SCANCODE_V;
        key_to_scan[KEY_W] = SDL_SCANCODE_W;
        key_to_scan[KEY_X] = SDL_SCANCODE_X;
        key_to_scan[KEY_Y] = SDL_SCANCODE_Y;
        key_to_scan[KEY_Z] = SDL_SCANCODE_Z;
        key_to_scan[KEY_NUMPAD0] = SDL_SCANCODE_KP_0;
        key_to_scan[KEY_NUMPAD1] = SDL_SCANCODE_KP_1;
        key_to_scan[KEY_NUMPAD2] = SDL_SCANCODE_KP_2;
        key_to_scan[KEY_NUMPAD3] = SDL_SCANCODE_KP_3;
        key_to_scan[KEY_NUMPAD4] = SDL_SCANCODE_KP_4;
        key_to_scan[KEY_NUMPAD5] = SDL_SCANCODE_KP_5;
        key_to_scan[KEY_NUMPAD6] = SDL_SCANCODE_KP_6;
        key_to_scan[KEY_NUMPAD7] = SDL_SCANCODE_KP_7;
        key_to_scan[KEY_NUMPAD8] = SDL_SCANCODE_KP_8;
        key_to_scan[KEY_NUMPAD9] = SDL_SCANCODE_KP_9;
        key_to_scan[KEY_DECIMAL] = SDL_SCANCODE_KP_PERIOD;
        key_to_scan[KEY_DIVIDE] = SDL_SCANCODE_KP_DIVIDE;
        key_to_scan[KEY_MULTIPLY] = SDL_SCANCODE_KP_MULTIPLY;
        key_to_scan[KEY_SUBTRACT] = SDL_SCANCODE_KP_MINUS;
        key_to_scan[KEY_ADD] = SDL_SCANCODE_KP_PLUS;
        key_to_scan[KEY_SEPARATOR] = SDL_SCANCODE_KP_ENTER;
        key_to_scan[KEY_UNKNOWN] = SDL_SCANCODE_KP_EQUALS;
        key_to_scan[KEY_UP] = SDL_SCANCODE_UP;
        key_to_scan[KEY_DOWN] = SDL_SCANCODE_DOWN;
        key_to_scan[KEY_RIGHT] = SDL_SCANCODE_RIGHT;
        key_to_scan[KEY_LEFT] = SDL_SCANCODE_LEFT;
        key_to_scan[KEY_INSERT] = SDL_SCANCODE_INSERT;
        key_to_scan[KEY_HOME] = SDL_SCANCODE_HOME;
        key_to_scan[KEY_END] = SDL_SCANCODE_END;
        key_to_scan[KEY_PRIOR] = SDL_SCANCODE_PAGEUP;
        key_to_scan[KEY_NEXT] = SDL_SCANCODE_PAGEDOWN;
        key_to_scan[KEY_F1] = SDL_SCANCODE_F1;
        key_to_scan[KEY_F2] = SDL_SCANCODE_F2;
        key_to_scan[KEY_F3] = SDL_SCANCODE_F3;
        key_to_scan[KEY_F4] = SDL_SCANCODE_F4;
        key_to_scan[KEY_F5] = SDL_SCANCODE_F5;
        key_to_scan[KEY_F6] = SDL_SCANCODE_F6;
        key_to_scan[KEY_F7] = SDL_SCANCODE_F7;
        key_to_scan[KEY_F8] = SDL_SCANCODE_F8;
        key_to_scan[KEY_F9] = SDL_SCANCODE_F9;
        key_to_scan[KEY_F10] = SDL_SCANCODE_F10;
        key_to_scan[KEY_F11] = SDL_SCANCODE_F11;
        key_to_scan[KEY_F12] = SDL_SCANCODE_F12;
        key_to_scan[KEY_F13] = SDL_SCANCODE_F13;
        key_to_scan[KEY_F14] = SDL_SCANCODE_F14;
        key_to_scan[KEY_F15] = SDL_SCANCODE_F15;
        key_to_scan[KEY_VOLUME_UP] = SDL_SCANCODE_VOLUMEUP;
        key_to_scan[KEY_VOLUME_DOWN] = SDL_SCANCODE_VOLUMEDOWN;
        key_to_scan[KEY_VOLUME_MUTE] = SDL_SCANCODE_AUDIOMUTE;
        key_to_scan[KEY_MEDIA_NEXT_TRACK] = SDL_SCANCODE_AUDIONEXT;
        key_to_scan[KEY_MEDIA_PREV_TRACK] = SDL_SCANCODE_AUDIOPREV;
        key_to_scan[KEY_MEDIA_PLAY_PAUSE] = SDL_SCANCODE_AUDIOPLAY;
        key_to_scan[KEY_MEDIA_STOP] = SDL_SCANCODE_AUDIOSTOP;
        key_to_scan[KEY_NUMLOCK] = SDL_SCANCODE_NUMLOCKCLEAR;
        key_to_scan[KEY_CAPITAL] = SDL_SCANCODE_CAPSLOCK;
        key_to_scan[KEY_SCROLL] = SDL_SCANCODE_SCROLLLOCK;
        key_to_scan[KEY_RSHIFT] = SDL_SCANCODE_RSHIFT;
        key_to_scan[KEY_LSHIFT] = SDL_SCANCODE_LSHIFT;
        key_to_scan[KEY_RCONTROL] = SDL_SCANCODE_RCTRL;
        key_to_scan[KEY_LCONTROL] = SDL_SCANCODE_LCTRL;
        key_to_scan[KEY_RMENU] = SDL_SCANCODE_RALT;
        key_to_scan[KEY_LMENU] = SDL_SCANCODE_LALT;
        key_to_scan[KEY_LWIN] = SDL_SCANCODE_LGUI;
        key_to_scan[KEY_RWIN] = SDL_SCANCODE_RGUI;
        key_to_scan[KEY_MODECHANGE] = SDL_SCANCODE_MODE;
        key_to_scan[KEY_ACCEPT] = SDL_SCANCODE_APPLICATION;
        key_to_scan[KEY_HELP] = SDL_SCANCODE_HELP;
        key_to_scan[KEY_SNAPSHOT] = SDL_SCANCODE_PRINTSCREEN;
        key_to_scan[KEY_VEXECUTE] = SDL_SCANCODE_SYSREQ;
        key_to_scan[KEY_DELETE] = SDL_SCANCODE_DELETE;

        scan_to_key[SDL_SCANCODE_BACKSPACE] = KEY_BACK;
        scan_to_key[SDL_SCANCODE_TAB] = KEY_TAB;
        scan_to_key[SDL_SCANCODE_CLEAR] = KEY_CLEAR;
        scan_to_key[SDL_SCANCODE_RETURN] = KEY_RETURN;
        scan_to_key[SDL_SCANCODE_PAUSE] = KEY_PAUSE;
        scan_to_key[SDL_SCANCODE_ESCAPE] = KEY_ESCAPE;
        scan_to_key[SDL_SCANCODE_SPACE] = KEY_SPACE;
        scan_to_key[SDL_SCANCODE_COMMA] = KEY_OEM_COMMA;
        scan_to_key[SDL_SCANCODE_MINUS] = KEY_OEM_MINUS;
        scan_to_key[SDL_SCANCODE_PERIOD] = KEY_OEM_PERIOD;
        scan_to_key[SDL_SCANCODE_SLASH] = KEY_OEM_2;
        scan_to_key[SDL_SCANCODE_0] = KEY_0;
        scan_to_key[SDL_SCANCODE_1] = KEY_1;
        scan_to_key[SDL_SCANCODE_2] = KEY_2;
        scan_to_key[SDL_SCANCODE_3] = KEY_3;
        scan_to_key[SDL_SCANCODE_4] = KEY_4;
        scan_to_key[SDL_SCANCODE_5] = KEY_5;
        scan_to_key[SDL_SCANCODE_6] = KEY_6;
        scan_to_key[SDL_SCANCODE_7] = KEY_7;
        scan_to_key[SDL_SCANCODE_8] = KEY_8;
        scan_to_key[SDL_SCANCODE_9] = KEY_9;
        scan_to_key[SDL_SCANCODE_SEMICOLON] = KEY_OEM_1;
        scan_to_key[SDL_SCANCODE_EQUALS] = KEY_OEM_PLUS;
        scan_to_key[SDL_SCANCODE_LEFTBRACKET] = KEY_OEM_4;
        scan_to_key[SDL_SCANCODE_BACKSLASH] = KEY_OEM_5;
        scan_to_key[SDL_SCANCODE_RIGHTBRACKET] = KEY_OEM_6;
        scan_to_key[SDL_SCANCODE_GRAVE] = KEY_TILDE;
        scan_to_key[SDL_SCANCODE_A] = KEY_A;
        scan_to_key[SDL_SCANCODE_B] = KEY_B;
        scan_to_key[SDL_SCANCODE_C] = KEY_C;
        scan_to_key[SDL_SCANCODE_D] = KEY_D;
        scan_to_key[SDL_SCANCODE_E] = KEY_E;
        scan_to_key[SDL_SCANCODE_F] = KEY_F;
        scan_to_key[SDL_SCANCODE_G] = KEY_G;
        scan_to_key[SDL_SCANCODE_H] = KEY_H;
        scan_to_key[SDL_SCANCODE_I] = KEY_I;
        scan_to_key[SDL_SCANCODE_J] = KEY_J;
        scan_to_key[SDL_SCANCODE_K] = KEY_K;
        scan_to_key[SDL_SCANCODE_L] = KEY_L;
        scan_to_key[SDL_SCANCODE_M] = KEY_M;
        scan_to_key[SDL_SCANCODE_N] = KEY_N;
        scan_to_key[SDL_SCANCODE_O] = KEY_O;
        scan_to_key[SDL_SCANCODE_P] = KEY_P;
        scan_to_key[SDL_SCANCODE_Q] = KEY_Q;
        scan_to_key[SDL_SCANCODE_R] = KEY_R;
        scan_to_key[SDL_SCANCODE_S] = KEY_S;
        scan_to_key[SDL_SCANCODE_T] = KEY_T;
        scan_to_key[SDL_SCANCODE_U] = KEY_U;
        scan_to_key[SDL_SCANCODE_V] = KEY_V;
        scan_to_key[SDL_SCANCODE_W] = KEY_W;
        scan_to_key[SDL_SCANCODE_X] = KEY_X;
        scan_to_key[SDL_SCANCODE_Y] = KEY_Y;
        scan_to_key[SDL_SCANCODE_Z] = KEY_Z;
        scan_to_key[SDL_SCANCODE_KP_0] = KEY_NUMPAD0;
        scan_to_key[SDL_SCANCODE_KP_1] = KEY_NUMPAD1;
        scan_to_key[SDL_SCANCODE_KP_2] = KEY_NUMPAD2;
        scan_to_key[SDL_SCANCODE_KP_3] = KEY_NUMPAD3;
        scan_to_key[SDL_SCANCODE_KP_4] = KEY_NUMPAD4;
        scan_to_key[SDL_SCANCODE_KP_5] = KEY_NUMPAD5;
        scan_to_key[SDL_SCANCODE_KP_6] = KEY_NUMPAD6;
        scan_to_key[SDL_SCANCODE_KP_7] = KEY_NUMPAD7;
        scan_to_key[SDL_SCANCODE_KP_8] = KEY_NUMPAD8;
        scan_to_key[SDL_SCANCODE_KP_9] = KEY_NUMPAD9;
        scan_to_key[SDL_SCANCODE_KP_PERIOD] = KEY_DECIMAL;
        scan_to_key[SDL_SCANCODE_KP_DIVIDE] = KEY_DIVIDE;
        scan_to_key[SDL_SCANCODE_KP_MULTIPLY] = KEY_MULTIPLY;
        scan_to_key[SDL_SCANCODE_KP_MINUS] = KEY_SUBTRACT;
        scan_to_key[SDL_SCANCODE_KP_PLUS] = KEY_ADD;
        scan_to_key[SDL_SCANCODE_KP_ENTER] = KEY_SEPARATOR;
        scan_to_key[SDL_SCANCODE_KP_EQUALS] = KEY_UNKNOWN;
        scan_to_key[SDL_SCANCODE_UP] = KEY_UP;
        scan_to_key[SDL_SCANCODE_DOWN] = KEY_DOWN;
        scan_to_key[SDL_SCANCODE_RIGHT] = KEY_RIGHT;
        scan_to_key[SDL_SCANCODE_LEFT] = KEY_LEFT;
        scan_to_key[SDL_SCANCODE_INSERT] = KEY_INSERT;
        scan_to_key[SDL_SCANCODE_HOME] = KEY_HOME;
        scan_to_key[SDL_SCANCODE_END] = KEY_END;
        scan_to_key[SDL_SCANCODE_PAGEUP] = KEY_PRIOR;
        scan_to_key[SDL_SCANCODE_PAGEDOWN] = KEY_NEXT;
        scan_to_key[SDL_SCANCODE_F1] = KEY_F1;
        scan_to_key[SDL_SCANCODE_F2] = KEY_F2;
        scan_to_key[SDL_SCANCODE_F3] = KEY_F3;
        scan_to_key[SDL_SCANCODE_F4] = KEY_F4;
        scan_to_key[SDL_SCANCODE_F5] = KEY_F5;
        scan_to_key[SDL_SCANCODE_F6] = KEY_F6;
        scan_to_key[SDL_SCANCODE_F7] = KEY_F7;
        scan_to_key[SDL_SCANCODE_F8] = KEY_F8;
        scan_to_key[SDL_SCANCODE_F9] = KEY_F9;
        scan_to_key[SDL_SCANCODE_F10] = KEY_F10;
        scan_to_key[SDL_SCANCODE_F11] = KEY_F11;
        scan_to_key[SDL_SCANCODE_F12] = KEY_F12;
        scan_to_key[SDL_SCANCODE_F13] = KEY_F13;
        scan_to_key[SDL_SCANCODE_F14] = KEY_F14;
        scan_to_key[SDL_SCANCODE_F15] = KEY_F15;
        scan_to_key[SDL_SCANCODE_VOLUMEUP] = KEY_VOLUME_UP;
        scan_to_key[SDL_SCANCODE_VOLUMEDOWN] = KEY_VOLUME_DOWN;
        scan_to_key[SDL_SCANCODE_AUDIOMUTE] = KEY_VOLUME_MUTE;
        scan_to_key[SDL_SCANCODE_AUDIONEXT] = KEY_MEDIA_NEXT_TRACK;
        scan_to_key[SDL_SCANCODE_AUDIOPREV] = KEY_MEDIA_PREV_TRACK;
        scan_to_key[SDL_SCANCODE_AUDIOPLAY] = KEY_MEDIA_PLAY_PAUSE;
        scan_to_key[SDL_SCANCODE_AUDIOSTOP] = KEY_MEDIA_STOP;
        scan_to_key[SDL_SCANCODE_NUMLOCKCLEAR] = KEY_NUMLOCK;
        scan_to_key[SDL_SCANCODE_CAPSLOCK] = KEY_CAPITAL;
        scan_to_key[SDL_SCANCODE_SCROLLLOCK] = KEY_SCROLL;
        scan_to_key[SDL_SCANCODE_RSHIFT] = KEY_RSHIFT;
        scan_to_key[SDL_SCANCODE_LSHIFT] = KEY_LSHIFT;
        scan_to_key[SDL_SCANCODE_RCTRL] = KEY_RCONTROL;
        scan_to_key[SDL_SCANCODE_LCTRL] = KEY_LCONTROL;
        scan_to_key[SDL_SCANCODE_RALT] = KEY_RMENU;
        scan_to_key[SDL_SCANCODE_LALT] = KEY_LMENU;
        scan_to_key[SDL_SCANCODE_LGUI] = KEY_LWIN;
        scan_to_key[SDL_SCANCODE_RGUI] = KEY_RWIN;
        scan_to_key[SDL_SCANCODE_MODE] = KEY_MODECHANGE;
        scan_to_key[SDL_SCANCODE_APPLICATION] = KEY_ACCEPT;
        scan_to_key[SDL_SCANCODE_HELP] = KEY_HELP;
        scan_to_key[SDL_SCANCODE_PRINTSCREEN] = KEY_SNAPSHOT;
        scan_to_key[SDL_SCANCODE_SYSREQ] = KEY_VEXECUTE;
        scan_to_key[SDL_SCANCODE_DELETE] = KEY_DELETE;
#pragma endregion

        UAssert(gApplication != nullptr,
            "Application instance doesn't exist.");

        SDL_StartTextInput();

        gApplication->GetInternalEvents().Listener(this)
            .On(SDL_KEYDOWN, &KeyboardManager::onKeyDown)
            .On(SDL_KEYUP, &KeyboardManager::onKeyUp)
            .On(SDL_TEXTINPUT, &KeyboardManager::onText);
    }

    KeyboardManager::~KeyboardManager(void)
    {
        gKeyboardManager = nullptr;

        gApplication->GetInternalEvents().Listener(this)
            .Off(SDL_KEYDOWN, &KeyboardManager::onKeyDown)
            .Off(SDL_KEYUP, &KeyboardManager::onKeyUp)
            .Off(SDL_TEXTINPUT, &KeyboardManager::onText);

        SDL_StopTextInput();
    }

    uint KeyboardManager::ScanToKey(SDL_Scancode scan)
    {
        return scan_to_key[scan];
    }

    uint KeyboardManager::KeyToScan(KeyboardKey key)
    {
        return key_to_scan[key];
    }

    void KeyboardManager::Update(void)
    {
        // copy the last state to the queued state
        memcpy(_state_queue, _state_last, SDL_NUM_SCANCODES);

        // copy the current state to the last state
        memcpy(_state_last, _state_current, SDL_NUM_SCANCODES);

        _state_current = SDL_GetKeyboardState(nullptr);

        _modifiers = static_cast<KeyboardModifiers>(SDL_GetModState());
    }

    bool KeyboardManager::IsDown(KeyboardKey key)
    {
        return _state_last && _state_last[key_to_scan[key]];
    }

    bool KeyboardManager::IsUp(KeyboardKey key)
    {
        return _state_last && !_state_last[key_to_scan[key]];
    }

    bool KeyboardManager::IsTriggeredDown(KeyboardKey key)
    {
        return IsDown(key) && !_state_queue[key_to_scan[key]];
    }

    bool KeyboardManager::IsTriggeredUp(KeyboardKey key)
    {
        return IsUp(key) && _state_queue[key_to_scan[key]];
    }

    KeyboardModifiers KeyboardManager::GetModifiers(void) const
    {
        return _modifiers;
    }

    void KeyboardManager::onKeyDown(EVENT_HANDLER(Application))
    {
        EVENT_ATTRS(Application, InternalApplicationArgs);

        KeyboardKeyArgs e(true,
            static_cast<KeyboardKey>(scan_to_key[args->data->key.keysym.scancode]), 
            args->data->key.repeat);

        Dispatch(KM_KEY_DOWN, &e);
    }

    void KeyboardManager::onKeyUp(EVENT_HANDLER(Application))
    {
        EVENT_ATTRS(Application, InternalApplicationArgs);

        KeyboardKeyArgs e(false,
            static_cast<KeyboardKey>(scan_to_key[args->data->key.keysym.scancode]), 
            args->data->key.repeat);

        Dispatch(KM_KEY_UP, &e);
    }

    void KeyboardManager::onText(EVENT_HANDLER(Application))
    {
        EVENT_ATTRS(Application, InternalApplicationArgs);

        KeyboardTextArgs e(args->data->text.text);

        Dispatch(KM_TEXT_INPUT, &e);
    }
}