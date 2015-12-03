#pragma once

#include <Component.h>

class Health : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    EditorField(
        float EntityHealth,
        GetHealth,
        SetHealth
    );

    Health(void);
    ~Health(void);

    float GetHealth(void) const;
    void SetHealth(const float health);

    Meta(Disable)
    void DealDamage(const float damage);

private:
    float m_health;

} Meta(Enable, DisplayName("Health"));
