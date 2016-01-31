/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** DamageOnCollideComponent.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

class Health;

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

    void OnCollide(EVENT_HANDLER(ursine::ecs::ENTITY_COLLISION_PERSISTED));

    void ApplyDamage(Health* healthComp);

    // add an entity to the damage intervals so it can not be damaged by this object during 
    //   damage interval
    void AddEntityToIntervals(ursine::ecs::EntityUniqueID uniqueID);

    void DecrementDamageIntervalTimes( const float dt );

    ////////////////////////////////////////////////////////////////////
    // Expose data to editor
    ////////////////////////////////////////////////////////////////////
    EditorField(
        float DamageToApply,
        GetDamageToApply,
        SetDamageToApply
        );

    EditorField(
        float CritModifier,
        GetCritModifier,
        SetCritModifier
        );

    EditorField(
        float DamageInterval,
        GetDamageInterval,
        SetDamageInterval
        );

    EditorField(
        bool DeleteOnCollision,
        GetDeleteOnCollision,
        SetDeleteOnCollision
        );

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

    Meta(Disable)
    bool DeleteOnCollision(void);

    // map of all objects hit
    std::unordered_map<ursine::ecs::EntityUniqueID, float> m_damageTimeMap;

} Meta (Enable, DisplayName( "DamageOnCollide" ));