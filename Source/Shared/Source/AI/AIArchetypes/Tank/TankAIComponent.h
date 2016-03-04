/* ---------------------------------------------------------------------------
** Team Bear King
** 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** TankAIComponent.h
**
** Author:
** - Joshua Shlemmer - joshua.shlemmer@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

#include "TankAIStateMachine.h"

// VS YOU SUPER NEED DIS DOE SHUT UP ABOUT IT BRO
#include "AIMovementControllerComponent.h"

class TankAI : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:

    EditorField(
        float SlamDelay,
        GetSlamDelay,
        SetSlamDelay
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
        float AttackRadius,
        GetAttackRadius,
        SetAttackRadius
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

    TankAI(void);
    ~TankAI(void);

    float GetSlamDelay(void) const;
    void SetSlamDelay(float delay);

    float GetDamage(void) const;
    void SetDamage(float dmg);

    float GetAttackRange(void) const;
    void SetAttackRange(float range);

    float GetAttackRadius(void) const;
    void SetAttackRadius(float radius);

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

    Meta(Disable)
        ursine::sm::TankAIStateMachine m_stateMachine;

    float m_slamDelay;

    float m_damage;

    float m_attackRange;

    float m_attackRadius;

    float m_cohesionScale;

    float m_separationScale;

    float m_boidScale;

} Meta(Enable, DisplayName("TankAI"), RequiresComponents(typeof(ursine::ecs::AIMovementController)));
