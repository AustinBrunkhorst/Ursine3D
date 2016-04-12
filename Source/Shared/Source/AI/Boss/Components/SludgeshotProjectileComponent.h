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

#include "EntityAnimatorComponent.h"
#include "HealthComponent.h"

#include <ParticleEmitterComponent.h>

class SludgeshotProjectile : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    EditorButton(
        drawRange,
        "Draw Range"
    );

    EditorField(
        float range,
        GetRange,
        SetRange
    );

    EditorField(
        float damage,
        GetDamage,
        SetDamage
    );

    EditorField(
        float damageInterval,
        GetDamageInterval,
        SetDamageInterval
    );

    EditorField(
        float impulse,
        GetImpulse,
        SetImpulse
    );

    SludgeshotProjectile(void);
    ~SludgeshotProjectile(void);

    float GetRange(void) const;
    void SetRange(float range);

    float GetDamage(void) const;
    void SetDamage(float damage);

    float GetDamageInterval(void) const;
    void SetDamageInterval(float interval);

    float GetImpulse(void) const;
    void SetImpulse(float impulse);

    void SetTargetPosition(const ursine::SVec3 &target);

    void SetTotalTimeOfAnimation(float time);

    // Called after all the properties are set from within "BossSludgeshotState.cpp"
    void InitializeComponents(void);

private:
    ursine::SVec3 m_target;

    float m_time;

    float m_emitionRate;

    float m_range;

    float m_damage;

    float m_interval;

    float m_impulse;

    // This map keeps track of timers related to the last time we've damaged
    // the player that corresponds to the key
    std::unordered_map<ursine::ecs::EntityHandle, float> m_damageMap;

    void onAnimationCompleted(EVENT_HANDLER(EntityAnimator));

    void onUpdate(EVENT_HANDLER(ursine::ecs::World));

} Meta(
    Enable,
) EditorMeta(
    RequiresComponents(
        typeof( EntityAnimator ),
        typeof( ursine::ecs::ParticleEmitter )
    )
);
