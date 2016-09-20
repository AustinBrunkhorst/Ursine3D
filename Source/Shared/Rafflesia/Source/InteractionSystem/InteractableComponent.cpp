/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** InteractableComponent.cpp
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/

#include "Precompiled.h"
#include "InteractableComponent.h"
#include "BaseInteraction.h"
#include "InteractionBayComponent.h"
#include "CollisionEventArgs.h"
#include "EntityEvent.h"

NATIVE_COMPONENT_DEFINITION( Interactable );

using namespace ursine;

Interactable::Interactable(void) 
    : BaseComponent( )
    , m_interaction( nullptr )
    , m_startFunc( nullptr )
    , m_interactFunc( nullptr )
    , m_stopFunc( nullptr )
    , m_type( NOT_SET )
{
}

Interactable::~Interactable(void)
{
    GetOwner( )->Listener( this )
        .Off( ursine::ecs::ENTITY_COLLISION_PERSISTED, &Interactable::onCollision );
}

void Interactable::OnInitialize(void)
{
    GetOwner( )->Listener( this )
        .On( ursine::ecs::ENTITY_COLLISION_PERSISTED, &Interactable::onCollision );
}

void Interactable::StartInteraction(const ecs::EntityHandle &entity)
{
    (m_interaction->*m_startFunc)( entity );
}

void Interactable::Interact(const ecs::EntityHandle &entity)
{
    (m_interaction->*m_interactFunc)( entity );
}

void Interactable::StopInteraction(const ecs::EntityHandle &entity)
{
    (m_interaction->*m_stopFunc)( entity );
}

void Interactable::SetUpInteractable(Interaction *interaction, InteractType type)
{
    m_interaction  = interaction;

    m_startFunc    = &Interaction::StartInteraction;
    m_interactFunc = &Interaction::Interact;
    m_stopFunc     = &Interaction::StopInteraction;

    m_type = type;
}

Interactable::InteractType Interactable::GetInteractType(void) const
{
    return m_type;
}

void Interactable::onCollision(EVENT_HANDLER(ursine::ecs::Entity))
{
    EVENT_ATTRS(ursine::ecs::Entity, ursine::physics::CollisionEventArgs);

    if ( args->otherEntity->HasComponent< InteractionBay >( ) )
    {
        float distSqu = args->otherEntity->GetTransform( )->GetWorldPosition( ).DistanceSquared(GetOwner( )->GetTransform( )->GetWorldPosition( ));

        args->otherEntity->GetComponent< InteractionBay >( )->AddInteractable( distSqu, this );
    }
}

