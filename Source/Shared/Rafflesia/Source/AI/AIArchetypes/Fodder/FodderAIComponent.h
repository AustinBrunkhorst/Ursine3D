/* ---------------------------------------------------------------------------
** Team Bear King
** 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** FodderAIComponent.h
**
** Author:
** - Joshua Shlemmer - joshua.shlemmer@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

#include "FodderAIStateMachine.h"

// sometimes VS thinks it doesn't need it but it do
#include "AIMovementControllerComponent.h"

class FodderAI : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    EditorField(
        float AttackDelay,
        GetPauseTime,
        SetPauseTime
    );

    EditorField(
        float damageOnAttack,
        GetDamage,
        SetDamage
    );

    EditorField(
        float AttackRange,
        GetAttackRange,
        SetAttackRange
    );

    EditorField(
        float Cohesion,
        GetCohesionScale,
        SetCohesionScale
    );

    EditorField(
        float Separation,
        GetSeparationScale,
        SetSeparationScale
    );

    EditorField(
        float TotalBoidBehaviorScale,
        GetBoidScale,
        SetBoidScale
    );

    FodderAI(void);
    ~FodderAI(void);

    float GetPauseTime(void) const;
    void SetPauseTime(float newTime);

    float GetDamage(void) const;
    void SetDamage(float dmg);

    float GetAttackRange(void) const;
    void SetAttackRange(float newRange);

    float GetCohesionScale(void) const;
    void SetCohesionScale(float newScale);

    float GetSeparationScale(void) const;
    void SetSeparationScale(float newScale);

    float GetBoidScale(void) const;
    void SetBoidScale(float newScale);

private:
    void OnInitialize(void) override;

    void onUpdate(EVENT_HANDLER(World));

    void onCollide(EVENT_HANDLER(ursine::ecs::ENTITY_COLLISION_PERSISTED));

    ursine::sm::FodderAIStateMachine m_stateMachine;

    float m_pauseTime;

    float m_damage;

    float m_range;

    float m_cohesionScale;

    float m_separationScale;

    float m_boidBehaviorScale;

} Meta(
    Enable, 
    DisplayName( "FodderAI" )
) EditorMeta(
    RequiresComponents( typeof( ursine::ecs::AIMovementController ) )    
);
