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
#include <AudioItemEventData.h>

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

    EditorResourceField(
        ursine::resources::ArchetypeData,
        aoeArchetype,
        GetAoeArchetype,
        SetAoeArchetype
    );

    EditorField(
        std::string mapRaycastEntity,
        GetMapRaycastEntity,
        SetMapRaycastEntity
    );

    EditorResourceField(
        ursine::resources::AudioItemEventData,
        shootSfx,
        GetShootSfx,
        SetShootSfx
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

    const ursine::resources::ResourceReference &GetAoeArchetype(void) const;
    void SetAoeArchetype(const ursine::resources::ResourceReference &archetype);

    const ursine::resources::ResourceReference &GetShootSfx(void) const;
    void SetShootSfx(const ursine::resources::ResourceReference &soundEvent);

    const std::string &GetMapRaycastEntity(void) const;
    void SetMapRaycastEntity(const std::string &name);

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

    ursine::resources::ResourceReference m_aoeArchetype;
    ursine::resources::ResourceReference m_shootSfx;

    std::string m_mapName;

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
