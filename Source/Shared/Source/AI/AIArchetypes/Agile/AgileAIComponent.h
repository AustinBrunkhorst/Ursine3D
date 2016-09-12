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

#include "AgileAIStateMachine.h"

// VS YOU SUPER NEED DIS DOE SHUT UP ABOUT IT BRO
#include "AIMovementControllerComponent.h"

class AgileAI : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:

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

    AgileAI(void);
    ~AgileAI(void);

    float GetDamage(void) const;
    void SetDamage(float dmg);

    float GetAttackRange(void) const;
    void SetAttackRange(float range);

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
        ursine::sm::AgileAIStateMachine m_stateMachine;

    float m_damage;

    float m_attackRange;

    float m_cohesionScale;

    float m_separationScale;

    float m_boidScale;

} Meta(Enable, DisplayName("AgileAI"), RequiresComponents(typeof(ursine::ecs::AIMovementController)));
