/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** PollinateProjectileComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include <Component.h>
#include <RigidbodyComponent.h>
#include <SphereColliderComponent.h>
#include <ParticleEmitterComponent.h>
#include <SVec3.h>
#include "DamageOnCollideComponent.h"

class PollinateProjectile : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    PollinateProjectile(void);

    void OnSceneReady(ursine::Scene *scene) override;

    // Called after all the properties are set from within "BossPollinateState.cpp"
    void InitializeComponents(void);

    void SetDirection(const ursine::SVec3 &direction);
    void SetGravity(float gravity);
    void SetSpreadDistance(float spreadDistance);
    void SetSpreadTime(float spreadTime);
    void SetLifeTime(float lifeTime);

private:
    ursine::SVec3 m_direction;

    float m_gravity;

    float m_spreadDistance;

    float m_spreadTime;
    float m_spreadTimer;

    float m_lifeTime;
    float m_lifeTimer;

    float m_dissipateTime;
    float m_dissipateTimer;

    void onUpdate(EVENT_HANDLER(World));

} Meta(
    Enable
) EditorMeta(
    RequiresComponents(
        typeof( DamageOnCollide ),
        typeof( ursine::ecs::Rigidbody ),
        typeof( ursine::ecs::SphereCollider ),
        typeof( ursine::ecs::ParticleEmitter )
    )    
);
