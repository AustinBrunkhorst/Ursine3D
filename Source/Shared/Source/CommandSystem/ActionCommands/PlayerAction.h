/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** PlayerAction.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#pragma once

#include <Entity.h>

namespace ursine
{
    class KeyboardManager;
    enum KeyboardKey;
    class GamepadManager;
    class GamepadState;
    enum GamepadButton;
    class MouseManager;
    enum MouseButton;
} // ursine namespace

class PlayerID;


class PlayerAction
{
public:
    enum ActionMode
    {
        Xbox,
        Keyboard,

        ActionCount
    };

    enum InputBinding
    {
        Action1,
        Action2,
        Action3,
        Action4,

        LeftStickLeft,
        LeftStickRight,
        LeftStickUp,
        LeftStickDown,

        RightStickLeft,
        RightStickRight,
        RightStickUp,
        RightStickDown,

        MouseLeft,
        MouseRight,
        MouseUp,
        MouseDown,

        KeyLeft,
        KeyRight,
        KeyUp,
        KeyDown,

        LeftTrigger,
        RightTrigger,

        BindingCount
    };

public:
    PlayerAction();
    PlayerAction(PlayerID* idComp, const ActionMode action, const InputBinding binding);

    bool WasPressed(void);
    bool IsPressed(void);
    bool WasReleased(void);

    bool StickUp(void);
    bool StickDown(void);
    bool StickLeft(void);
    bool StickRight(void);

    ursine::Vec2 GetAxis(void);

    bool operator==(const ActionMode &mode) const;

private:
    ActionMode m_actionMode;

    InputBinding m_binding;

    PlayerID* m_playerIDComp;

    ursine::GamepadState* m_gamepadState;

    ursine::GamepadManager* m_gamepadManager;
    ursine::KeyboardManager* m_keyboardManager;
    ursine::MouseManager* m_mouseManager;

    bool PrepForInput(void);

    ursine::Vec2 GetMouseVec(void) const;
    
    bool EvalXboxButtons(bool(ursine::GamepadState::*)(ursine::GamepadButton, float) const);
    bool EvalKeyboardButtons(bool( ursine::KeyboardManager::* )(ursine::KeyboardKey),
                             bool( ursine::MouseManager::* )( ursine::MouseButton ) const ) const;
    bool EvalMouseButtons(bool( ursine::MouseManager::* )( ursine::MouseButton ) const) const;
};