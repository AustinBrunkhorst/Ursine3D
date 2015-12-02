#pragma once

#include <Component.h>

class Health : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    EditorField(
        int EntityHealth,
        GetHealth,
        SetHealth
    );

    Health(void);
    ~Health(void);

    int GetHealth(void) const;
    void SetHealth(const int health);

    Meta(Disable)
    void DealDamage(const int damage);

private:
    int m_health;

} Meta(Enable, DisplayName("Health"));
