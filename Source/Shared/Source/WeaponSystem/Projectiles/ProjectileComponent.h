/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ProjectileComponent.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/
#pragma once

#include <Component.h>
#include "RigidbodyComponent.h"

class Projectile : public ursine::ecs::Component
{
    NATIVE_COMPONENT;
public:

    EditorField(
        float Speed,
        GetSpeed,
        SetSpeed
        );

    Projectile(void);
    ~Projectile(void);

    void OnInitialize(void) override;

    float GetSpeed(void) const;
    void SetSpeed(const float speed);

    void Update(const float dt);

private:
    void OnInit(EVENT_HANDLER(ursine::ecs::Entity));

    void CalculateLifeTime(float range);

    float m_range;
    float m_speed;
    float m_lifeTime;

} Meta( Enable, DisplayName( "Projectile" ), RequiresComponents( typeof( ursine::ecs::Rigidbody ) ) );
