#pragma once

#include <EntityEvent.h>
#include <EventArgs.h>

class DamageOnCollide;

namespace game
{
    enum GameEvents
    {
        GAME_EVENTS_START = ursine::ecs::ENTITY_EVENT_USER,

        // Control Point Events
        CONTROL_POINT_SUCCESS,

        // Damage events
        DAMAGE_EVENT,


        //////////////////////////
        ////  Command Events  ////
        //////////////////////////
#define ENUMERATE(eventName) eventName,
#include "CommandEvents.inl"
#undef ENUMERATE


        /////////////////////////
        ////  Pickup Events  ////
        /////////////////////////
#define ENUMERATE(eventName) eventName,
#include "PickupEvents.inl"
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

    struct DamageEventArgs : ursine::EventArgs
    {
        float m_damage;
        DamageOnCollide* m_damageComp;

        DamageEventArgs(float damage, DamageOnCollide* damageComp)
            : m_damage(damage), m_damageComp(damageComp)
        { }
    };

}
