/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** PlayerDownedObjectComponent.cpp
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/

#include <Precompiled.h>
#include "PlayerDownedObjectComponent.h"
#include "ComponentIncludes.h"

NATIVE_COMPONENT_DEFINITION( PlayerDownedObject );

using namespace ursine;


PlayerDownedObject::PlayerDownedObject(void) :
    BaseComponent( )
{
}

PlayerDownedObject::~PlayerDownedObject(void)
{
    GetOwner( )->Listener(this)
        .Off(ursine::ecs::ENTITY_HIERARCHY_SERIALIZED, &PlayerDownedObject::onHeirarchy);
}

const ursine::resources::ResourceReference& PlayerDownedObject::GetReviveObject(void) const
{
    return m_reviveObject;
}

void PlayerDownedObject::SetReviveObject(const ursine::resources::ResourceReference& archetype)
{
    m_reviveObject = archetype;

    NOTIFY_COMPONENT_CHANGED("ReviveObject", m_reviveObject);
}

void PlayerDownedObject::OnInitialize(void)
{
    GetOwner( )->Listener( this )
        .On( ursine::ecs::ENTITY_HIERARCHY_SERIALIZED, &PlayerDownedObject::onHeirarchy );
}

void PlayerDownedObject::onHeirarchy(EVENT_HANDLER(ursine::ecs::Entity))
{
    Health* health = GetOwner( )->GetComponent< Health >( );

    if ( health )
    {
        health->Listener( this )
            .On( HealthEvents::HEALTH_ZERO, &PlayerDownedObject::onZeroHealth );
    }
}

void PlayerDownedObject::onZeroHealth(EVENT_HANDLER(ursine::ecs::Entity))
{
    ursine::ecs::EntityHandle obj = GetOwner( )->GetWorld( )->CreateEntityFromArchetype( m_reviveObject );

    if ( obj )
    {
        obj->GetTransform( )->SetWorldPosition( GetOwner( )->GetTransform( )->GetWorldPosition( ) );
        GetOwner( )->GetTransform( )->AddChild( obj->GetTransform( ) );
    }
}



