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
#include <Vec2.h>

namespace gameEvent
{
    enum GameEventType
    {
        // The player is wanting to fire
        FIRE_START,
        FIRE_END,
        MOVEMENT
    };

    struct GameEventArgs : ursine::EventArgs
    {
        GameEventType type;

        GameEventArgs(GameEventType type)
            : type(type) { }
    };

    struct MovementEventArgs : GameEventArgs
    {
        ursine::Vec2 m_moveDir;

        MovementEventArgs(GameEventType type, ursine::Vec2& moveDir)
            : GameEventArgs( type )
            , m_moveDir( moveDir )
        { }
    };

}  // gameEvent namespace

