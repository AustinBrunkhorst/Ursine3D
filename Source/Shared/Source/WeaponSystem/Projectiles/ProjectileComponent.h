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

    EditorField(
        bool faceTowardsPath,
        GetFaceTowardsPath,
        SetFaceTowardsPath
    );

    Projectile(void);
    ~Projectile(void);

    void OnInitialize(void) override;

    float GetSpeed(void) const;
    void SetSpeed(float speed);

    bool GetFaceTowardsPath(void) const;
    void SetFaceTowardsPath(bool flag);

    void Update(float dt);

private:
    void OnInit(EVENT_HANDLER(ursine::ecs::Entity));

    void CalculateLifeTime(float range);

    bool m_faceTowardsPath;

    float m_speed;
    float m_lifeTime;

    ursine::SVec3 m_movementVec;

    ursine::ecs::Transform* m_transform;

} Meta( Enable, DisplayName( "Projectile" ), RequiresComponents( typeof( ursine::ecs::Rigidbody ) ) );
