#pragma once

/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** PlayerLookAtComponent.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/


#include <Component.h>

class Health;

namespace ursine
{
    namespace ecs
    {
        class EntityHandle;
    }
}

class PlayerLookAt : public ursine::ecs::Component
{
    NATIVE_COMPONENT;
public:

    PlayerLookAt(void);
    ~PlayerLookAt(void);

    EditorField(
        float DelayTime,
        GetDelayTime,
        SetDelayTime
        );

    float GetDelayTime(void) const;
    void SetDelayTime(float time);

    float GetTimer(void) const;
    void SetTimer(float time);
    void IncrementTimer(float dt);

    float GetHealthPercent(void) const;
    void SetHealthPercent(float);

    bool ReticleActive(void) const;
    void SetReticleActive(bool);

    const ursine::ecs::EntityHandle& GetCurrentEnemy(void) const;
    void SetCurrentEnemy(ursine::ecs::EntityHandle& entity);

    Health* GetCurrentEnemyHealth(void) const;

    void SetCurrentEnemyHealth(Health* healthComp);

    void onEnemyDeath(EVENT_HANDLER(ursine::ecs::Entity));

protected:

    void OnInitialize(void) override;

private:

    float m_delay;

    float m_timer;

    float m_healthPercent;

    ursine::ecs::EntityHandle m_currEnemy;

    Health* m_currEnemyHealth;

    bool m_reticleActive;


} Meta(Enable, DisplayName("PlayerLookAt"));
