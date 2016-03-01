#pragma once

#include <EntityEvent.h>
#include <EventArgs.h>

class DamageOnCollide;

namespace game
{
    enum GameEvents
    {
        GAME_EVENTS_START = ursine::ecs::ENTITY_EVENT_USER,

        // control point events
        CONTROL_POINT_SUCCESS,

        // damage events
        DAMAGE_EVENT,

        // open door
        OPEN_DOOR,

        // weapon events
        ACTIVATE_WEAPON,
        DETACH_WEAPON,
        DEACTIVATE_WEAPON,

        ENEMY_DEATH,
        AREA_CLEAR,

        ////  Command Events  ////
#define ENUMERATE(eventName) eventName,
#include "CommandEvents.inl"
#undef ENUMERATE

        GAME_EVENTS_END
    };

    struct MovementEventArgs : ursine::EventArgs
    {
        ursine::Vec2 m_moveDir;

        MovementEventArgs(ursine::Vec2 &moveDir)
            : m_moveDir( moveDir ) { }
    };

    struct DamageEventArgs : ursine::EventArgs
    {
        float m_damage;

        DamageEventArgs(float damage)
            : m_damage( damage ) { }
    };

    struct WeaponDeactivationEventArgs : ursine::EventArgs
    {
        int m_ammo;
        int m_clip;

        ursine::ecs::EntityHandle whoToConnect;

        WeaponDeactivationEventArgs(const ursine::ecs::EntityHandle &who, int ammo = 1, int clip = -1)
            : m_ammo( ammo )
            , m_clip( clip )
            , whoToConnect( who ) { }
    };


    struct WeaponActivationEventArgs : WeaponDeactivationEventArgs
    {
        ursine::ecs::Transform *m_camHandle;
        ursine::SVec3 *m_spawnOffset;

        WeaponActivationEventArgs(const ursine::ecs::EntityHandle &who, ursine::ecs::Transform *camHandle = nullptr, int ammo = -1, int clip = -1)
            : WeaponDeactivationEventArgs( who, ammo, clip )
            , m_camHandle( camHandle )
            , m_spawnOffset( nullptr ) { }
    };
}
