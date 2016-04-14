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

    bool ReticleActive(void) const;
    void SetReticleActive(bool);

    const ursine::ecs::EntityHandle& GetCurrentEnemy(void) const;
    void SetCurrentEnemy(ursine::ecs::EntityHandle& entity);

    void onEnemyDeath(EVENT_HANDLER(ursine::ecs::Entity));

protected:

    void OnInitialize(void) override;

private:

    float m_delay;

    Meta(Disable)
        float m_timer;

    Meta(Disable)
        ursine::ecs::EntityHandle m_currEnemy;

    Meta(Disable)
        bool m_reticleActive;


} Meta(Enable, DisplayName("PlayerLookAt"));



