/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** HealthComponent.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <Component.h>
#include <RigidbodyComponent.h>

struct HealthEventArgs : ursine::EventArgs
{
    float health;
    float percentage;

    HealthEventArgs(float health, float percentage)
        : health( health )
        , percentage( percentage ) { }
};

class Health : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    EditorField(
        float EntityHealth,
        GetHealth,
        SetHealth
    );

    Meta(Enable)
    Health(void);
    ~Health(void);

    float GetHealth(void) const;
    void SetHealth(const float health);
    float GetMaxHealth(void) const;

    void DealDamage(const float damage);

private:
    void OnInitialize(void) override;
    void ConnectToAllCritSpots(void);

    void OnDamaged(EVENT_HANDLER(game::DAMAGE_EVENT));

    float m_health;
    float m_maxHealth;
} Meta(Enable, WhiteListMethods, DisplayName( "Health" ), RequiresComponents( typeof( ursine::ecs::Rigidbody ) ));
