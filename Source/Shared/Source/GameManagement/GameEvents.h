#pragma once

#include <EntityEvent.h>
#include <EventArgs.h>

namespace game
{
    enum GameEvents
    {
        GAME_EVENTS_START = ursine::ecs::ENTITY_EVENT_USER,

        // control point events
        CONTROL_POINT_SUCCESS,


        /////////////////////
        ////  open door  ////
        /////////////////////
        OPEN_DOOR,


        /////////////////////////
        ////  weapon events  ////
        /////////////////////////
        ACTIVATE_WEAPON,
        DETACH_WEAPON,
        DEACTIVATE_WEAPON,
        PROJECTILE_INIT,
        PICKUP_AMMO,
        CEASE_FIRE,
        FIRE_AT_WILL,


        /////////////////////////
        ////  Player events  ////
        /////////////////////////
        REVIVE_PLAYER,


        /////////////////////////////
        ////  enemy stat events  ////
        /////////////////////////////
        ENEMY_DEATH,
        AREA_CLEAR,


        ////////////////////////////////
        ////  Empower / Disempower  ////
        ////////////////////////////////
        EMPOWER_ACTIVE,
        EMPOWER_UNACTIVE,
        DISEMPOWER_ACTIVE,
        DISEMPOWER_UNACTIVE,


        ////////////////////////////////
        ////          Boss          ////
        ////////////////////////////////
        BOSS_SHIELD_DOWN,


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
        MovementEventArgs(ursine::Vec2 &moveDir)
            : m_moveDir( moveDir ) { }

        ursine::Vec2 m_moveDir;
    };

    struct AmmoPickupEventArgs : ursine::EventArgs
    {
        AmmoPickupEventArgs(const int ammo)
            : m_ammo( ammo ) { }

        int m_ammo;
    };

    struct WeaponDeactivationEventArgs : AmmoPickupEventArgs
    {
        WeaponDeactivationEventArgs(const ursine::ecs::EntityHandle &who, int ammo = 1, int clip = -1)
            : AmmoPickupEventArgs( ammo )
            , m_clip( clip )
            , whoToConnect( who ) { }

        int m_clip;

        ursine::ecs::EntityHandle whoToConnect;
    };

    struct WeaponActivationEventArgs : WeaponDeactivationEventArgs
    {
        WeaponActivationEventArgs(const ursine::ecs::EntityHandle &who, ursine::ecs::Transform* camHandle = nullptr, int ammo = -1, int clip = -1)
            : WeaponDeactivationEventArgs( who, ammo, clip )
            , m_camHandle( camHandle ) { }

        ursine::ecs::Transform* m_camHandle;
    };
    

    // projectile event args
    struct ProjectileInitEventArgs : ursine::EventArgs
    {
        ProjectileInitEventArgs(const ursine::SVec3 forward, float range)
            : m_forwardVec( forward )
            , m_range( range ) { }

        ursine::SVec3 m_forwardVec;

        float m_range;
    };
}
