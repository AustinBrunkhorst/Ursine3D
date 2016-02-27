/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** PlayerAction.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#pragma once

#include <Entity.h>
#include <GameEvents.h>

namespace ursine
{
    class KeyboardManager;
    enum KeyboardKey;
    class GamepadManager;
    class GamepadState;
    enum GamepadButton;
    class MouseManager;
    enum MouseButton;

    class Vec2;
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

    enum Interaction
    {
        Down,
        Held,
        Released,
        Up
    };

public:
    PlayerAction();
    PlayerAction(PlayerID* idComp, ActionMode action);

    void SetActionMode(ActionMode mode);
    void SetPlayerIDComp(PlayerID* idComp);

    bool WasPressed(const InputBinding binding);
    bool IsPressed(const InputBinding binding);
    bool WasReleased(const InputBinding binding);
    bool IsUp(const InputBinding binding);

    bool StickUp(const InputBinding binding);
    bool StickDown(const InputBinding binding);
    bool StickLeft(const InputBinding binding);
    bool StickRight(const InputBinding binding);

    void GetAxis(const InputBinding binding, ursine::Vec2& vecToSet);

    bool operator==(const ActionMode &mode) const;

private:

    static ursine::GamepadManager*  m_gamepadManager;
    static ursine::KeyboardManager* m_keyboardManager;
    static ursine::MouseManager*    m_mouseManager;
    
    ActionMode m_actionMode;

    PlayerID* m_playerIDComp;
    ursine::GamepadState* m_gamepadState;

    bool PrepForInput(void);
    void GetMouseVec(ursine::Vec2& vecToSet) const;
    
    bool EvalXboxButtons(const InputBinding binding, bool(ursine::GamepadState::*)(ursine::GamepadButton, float) const);
    bool EvalKeyboardButtons(const InputBinding binding,
                             bool( ursine::KeyboardManager::* )(ursine::KeyboardKey),
                             bool( ursine::MouseManager::* )( ursine::MouseButton ) const ) const;
    bool EvalMouseButtons(const InputBinding binding, bool( ursine::MouseManager::* )( ursine::MouseButton ) const) const;
};