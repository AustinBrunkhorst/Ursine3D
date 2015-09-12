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

namespace ursine
{
    namespace
    {
        // virtual keyboard keys mapped to scan codes
        uint keyToScan[KEY_NUM_KEYS];

        // scan codes mapped to virtual keyboard
        uint scanToKey[SDL_NUM_SCANCODES];
    }

    CORE_SYSTEM_DEFINITION( KeyboardManager );

    KeyboardManager::KeyboardManager(void)
        : EventDispatcher( this )
        , m_stateCurrent( SDL_GetKeyboardState( nullptr ) )
        , m_modifiers( KMD_NONE )
    {
        // initialize to zero
        memset( m_stateQueue, 0, SDL_NUM_SCANCODES );
        memset( m_stateLast, 0, SDL_NUM_SCANCODES );

#pragma region Generated Key Mapping
        keyToScan[ KEY_BACK ] = SDL_SCANCODE_BACKSPACE;
        keyToScan[ KEY_TAB ] = SDL_SCANCODE_TAB;
        keyToScan[ KEY_CLEAR ] = SDL_SCANCODE_CLEAR;
        keyToScan[ KEY_RETURN ] = SDL_SCANCODE_RETURN;
        keyToScan[ KEY_PAUSE ] = SDL_SCANCODE_PAUSE;
        keyToScan[ KEY_ESCAPE ] = SDL_SCANCODE_ESCAPE;
        keyToScan[ KEY_SPACE ] = SDL_SCANCODE_SPACE;
        keyToScan[ KEY_OEM_COMMA ] = SDL_SCANCODE_COMMA;
        keyToScan[ KEY_OEM_MINUS ] = SDL_SCANCODE_MINUS;
        keyToScan[ KEY_OEM_PERIOD ] = SDL_SCANCODE_PERIOD;
        keyToScan[ KEY_OEM_2 ] = SDL_SCANCODE_SLASH;
        keyToScan[ KEY_0 ] = SDL_SCANCODE_0;
        keyToScan[ KEY_1 ] = SDL_SCANCODE_1;
        keyToScan[ KEY_2 ] = SDL_SCANCODE_2;
        keyToScan[ KEY_3 ] = SDL_SCANCODE_3;
        keyToScan[ KEY_4 ] = SDL_SCANCODE_4;
        keyToScan[ KEY_5 ] = SDL_SCANCODE_5;
        keyToScan[ KEY_6 ] = SDL_SCANCODE_6;
        keyToScan[ KEY_7 ] = SDL_SCANCODE_7;
        keyToScan[ KEY_8 ] = SDL_SCANCODE_8;
        keyToScan[ KEY_9 ] = SDL_SCANCODE_9;
        keyToScan[ KEY_OEM_1 ] = SDL_SCANCODE_SEMICOLON;
        keyToScan[ KEY_OEM_PLUS ] = SDL_SCANCODE_EQUALS;
        keyToScan[ KEY_OEM_4 ] = SDL_SCANCODE_LEFTBRACKET;
        keyToScan[ KEY_OEM_5 ] = SDL_SCANCODE_BACKSLASH;
        keyToScan[ KEY_OEM_6 ] = SDL_SCANCODE_RIGHTBRACKET;
        keyToScan[ KEY_TILDE ] = SDL_SCANCODE_GRAVE;
        keyToScan[ KEY_A ] = SDL_SCANCODE_A;
        keyToScan[ KEY_B ] = SDL_SCANCODE_B;
        keyToScan[ KEY_C ] = SDL_SCANCODE_C;
        keyToScan[ KEY_D ] = SDL_SCANCODE_D;
        keyToScan[ KEY_E ] = SDL_SCANCODE_E;
        keyToScan[ KEY_F ] = SDL_SCANCODE_F;
        keyToScan[ KEY_G ] = SDL_SCANCODE_G;
        keyToScan[ KEY_H ] = SDL_SCANCODE_H;
        keyToScan[ KEY_I ] = SDL_SCANCODE_I;
        keyToScan[ KEY_J ] = SDL_SCANCODE_J;
        keyToScan[ KEY_K ] = SDL_SCANCODE_K;
        keyToScan[ KEY_L ] = SDL_SCANCODE_L;
        keyToScan[ KEY_M ] = SDL_SCANCODE_M;
        keyToScan[ KEY_N ] = SDL_SCANCODE_N;
        keyToScan[ KEY_O ] = SDL_SCANCODE_O;
        keyToScan[ KEY_P ] = SDL_SCANCODE_P;
        keyToScan[ KEY_Q ] = SDL_SCANCODE_Q;
        keyToScan[ KEY_R ] = SDL_SCANCODE_R;
        keyToScan[ KEY_S ] = SDL_SCANCODE_S;
        keyToScan[ KEY_T ] = SDL_SCANCODE_T;
        keyToScan[ KEY_U ] = SDL_SCANCODE_U;
        keyToScan[ KEY_V ] = SDL_SCANCODE_V;
        keyToScan[ KEY_W ] = SDL_SCANCODE_W;
        keyToScan[ KEY_X ] = SDL_SCANCODE_X;
        keyToScan[ KEY_Y ] = SDL_SCANCODE_Y;
        keyToScan[ KEY_Z ] = SDL_SCANCODE_Z;
        keyToScan[ KEY_NUMPAD0 ] = SDL_SCANCODE_KP_0;
        keyToScan[ KEY_NUMPAD1 ] = SDL_SCANCODE_KP_1;
        keyToScan[ KEY_NUMPAD2 ] = SDL_SCANCODE_KP_2;
        keyToScan[ KEY_NUMPAD3 ] = SDL_SCANCODE_KP_3;
        keyToScan[ KEY_NUMPAD4 ] = SDL_SCANCODE_KP_4;
        keyToScan[ KEY_NUMPAD5 ] = SDL_SCANCODE_KP_5;
        keyToScan[ KEY_NUMPAD6 ] = SDL_SCANCODE_KP_6;
        keyToScan[ KEY_NUMPAD7 ] = SDL_SCANCODE_KP_7;
        keyToScan[ KEY_NUMPAD8 ] = SDL_SCANCODE_KP_8;
        keyToScan[ KEY_NUMPAD9 ] = SDL_SCANCODE_KP_9;
        keyToScan[ KEY_DECIMAL ] = SDL_SCANCODE_KP_PERIOD;
        keyToScan[ KEY_DIVIDE ] = SDL_SCANCODE_KP_DIVIDE;
        keyToScan[ KEY_MULTIPLY ] = SDL_SCANCODE_KP_MULTIPLY;
        keyToScan[ KEY_SUBTRACT ] = SDL_SCANCODE_KP_MINUS;
        keyToScan[ KEY_ADD ] = SDL_SCANCODE_KP_PLUS;
        keyToScan[ KEY_SEPARATOR ] = SDL_SCANCODE_KP_ENTER;
        keyToScan[ KEY_UNKNOWN ] = SDL_SCANCODE_KP_EQUALS;
        keyToScan[ KEY_UP ] = SDL_SCANCODE_UP;
        keyToScan[ KEY_DOWN ] = SDL_SCANCODE_DOWN;
        keyToScan[ KEY_RIGHT ] = SDL_SCANCODE_RIGHT;
        keyToScan[ KEY_LEFT ] = SDL_SCANCODE_LEFT;
        keyToScan[ KEY_INSERT ] = SDL_SCANCODE_INSERT;
        keyToScan[ KEY_HOME ] = SDL_SCANCODE_HOME;
        keyToScan[ KEY_END ] = SDL_SCANCODE_END;
        keyToScan[ KEY_PRIOR ] = SDL_SCANCODE_PAGEUP;
        keyToScan[ KEY_NEXT ] = SDL_SCANCODE_PAGEDOWN;
        keyToScan[ KEY_F1 ] = SDL_SCANCODE_F1;
        keyToScan[ KEY_F2 ] = SDL_SCANCODE_F2;
        keyToScan[ KEY_F3 ] = SDL_SCANCODE_F3;
        keyToScan[ KEY_F4 ] = SDL_SCANCODE_F4;
        keyToScan[ KEY_F5 ] = SDL_SCANCODE_F5;
        keyToScan[ KEY_F6 ] = SDL_SCANCODE_F6;
        keyToScan[ KEY_F7 ] = SDL_SCANCODE_F7;
        keyToScan[ KEY_F8 ] = SDL_SCANCODE_F8;
        keyToScan[ KEY_F9 ] = SDL_SCANCODE_F9;
        keyToScan[ KEY_F10 ] = SDL_SCANCODE_F10;
        keyToScan[ KEY_F11 ] = SDL_SCANCODE_F11;
        keyToScan[ KEY_F12 ] = SDL_SCANCODE_F12;
        keyToScan[ KEY_F13 ] = SDL_SCANCODE_F13;
        keyToScan[ KEY_F14 ] = SDL_SCANCODE_F14;
        keyToScan[ KEY_F15 ] = SDL_SCANCODE_F15;
        keyToScan[ KEY_VOLUME_UP ] = SDL_SCANCODE_VOLUMEUP;
        keyToScan[ KEY_VOLUME_DOWN ] = SDL_SCANCODE_VOLUMEDOWN;
        keyToScan[ KEY_VOLUME_MUTE ] = SDL_SCANCODE_AUDIOMUTE;
        keyToScan[ KEY_MEDIA_NEXT_TRACK ] = SDL_SCANCODE_AUDIONEXT;
        keyToScan[ KEY_MEDIA_PREV_TRACK ] = SDL_SCANCODE_AUDIOPREV;
        keyToScan[ KEY_MEDIA_PLAY_PAUSE ] = SDL_SCANCODE_AUDIOPLAY;
        keyToScan[ KEY_MEDIA_STOP ] = SDL_SCANCODE_AUDIOSTOP;
        keyToScan[ KEY_NUMLOCK ] = SDL_SCANCODE_NUMLOCKCLEAR;
        keyToScan[ KEY_CAPITAL ] = SDL_SCANCODE_CAPSLOCK;
        keyToScan[ KEY_SCROLL ] = SDL_SCANCODE_SCROLLLOCK;
        keyToScan[ KEY_RSHIFT ] = SDL_SCANCODE_RSHIFT;
        keyToScan[ KEY_LSHIFT ] = SDL_SCANCODE_LSHIFT;
        keyToScan[ KEY_RCONTROL ] = SDL_SCANCODE_RCTRL;
        keyToScan[ KEY_LCONTROL ] = SDL_SCANCODE_LCTRL;
        keyToScan[ KEY_RMENU ] = SDL_SCANCODE_RALT;
        keyToScan[ KEY_LMENU ] = SDL_SCANCODE_LALT;
        keyToScan[ KEY_LWIN ] = SDL_SCANCODE_LGUI;
        keyToScan[ KEY_RWIN ] = SDL_SCANCODE_RGUI;
        keyToScan[ KEY_MODECHANGE ] = SDL_SCANCODE_MODE;
        keyToScan[ KEY_ACCEPT ] = SDL_SCANCODE_APPLICATION;
        keyToScan[ KEY_HELP ] = SDL_SCANCODE_HELP;
        keyToScan[ KEY_SNAPSHOT ] = SDL_SCANCODE_PRINTSCREEN;
        keyToScan[ KEY_VEXECUTE ] = SDL_SCANCODE_SYSREQ;
        keyToScan[ KEY_DELETE ] = SDL_SCANCODE_DELETE;

        scanToKey[ SDL_SCANCODE_BACKSPACE ] = KEY_BACK;
        scanToKey[ SDL_SCANCODE_TAB ] = KEY_TAB;
        scanToKey[ SDL_SCANCODE_CLEAR ] = KEY_CLEAR;
        scanToKey[ SDL_SCANCODE_RETURN ] = KEY_RETURN;
        scanToKey[ SDL_SCANCODE_PAUSE ] = KEY_PAUSE;
        scanToKey[ SDL_SCANCODE_ESCAPE ] = KEY_ESCAPE;
        scanToKey[ SDL_SCANCODE_SPACE ] = KEY_SPACE;
        scanToKey[ SDL_SCANCODE_COMMA ] = KEY_OEM_COMMA;
        scanToKey[ SDL_SCANCODE_MINUS ] = KEY_OEM_MINUS;
        scanToKey[ SDL_SCANCODE_PERIOD ] = KEY_OEM_PERIOD;
        scanToKey[ SDL_SCANCODE_SLASH ] = KEY_OEM_2;
        scanToKey[ SDL_SCANCODE_0 ] = KEY_0;
        scanToKey[ SDL_SCANCODE_1 ] = KEY_1;
        scanToKey[ SDL_SCANCODE_2 ] = KEY_2;
        scanToKey[ SDL_SCANCODE_3 ] = KEY_3;
        scanToKey[ SDL_SCANCODE_4 ] = KEY_4;
        scanToKey[ SDL_SCANCODE_5 ] = KEY_5;
        scanToKey[ SDL_SCANCODE_6 ] = KEY_6;
        scanToKey[ SDL_SCANCODE_7 ] = KEY_7;
        scanToKey[ SDL_SCANCODE_8 ] = KEY_8;
        scanToKey[ SDL_SCANCODE_9 ] = KEY_9;
        scanToKey[ SDL_SCANCODE_SEMICOLON ] = KEY_OEM_1;
        scanToKey[ SDL_SCANCODE_EQUALS ] = KEY_OEM_PLUS;
        scanToKey[ SDL_SCANCODE_LEFTBRACKET ] = KEY_OEM_4;
        scanToKey[ SDL_SCANCODE_BACKSLASH ] = KEY_OEM_5;
        scanToKey[ SDL_SCANCODE_RIGHTBRACKET ] = KEY_OEM_6;
        scanToKey[ SDL_SCANCODE_GRAVE ] = KEY_TILDE;
        scanToKey[ SDL_SCANCODE_A ] = KEY_A;
        scanToKey[ SDL_SCANCODE_B ] = KEY_B;
        scanToKey[ SDL_SCANCODE_C ] = KEY_C;
        scanToKey[ SDL_SCANCODE_D ] = KEY_D;
        scanToKey[ SDL_SCANCODE_E ] = KEY_E;
        scanToKey[ SDL_SCANCODE_F ] = KEY_F;
        scanToKey[ SDL_SCANCODE_G ] = KEY_G;
        scanToKey[ SDL_SCANCODE_H ] = KEY_H;
        scanToKey[ SDL_SCANCODE_I ] = KEY_I;
        scanToKey[ SDL_SCANCODE_J ] = KEY_J;
        scanToKey[ SDL_SCANCODE_K ] = KEY_K;
        scanToKey[ SDL_SCANCODE_L ] = KEY_L;
        scanToKey[ SDL_SCANCODE_M ] = KEY_M;
        scanToKey[ SDL_SCANCODE_N ] = KEY_N;
        scanToKey[ SDL_SCANCODE_O ] = KEY_O;
        scanToKey[ SDL_SCANCODE_P ] = KEY_P;
        scanToKey[ SDL_SCANCODE_Q ] = KEY_Q;
        scanToKey[ SDL_SCANCODE_R ] = KEY_R;
        scanToKey[ SDL_SCANCODE_S ] = KEY_S;
        scanToKey[ SDL_SCANCODE_T ] = KEY_T;
        scanToKey[ SDL_SCANCODE_U ] = KEY_U;
        scanToKey[ SDL_SCANCODE_V ] = KEY_V;
        scanToKey[ SDL_SCANCODE_W ] = KEY_W;
        scanToKey[ SDL_SCANCODE_X ] = KEY_X;
        scanToKey[ SDL_SCANCODE_Y ] = KEY_Y;
        scanToKey[ SDL_SCANCODE_Z ] = KEY_Z;
        scanToKey[ SDL_SCANCODE_KP_0 ] = KEY_NUMPAD0;
        scanToKey[ SDL_SCANCODE_KP_1 ] = KEY_NUMPAD1;
        scanToKey[ SDL_SCANCODE_KP_2 ] = KEY_NUMPAD2;
        scanToKey[ SDL_SCANCODE_KP_3 ] = KEY_NUMPAD3;
        scanToKey[ SDL_SCANCODE_KP_4 ] = KEY_NUMPAD4;
        scanToKey[ SDL_SCANCODE_KP_5 ] = KEY_NUMPAD5;
        scanToKey[ SDL_SCANCODE_KP_6 ] = KEY_NUMPAD6;
        scanToKey[ SDL_SCANCODE_KP_7 ] = KEY_NUMPAD7;
        scanToKey[ SDL_SCANCODE_KP_8 ] = KEY_NUMPAD8;
        scanToKey[ SDL_SCANCODE_KP_9 ] = KEY_NUMPAD9;
        scanToKey[ SDL_SCANCODE_KP_PERIOD ] = KEY_DECIMAL;
        scanToKey[ SDL_SCANCODE_KP_DIVIDE ] = KEY_DIVIDE;
        scanToKey[ SDL_SCANCODE_KP_MULTIPLY ] = KEY_MULTIPLY;
        scanToKey[ SDL_SCANCODE_KP_MINUS ] = KEY_SUBTRACT;
        scanToKey[ SDL_SCANCODE_KP_PLUS ] = KEY_ADD;
        scanToKey[ SDL_SCANCODE_KP_ENTER ] = KEY_SEPARATOR;
        scanToKey[ SDL_SCANCODE_KP_EQUALS ] = KEY_UNKNOWN;
        scanToKey[ SDL_SCANCODE_UP ] = KEY_UP;
        scanToKey[ SDL_SCANCODE_DOWN ] = KEY_DOWN;
        scanToKey[ SDL_SCANCODE_RIGHT ] = KEY_RIGHT;
        scanToKey[ SDL_SCANCODE_LEFT ] = KEY_LEFT;
        scanToKey[ SDL_SCANCODE_INSERT ] = KEY_INSERT;
        scanToKey[ SDL_SCANCODE_HOME ] = KEY_HOME;
        scanToKey[ SDL_SCANCODE_END ] = KEY_END;
        scanToKey[ SDL_SCANCODE_PAGEUP ] = KEY_PRIOR;
        scanToKey[ SDL_SCANCODE_PAGEDOWN ] = KEY_NEXT;
        scanToKey[ SDL_SCANCODE_F1 ] = KEY_F1;
        scanToKey[ SDL_SCANCODE_F2 ] = KEY_F2;
        scanToKey[ SDL_SCANCODE_F3 ] = KEY_F3;
        scanToKey[ SDL_SCANCODE_F4 ] = KEY_F4;
        scanToKey[ SDL_SCANCODE_F5 ] = KEY_F5;
        scanToKey[ SDL_SCANCODE_F6 ] = KEY_F6;
        scanToKey[ SDL_SCANCODE_F7 ] = KEY_F7;
        scanToKey[ SDL_SCANCODE_F8 ] = KEY_F8;
        scanToKey[ SDL_SCANCODE_F9 ] = KEY_F9;
        scanToKey[ SDL_SCANCODE_F10 ] = KEY_F10;
        scanToKey[ SDL_SCANCODE_F11 ] = KEY_F11;
        scanToKey[ SDL_SCANCODE_F12 ] = KEY_F12;
        scanToKey[ SDL_SCANCODE_F13 ] = KEY_F13;
        scanToKey[ SDL_SCANCODE_F14 ] = KEY_F14;
        scanToKey[ SDL_SCANCODE_F15 ] = KEY_F15;
        scanToKey[ SDL_SCANCODE_VOLUMEUP ] = KEY_VOLUME_UP;
        scanToKey[ SDL_SCANCODE_VOLUMEDOWN ] = KEY_VOLUME_DOWN;
        scanToKey[ SDL_SCANCODE_AUDIOMUTE ] = KEY_VOLUME_MUTE;
        scanToKey[ SDL_SCANCODE_AUDIONEXT ] = KEY_MEDIA_NEXT_TRACK;
        scanToKey[ SDL_SCANCODE_AUDIOPREV ] = KEY_MEDIA_PREV_TRACK;
        scanToKey[ SDL_SCANCODE_AUDIOPLAY ] = KEY_MEDIA_PLAY_PAUSE;
        scanToKey[ SDL_SCANCODE_AUDIOSTOP ] = KEY_MEDIA_STOP;
        scanToKey[ SDL_SCANCODE_NUMLOCKCLEAR ] = KEY_NUMLOCK;
        scanToKey[ SDL_SCANCODE_CAPSLOCK ] = KEY_CAPITAL;
        scanToKey[ SDL_SCANCODE_SCROLLLOCK ] = KEY_SCROLL;
        scanToKey[ SDL_SCANCODE_RSHIFT ] = KEY_RSHIFT;
        scanToKey[ SDL_SCANCODE_LSHIFT ] = KEY_LSHIFT;
        scanToKey[ SDL_SCANCODE_RCTRL ] = KEY_RCONTROL;
        scanToKey[ SDL_SCANCODE_LCTRL ] = KEY_LCONTROL;
        scanToKey[ SDL_SCANCODE_RALT ] = KEY_RMENU;
        scanToKey[ SDL_SCANCODE_LALT ] = KEY_LMENU;
        scanToKey[ SDL_SCANCODE_LGUI ] = KEY_LWIN;
        scanToKey[ SDL_SCANCODE_RGUI ] = KEY_RWIN;
        scanToKey[ SDL_SCANCODE_MODE ] = KEY_MODECHANGE;
        scanToKey[ SDL_SCANCODE_APPLICATION ] = KEY_ACCEPT;
        scanToKey[ SDL_SCANCODE_HELP ] = KEY_HELP;
        scanToKey[ SDL_SCANCODE_PRINTSCREEN ] = KEY_SNAPSHOT;
        scanToKey[ SDL_SCANCODE_SYSREQ ] = KEY_VEXECUTE;
        scanToKey[ SDL_SCANCODE_DELETE ] = KEY_DELETE;
#pragma endregion

        SDL_StartTextInput( );

        auto *app = Application::Instance;

        app->Connect( APP_UPDATE, this, &KeyboardManager::onAppUpdate );

        app->GetPlatformEvents( ).Listener( this )
            .On( SDL_KEYDOWN, &KeyboardManager::onKeyDown )
            .On( SDL_KEYUP, &KeyboardManager::onKeyUp )
            .On( SDL_TEXTINPUT, &KeyboardManager::onText );
    }

    KeyboardManager::~KeyboardManager(void)
    {
        auto *app = Application::Instance;

        app->Disconnect( APP_UPDATE, this, &KeyboardManager::onAppUpdate );

        app->GetPlatformEvents( ).Listener( this )
            .Off( SDL_KEYDOWN, &KeyboardManager::onKeyDown )
            .Off( SDL_KEYUP, &KeyboardManager::onKeyUp )
            .Off( SDL_TEXTINPUT, &KeyboardManager::onText );

        SDL_StopTextInput( );
    }

    uint KeyboardManager::ScanToKey(SDL_Scancode scan)
    {
        return scanToKey[ scan ];
    }

    uint KeyboardManager::KeyToScan(KeyboardKey key)
    {
        return keyToScan[ key ];
    }

    bool KeyboardManager::IsDown(KeyboardKey key)
    {
        return m_stateLast && m_stateLast[ keyToScan[ key ] ];
    }

    bool KeyboardManager::IsUp(KeyboardKey key)
    {
        return m_stateLast && !m_stateLast[ keyToScan[ key ] ];
    }

    bool KeyboardManager::IsTriggeredDown(KeyboardKey key)
    {
        return IsDown( key ) && !m_stateQueue[ keyToScan[ key ] ];
    }

    bool KeyboardManager::IsTriggeredUp(KeyboardKey key)
    {
        return IsUp( key ) && m_stateQueue[ keyToScan[ key ] ];
    }

    KeyboardModifiers KeyboardManager::GetModifiers(void) const
    {
        return m_modifiers;
    }

    void KeyboardManager::onAppUpdate(EVENT_HANDLER(Application))
    {
        // copy the last state to the queued state
        memcpy( m_stateQueue, m_stateLast, SDL_NUM_SCANCODES );

        // copy the current state to the last state
        memcpy( m_stateLast, m_stateCurrent, SDL_NUM_SCANCODES );

        m_stateCurrent = SDL_GetKeyboardState( nullptr );

        m_modifiers = static_cast<KeyboardModifiers>( SDL_GetModState( ) );
    }

    void KeyboardManager::onKeyDown(EVENT_HANDLER(Application))
    {
        EVENT_ATTRS(Application, PlatformEventArgs);

        KeyboardKeyArgs e( true,
            static_cast<KeyboardKey>( scanToKey[ args->data->key.keysym.scancode ] ),
            args->data->key.repeat );

        Dispatch( KM_KEY_DOWN, &e );
    }

    void KeyboardManager::onKeyUp(EVENT_HANDLER(Application))
    {
        EVENT_ATTRS(Application, PlatformEventArgs);

        KeyboardKeyArgs e( false,
            static_cast<KeyboardKey>( scanToKey[ args->data->key.keysym.scancode ] ),
            args->data->key.repeat );

        Dispatch( KM_KEY_UP, &e );
    }

    void KeyboardManager::onText(EVENT_HANDLER(Application))
    {
        EVENT_ATTRS(Application, PlatformEventArgs);

        KeyboardTextArgs e( args->data->text.text );

        Dispatch( KM_TEXT_INPUT, &e );
    }
}
