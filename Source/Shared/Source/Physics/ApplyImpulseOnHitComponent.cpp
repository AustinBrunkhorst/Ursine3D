/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ApplyImpulseOnHitComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "ApplyImpulseOnHitComponent.h"

#include <EntityEvent.h>
#include <CollisionEventArgs.h>
#include <RigidbodyComponent.h>
#include <SweptControllerComponent.h>

NATIVE_COMPONENT_DEFINITION( ApplyImpulseOnHit );

using namespace ursine;
using namespace ecs;

ApplyImpulseOnHit::ApplyImpulseOnHit(void)
    : BaseComponent( )
    , m_impulse( 0.0f )
    , m_effectSweptController( true )
{
}

ApplyImpulseOnHit::~ApplyImpulseOnHit(void)
{
    GetOwner( )->Listener( this )
        .Off( ENTITY_COLLISION_PERSISTED, &ApplyImpulseOnHit::onCollision );
}

bool ApplyImpulseOnHit::GetEffectSweptController(void) const
{
    return m_effectSweptController;
}

void ApplyImpulseOnHit::SetEffectSweptController(bool flag)
{
    m_effectSweptController = flag;
}

float ApplyImpulseOnHit::GetImpulse(void) const
{
    return m_impulse;
}

void ApplyImpulseOnHit::SetImpulse(float impulse)
{
    m_impulse = impulse;

    NOTIFY_COMPONENT_CHANGED( "impulse", m_impulse );
}

const SVec3& ApplyImpulseOnHit::GetLocalDirectionInfluence(void) const
{
    return m_localDirection;
}

void ApplyImpulseOnHit::SetLocalDirectionInfluence(const SVec3& direction)
{
    m_localDirection = direction;

    NOTIFY_COMPONENT_CHANGED( "localDirectionInfluence", m_localDirection );
}

const SVec3& ApplyImpulseOnHit::GetWorldDirectionInfluence(void) const
{
    return m_worldDirection;
}

void ApplyImpulseOnHit::SetWorldDirectionInfluence(const SVec3& direction)
{
    m_worldDirection = direction;

    NOTIFY_COMPONENT_CHANGED( "worldDirectionInfluence", m_worldDirection );
}

void ApplyImpulseOnHit::OnInitialize(void)
{
    GetOwner( )->Listener( this )
        .On( ENTITY_COLLISION_PERSISTED, &ApplyImpulseOnHit::onCollision );
}

void ApplyImpulseOnHit::onCollision(EVENT_HANDLER(Entity))
{
    EVENT_ATTRS(Entity, physics::CollisionEventArgs);

    if (m_effectSweptController && args->otherEntity->HasComponent<SweptController>( ))
    {
        auto controller = args->otherEntity->GetComponent<SweptController>( );

        auto &localToWorld = GetOwner( )->GetTransform( )->GetLocalToWorldMatrix( );
        auto dir = m_worldDirection + localToWorld.TransformVector( m_localDirection );

        controller->AddImpulse( dir * m_impulse );
    }
    else if (args->otherEntity->HasComponent<Rigidbody>( ))
    {
        auto body = args->otherEntity->GetComponent<Rigidbody>( );

        auto &localToWorld = GetOwner( )->GetTransform( )->GetLocalToWorldMatrix( );
        auto dir = m_worldDirection + localToWorld.TransformVector( m_localDirection );

        body->AddImpulse( dir * m_impulse );
    }
}
