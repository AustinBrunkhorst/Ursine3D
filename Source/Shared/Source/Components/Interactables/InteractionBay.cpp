/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** InteractionBay.cpp
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/

#include <Precompiled.h>
#include "InteractionBay.h"
#include "ComponentIncludes.h"
#include "InteractableComponent.h"
#include <CollisionEventArgs.h>


NATIVE_COMPONENT_DEFINITION( InteractionBay ) ;

using namespace ursine;


InteractionBay::InteractionBay(void) :
    BaseComponent( ),
    m_prevInteractable(nullptr)
{
}

InteractionBay::~InteractionBay(void)
{
    Clear( );

    GetOwner( )->Listener( this )
         .Off( ursine::ecs::ENTITY_COLLISION_PERSISTED, &InteractionBay::OnCollision );
}

void InteractionBay::OnInitialize(void)
{
    GetOwner( )->Listener( this )
        .On( ursine::ecs::ENTITY_COLLISION_PERSISTED, &InteractionBay::OnCollision );
}

void InteractionBay::OnCollision(EVENT_HANDLER(ursine::ecs::ENTITY_COLLISION_PERSISTED))
{
    EVENT_ATTRS( ursine::ecs::Entity, ursine::physics::CollisionEventArgs );

    if ( args->otherEntity->HasComponent<Interactable>( ) )
    {
        m_interactables.push_back( args->otherEntity->GetComponent<Interactable>( ) );
        m_transforms.push_back( args->otherEntity->GetTransform( ) );
    }

}

void InteractionBay::Clear(void)
{
    m_interactables.clear( );
    m_transforms.clear( );
}


