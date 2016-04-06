#pragma once

/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** HealthRegenComponent.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/


#include <Component.h>
#include "HealthComponent.h"

class HealthRegen : public ursine::ecs::Component
{
    NATIVE_COMPONENT
public:

    HealthRegen(void);
    ~HealthRegen(void);

    EditorField(
        float HealthRegenTime,
        GetHealthRegenTime,
        SetHealthRegenTime
        );
    float GetHealthRegenTime(void) const;
    void SetHealthRegenTime(float time);

    EditorField(
        float ShieldRegenTime,
        GetShieldRegenTime,
        SetShieldRegenTime
        );
    float GetShieldRegenTime(void) const;
    void SetShieldRegenTime(float time);

    EditorField(
        float WaitTime,
        GetWaitTime,
        SetWaitTime
        );
    float GetWaitTime(void) const;
    void SetWaitTime(float time);

    Meta(Disable)
    float GetTimer(void) const;
    Meta(Disable)
    void IncrementTimer(float dt);
    Meta(Disable)
    void ResetTimer(void);

    Meta(Disable)
    float GetHealthToAdd(void) const;
    Meta(Disable)
    void SetHealthToAdd(float healthToAdd);

    Meta(Disable)
        float GetShieldToAdd(void) const;
    Meta(Disable)
        void SetShieldToAdd(float shieldToAdd);

protected:

    void OnInitialize(void) override;

private:

    void OnDamaged(EVENT_HANDLER(ursine::ecs::Entity));

    // time it takes to regen from no health to full
    float m_healthRegenTime;

    // time it takes to regen from no shield to full
    float m_shieldRegenTime;

    // time to wait when taken damage to start regen
    float m_waitTime;

    // timer to track wait time;
    float m_timer;

    // health to add each frame while regenerating
    float m_healthToAdd;

    // health to add to shield each frame while at full health
    float m_shieldToAdd;

} Meta(
    Enable, 
    DisplayName( "HealthRegen" ),
) EditorMeta(
    RequiresComponents( typeof( Health ) )
);
