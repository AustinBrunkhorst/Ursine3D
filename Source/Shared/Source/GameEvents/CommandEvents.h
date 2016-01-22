#pragma once

/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** GamePlayEvents.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** --------------------------------------------------------------------------*/

#include <EventArgs.h>
#include <EntityEvent.h>
#include <Vec2.h>

namespace commandEvent
{
    enum CommandEventType
    {
        // The player is wanting to fire
        FIRE_START = ursine::ecs::ENTITY_EVENT_USER,
        FIRE_END,
        LOOK_COMMAND,
        JUMP_COMMAND,
        MOVEMENT
    };

    struct CommandEventArgs : ursine::EventArgs
    {
        CommandEventType type;

        CommandEventArgs(CommandEventType type)
            : type(type) { }
    };

    struct MovementEventArgs : CommandEventArgs
    {
        ursine::Vec2 m_moveDir;

        MovementEventArgs(CommandEventType type, ursine::Vec2& moveDir)
            : CommandEventArgs( type )
            , m_moveDir( moveDir )
        { }
    };


}  // commandEvent namespace

