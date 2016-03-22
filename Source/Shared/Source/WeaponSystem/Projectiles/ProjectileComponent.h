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

class Projectile : public ursine::ecs::Component
{
    NATIVE_COMPONENT;
public:
    EditorField(
        float Speed,
        GetSpeed,
        SetSpeed
    );

    Projectile( void );
    ~Projectile( void );

    float GetSpeed( void ) const;
    void SetSpeed( const float speed );

    void CalculateLifeTime( float range );
    void Update( const float dt );

private:

    float m_speed;
    float m_lifeTime;

} Meta(Enable, DisplayName("Projectile"));
