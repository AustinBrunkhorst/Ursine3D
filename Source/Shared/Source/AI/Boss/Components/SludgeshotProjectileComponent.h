/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** SludgeshotProjectileComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#include <Component.h>

#include "DamageOnCollideComponent.h"
#include "EntityAnimatorComponent.h"
#include "HealthComponent.h"

#include <ParticleEmitterComponent.h>

class SludgeshotProjectile : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:

    SludgeshotProjectile(void);
    ~SludgeshotProjectile(void);

    void SetTargetPosition(const ursine::SVec3 &target);

    void SetTotalTimeOfAnimation(float time);

    // Called after all the properties are set from within "BossSludgeshotState.cpp"
    void InitializeComponents(void);

private:

    ursine::SVec3 m_target;

    float m_time;

    void onAnimationCompleted(EVENT_HANDLER(EntityAnimator));

} Meta(
    Enable,
    RequiresComponents(
        typeof(DamageOnCollide),
        typeof(EntityAnimator),
        typeof(Health),
        typeof(ursine::ecs::ParticleEmitter)
    )
);
