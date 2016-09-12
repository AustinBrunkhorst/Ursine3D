/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** SludgeshotAoeComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#include <Component.h>

#include <ParticleEmitterComponent.h>

class SludgeshotAoe : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    EditorField(
        float lifeTime,
        GetLifeTime,
        SetLifeTime
    );

    EditorField(
        float damage,
        GetDamage,
        SetDamage
    );

    EditorField(
        float interval,
        GetInterval,
        SetInterval
    );

    SludgeshotAoe(void);
    ~SludgeshotAoe(void);

    void OnSceneReady(ursine::Scene *scene) override;

    float GetLifeTime(void) const;
    void SetLifeTime(float lifetime);

    float GetDamage(void) const;
    void SetDamage(float damage);

    float GetInterval(void) const;
    void SetInterval(float interval);

private:

    float m_lifeTime;
    float m_damage;
    float m_interval;

    float m_fadeOutTime;
    float m_fadeOutTimer;
    float m_originalScale;

    std::unordered_map<ursine::ecs::EntityHandle, float> m_damageMap;

    void onUpdate(EVENT_HANDLER(ursine::ecs::World));

} Meta( Enable );
