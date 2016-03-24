/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ProjectileComponent.cpp
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/

#include <Precompiled.h>
#include "ProjectileComponent.h"
#include "GameEvents.h"
#include "Application.h"

NATIVE_COMPONENT_DEFINITION(Projectile);

using namespace ursine;
using namespace ecs;

Projectile::Projectile(void)
    : BaseComponent( )
    , m_faceTowardsPath( false )
    , m_speed( 10 )
    , m_lifeTime( 0 )
    , m_movementVec( ursine::SVec3( ) )
    , m_transform( nullptr ) { }

Projectile::~Projectile(void)
{
}

void Projectile::OnInitialize(void)
{
    GetOwner( )->Listener( this )
        .On( game::PROJECTILE_INIT, &Projectile::OnInit );
}

float Projectile::GetSpeed(void) const
{
    return m_speed;
}

void Projectile::SetSpeed(float speed)
{
    m_speed = speed;
}

bool Projectile::GetFaceTowardsPath(void) const
{
    return m_faceTowardsPath;
}

void Projectile::SetFaceTowardsPath(bool flag)
{
    m_faceTowardsPath = flag;

    NOTIFY_COMPONENT_CHANGED( "faceTowardsPath", m_faceTowardsPath );
}

void Projectile::Update(float dt)
{
    m_lifeTime -= dt;

    if ( m_lifeTime <= 0.0f )
    {
        GetOwner( )->Delete( );
    }

    auto oldPosition = m_transform->GetWorldPosition( );
    auto newPosition = oldPosition + m_movementVec * Application::Instance->GetDeltaTime( );

    m_transform->SetWorldPosition( newPosition );
}

void Projectile::OnInit(EVENT_HANDLER(ursine::ecs::Entity))
{
    EVENT_ATTRS(ursine::ecs::Entity, game::ProjectileInitEventArgs);

    CalculateLifeTime(args->m_range);

    m_movementVec = args->m_forwardVec * m_speed;

    m_transform = GetOwner( )->GetTransform( );

    if (m_faceTowardsPath)
        m_transform->SetWorldRotation( SQuat::LookAt( m_movementVec ) );
}

// calculate lifetime of projectile
void Projectile::CalculateLifeTime(float range)
{
    // infinite range
    if ( range == 0.0f )
    {
        range = FLT_MAX;
    }

    m_lifeTime = range / m_speed;
}
