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
#include <Application.h>

NATIVE_COMPONENT_DEFINITION( ApplyImpulseOnHit );

using namespace ursine;
using namespace ecs;

ApplyImpulseOnHit::ApplyImpulseOnHit(void)
    : BaseComponent( )
    , m_impulse( 0.0f )
    , m_interval( 0.1f )
    , m_effectSweptController( true )
    , m_listenToChildren( false )
    , m_serialized( false )
{
}

ApplyImpulseOnHit::~ApplyImpulseOnHit(void)
{
    GetOwner( )->Listener( this )
        .Off( ENTITY_COLLISION_PERSISTED, &ApplyImpulseOnHit::onCollision );

    if (m_listenToChildren)
        connectToChildrenCollisionEvents( false, GetOwner( )->GetChildren( ) );

    auto world = GetOwner( )->GetWorld( );

    if (world)
        world->Listener( this )
            .Off( WORLD_UPDATE, &ApplyImpulseOnHit::onUpdate );
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

float ApplyImpulseOnHit::GetInterval(void) const
{
    return m_interval;
}

void ApplyImpulseOnHit::SetInterval(float interval)
{
    m_interval = interval;

    NOTIFY_COMPONENT_CHANGED( "interval", m_interval );
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

bool ApplyImpulseOnHit::GetListenToChildren(void) const
{
    return m_listenToChildren;
}

void ApplyImpulseOnHit::SetListenToChildren(bool flag)
{
    if (m_listenToChildren == flag)
        return;

    if (m_serialized)
        connectToChildrenCollisionEvents( flag, GetOwner( )->GetChildren( ) );

    m_listenToChildren = flag;

    NOTIFY_COMPONENT_CHANGED( "listenToChildren", m_listenToChildren );
}

const ursine::IgnoredEntityArray& ApplyImpulseOnHit::GetIgnoredEntities(void) const
{
    return m_ignored;
}

void ApplyImpulseOnHit::SetIgnoredEntities(const ursine::IgnoredEntityArray& ignored)
{
    m_ignored = ignored;
}

void ApplyImpulseOnHit::OnInitialize(void)
{
    GetOwner( )->Listener( this )
        .On( ENTITY_COLLISION_PERSISTED, &ApplyImpulseOnHit::onCollision )
        .On( ENTITY_HIERARCHY_SERIALIZED, &ApplyImpulseOnHit::onHierarchySerialized );

    auto world = GetOwner( )->GetWorld( );

    if (world)
        world->Listener( this )
            .On( WORLD_UPDATE, &ApplyImpulseOnHit::onUpdate );
}

void ApplyImpulseOnHit::onHierarchySerialized(EVENT_HANDLER(Entity))
{
    EVENT_ATTRS(Entity, EntityEventArgs);

    m_serialized = true;

    if (m_listenToChildren)
        connectToChildrenCollisionEvents( true, GetOwner( )->GetChildren( ) );

    sender->Listener( this )
        .Off( ENTITY_HIERARCHY_SERIALIZED, &ApplyImpulseOnHit::onHierarchySerialized );
}

void ApplyImpulseOnHit::onCollision(EVENT_HANDLER(Entity))
{
    EVENT_ATTRS(Entity, physics::CollisionEventArgs);

    // if entity name is on ignore list
    for (auto name : m_ignored)
    {
        if (name.compare( args->otherEntity->GetRoot( )->GetName( ) ) == 0)
            return;
    }

    if (m_effectSweptController && 
        args->otherEntity->HasComponent<SweptController>( ) &&
        canAffect( args->otherEntity ))
    {
        auto controller = args->otherEntity->GetComponent<SweptController>( );

        auto &localToWorld = GetOwner( )->GetTransform( )->GetLocalToWorldMatrix( );
        auto dir = m_worldDirection + localToWorld.TransformVector( m_localDirection );

        controller->AddImpulse( dir * m_impulse );
    }
    else if (args->otherEntity->HasComponent<Rigidbody>( ) &&
             canAffect( args->otherEntity ))
    {
        auto body = args->otherEntity->GetComponent<Rigidbody>( );

        auto &localToWorld = GetOwner( )->GetTransform( )->GetLocalToWorldMatrix( );
        auto dir = m_worldDirection + localToWorld.TransformVector( m_localDirection );

        body->AddImpulse( dir * m_impulse );
    }
}

bool ApplyImpulseOnHit::canAffect(const EntityHandle &entity)
{
    auto itr = m_impulseMap.find( entity );

    if (m_impulseMap.end( ) == itr)
    {
        m_impulseMap[ entity ] = m_interval;
        return true;
    }
    else
        return false;
}

void ApplyImpulseOnHit::onUpdate(EVENT_HANDLER(World))
{
    auto dt = Application::Instance->GetDeltaTime( );

    std::vector<EntityHandle> toRemove;

    for (auto &pair : m_impulseMap)
    {
        pair.second -= dt;

        if (pair.second <= 0.0f)
            toRemove.push_back( pair.first );
    }

    for (auto &remove : toRemove)
    {
        m_impulseMap.erase( remove );
    }
}

void ApplyImpulseOnHit::connectToChildrenCollisionEvents(bool connect, const std::vector<EntityID> *children)
{
    if (children == nullptr)
        return;

    auto world = GetOwner( )->GetWorld( );

    if (world == nullptr)
        return;

    for (auto &child : *children)
    {
        auto entity = world->GetEntity( child );

        if (!entity || entity->IsDeleting( ))
            continue;

        if (connect)
            entity->Listener( this ).On( ENTITY_COLLISION_PERSISTED, &ApplyImpulseOnHit::onCollision );
        else
            entity->Listener( this ).Off( ENTITY_COLLISION_PERSISTED, &ApplyImpulseOnHit::onCollision );

        connectToChildrenCollisionEvents( connect, entity->GetChildren( ) );
    }
}

