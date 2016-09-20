/* ---------------------------------------------------------------------------
** Team Bear King
** 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** BomberAIComponent.h
**
** Author:
** - Joshua Shlemmer - joshua.shlemmer@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

#include <ArchetypeData.h>

#include "BomberAIStateMachine.h"

// This header is needed. Don't be tricked by VS's lies
#include "AIMovementControllerComponent.h"

class BomberAI : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:

    EditorField(
        float ExplosionDelay,
        GetExplosionDelay,
        SetExplosionDelay
    );

    /*
    EditorField(
        float damageOnAttack,
        GetDamage,
        SetDamage
        );

        EditorField(
        float BlastRadius,
        GetBlastRadius,
        SetBlastRadius
        );
        */

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

    EditorResourceField(
        ursine::resources::ArchetypeData,
        explosionObject,
        GetExplosionArchetype,
        SetExplosionArchetype
    );

    BomberAI(void);
    ~BomberAI(void);

    float GetExplosionDelay(void) const;
    void SetExplosionDelay(float delay);
    /*
    float GetDamage(void) const;
    void SetDamage(float dmg);

    float GetBlastRadius(void) const;
    void SetBlastRadius(float radius);
    */
    float GetAttackRange(void) const;
    void SetAttackRange(float range);

    float GetCohesionScale(void) const;
    void SetCohesionScale(float newScale);

    float GetSeparationScale(void) const;
    void SetSeparationScale(float newScale);

    float GetBoidScale(void) const;
    void SetBoidScale(float newScale);

    const ursine::resources::ResourceReference &GetExplosionArchetype(void) const;
    void SetExplosionArchetype(const ursine::resources::ResourceReference &explosion);

private:

    void OnInitialize(void) override;

    void onUpdate(EVENT_HANDLER(World));

    ursine::sm::BomberAIStateMachine m_stateMachine;

    // this gets passed to the explode state to be spawned on explosion
    ursine::resources::ResourceReference m_explosionObj;

    float m_explosionDelay;

    float m_damage;

    float m_attackRange;

    //float m_blastRadius;

    float m_cohesionScale;

    float m_separationScale;

    float m_boidScale;

} Meta(
    Enable, 
    DisplayName( "BomberAI" ), 
) EditorMeta(
    RequiresComponents( typeof( ursine::ecs::AIMovementController ) )    
);
