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

        /////////////////////////
        ////  damage events  ////
        /////////////////////////
        DAMAGE_TEXT_EVENT,


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

        MovementEventArgs(ursine::Vec2 &moveDir)
            : m_moveDir( moveDir ) { }
    };

    struct DamageEventArgs : public ursine::EventArgs
    {
        DamageEventArgs(const ursine::SVec3 &hitPosition,
                        ursine::ecs::EntityHandle &entityHit,
                        float damage, bool crit)
            : hitPosition(hitPosition)
            , entityHit(entityHit)
            , damage(damage)
            , crit(crit)
        { }

        const ursine::SVec3& hitPosition;
        ursine::ecs::EntityHandle entityHit;
        float damage;
        bool crit;

    };

    struct AmmoPickupEventArgs : ursine::EventArgs
    {
        int m_ammo;

        AmmoPickupEventArgs(const int ammo)
            : m_ammo( ammo )
        { }
    };

    struct WeaponDeactivationEventArgs : AmmoPickupEventArgs
    {
        int m_clip;

        ursine::ecs::EntityHandle whoToConnect;

        WeaponDeactivationEventArgs(const ursine::ecs::EntityHandle &who, int ammo = 1, int clip = -1)
            : AmmoPickupEventArgs( ammo )
            , m_clip( clip )
            , whoToConnect( who ) 
        { }
    };

    struct WeaponActivationEventArgs : WeaponDeactivationEventArgs
    {
        ursine::ecs::Transform* m_camHandle;

        WeaponActivationEventArgs(const ursine::ecs::EntityHandle &who, ursine::ecs::Transform* camHandle = nullptr, int ammo = -1, int clip = -1)
            : WeaponDeactivationEventArgs(who, ammo, clip)
            , m_camHandle(camHandle) 
        { }
    };
    

    // projectile event args
    struct ProjectileInitEventArgs : ursine::EventArgs
    {
        ursine::SVec3 m_forwardVec;
        float m_range;

        ProjectileInitEventArgs(const ursine::SVec3 forward, float range)
            : m_forwardVec( forward )
            , m_range( range )
        { }
    };
}
