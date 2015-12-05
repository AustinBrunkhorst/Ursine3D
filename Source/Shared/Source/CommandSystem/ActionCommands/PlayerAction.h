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

        LeftTrigger,
        RightTrigger,

        BindingCount
    };

public:
    PlayerAction();
    PlayerAction(ursine::ecs::Entity *entity, const ActionMode action, const InputBinding binding);
    
    bool WasPressed(void) const;
    bool IsPressed(void) const;
    bool WasReleased(void) const;

    bool StickUp(void) const;
    bool StickDown(void) const;
    bool StickLeft(void) const;
    bool StickRight(void) const;

    bool operator==(const ActionMode &mode);

private:
    ursine::ecs::Entity *m_entity;

    ActionMode m_actionMode;

    InputBinding m_binding;
};