/* ---------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** DamageOnCollideComponent.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include <Component.h>


class DamageOnCollide : public ursine::ecs::Component
{
    NATIVE_COMPONENT;
public:

    DamageOnCollide( void );
    ~DamageOnCollide( void );

    void OnInitialize( void ) override;

    float GetDamageToApply( void ) const;
    void SetDamageToApply( const float damage );

    float GetCritModifier( void ) const;
    void SetCritModifier( const float modifier );

    float GetDamageInterval( void ) const;
    void SetDamageInterval( const float damageInterval );

    bool GetDeleteOnCollision( void ) const;
    void SetDeleteOnCollision( const bool state );

    void ApplyDamage( EVENT_HANDLER( ursine::ecs::ENTITY_COLLISION_PERSISTED ) );

    void DecrementDamageIntervalTimes( const float dt );

private:

    // damage to apply when triggered
    float m_damageToApply;

    // scalar to apply to damage
    float m_critModifier;

    // time to wait until apply damage again
    //   (keeps from applying damage each frame)
    float m_damageInterval;

    // does projectile die on first collision
    bool m_deleteOnCollision;

    // was projectile already deleted this frame
    //   (make sure damage only applied to one collision if delete on collision is set)
    bool m_deleted;

    // map of all objects hit
    std::unordered_map<int, float> m_damageTimeMap;

} Meta (Enable, DisplayName( "DamageOnCollide" ));