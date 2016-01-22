#pragma once

#include <EntityEvent.h>
#include <EventArgs.h>

namespace game
{
    enum GameEvents
    {
        GAME_EVENTS_START = ursine::ecs::ENTITY_EVENT_USER,

        CONTROL_POINT_SUCCESS,


        //////////////////////////
        ////  Command Events  ////
        //////////////////////////
#define ENUMERATE(eventName) eventName,
#include "CommandEvents.inl"
#undef ENUMERATE

        GAME_EVENTS_END
    };


    struct MovementEventArgs : ursine::EventArgs
    {
        ursine::Vec2 m_moveDir;

        MovementEventArgs(ursine::Vec2& moveDir)
            : m_moveDir(moveDir)
        { }
    };
}
