/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** InteractionBay.cpp
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/

#include <Precompiled.h>
#include "InteractionBayComponent.h"
#include "ComponentIncludes.h"
#include "InteractableComponent.h"
#include <CollisionEventArgs.h>


NATIVE_COMPONENT_DEFINITION( InteractionBay ) ;

using namespace ursine;


namespace InteractionBayUtils
{
    bool Compare(const InteractInfo& lhs, const InteractInfo& rhs)
    {
        Interactable::InteractType type = lhs.second->GetInteractType( );

        if ( type == rhs.second->GetInteractType( ) )
            return lhs.first < rhs.first;

        if ( type == Interactable::CONTINUE )
            return true;

        return false;
    }
} // InteractionBayUtils namespace


InteractionBay::InteractionBay(void) :
    BaseComponent( ),
    m_interactQueue( InteractionBayUtils::Compare )
{
}

InteractionBay::~InteractionBay(void)
{
    Clear( );
}

void InteractionBay::OnInitialize(void)
{
}

void InteractionBay::OnCollision(EVENT_HANDLER(ursine::ecs::ENTITY_COLLISION_PERSISTED))
{
    EVENT_ATTRS( ursine::ecs::Entity, ursine::physics::CollisionEventArgs );

    if ( args->otherEntity->HasComponent<Interactable>( ) )
    {
        float distSqu = args->otherEntity->GetTransform( )->GetWorldPosition( ).DistanceSquared( GetOwner( )->GetTransform( )->GetWorldPosition( ) );

        m_interactQueue.push( std::make_pair( distSqu, args->otherEntity->GetComponent< Interactable >( ) ) );
    }

}

void InteractionBay::AddInteractable(float distSquared, Interactable* interactable)
{
    m_interactQueue.push( std::make_pair( distSquared, interactable ) );
}

void InteractionBay::Update(void)
{
    m_prevInteractables = m_currInteractables;

    m_currInteractables.clear( );

    Clear( );
}

void InteractionBay::Clear(void)
{
    m_interactQueue = InteractQueue( InteractionBayUtils::Compare );
}


