
/* ----------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** InteractionBaySystem.cpp
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** --------------------------------------------------------------------------*/

#include "Precompiled.h"
#include "InteractionBaySystem.h"
#include "TransformComponent.h"
#include "InteractionBayComponent.h"
#include "InteractableComponent.h"
#include "CommandQueueComponent.h"


ENTITY_SYSTEM_DEFINITION( InteractionBaySystem ) ;


using namespace ursine;
using namespace ursine::ecs;


//////////////////////////////////
////  Interaction Bay System  ////
//////////////////////////////////

InteractionBaySystem::InteractionBaySystem(World* world)
    : FilterSystem( world, Filter( ).One< InteractionBay >( ) )
{
}

void InteractionBaySystem::Enable(const EntityHandle &entity)
{
    // grab all comps needed
    m_interactionBays[ entity ] = entity->GetComponent<InteractionBay>( );
}

void InteractionBaySystem::Disable(const EntityHandle &entity)
{
    m_interactionBays.erase( entity );
}

void InteractionBaySystem::onUpdate(EVENT_HANDLER(World))
{
    //float dt = Application::Instance->GetDeltaTime( );

    InteractionBay* bay;

    for ( auto it : m_interactionBays )
    {
        // grab current Interaction bay
        bay = it.second;

        UpdateBay( bay );
    }
}

void InteractionBaySystem::UpdateBay(InteractionBay* bay)
{
    EntityHandle bayOwner = bay->GetOwner( );

    // loop through all interactables in bay
    while ( !bay->m_interactQueue.empty( ) )
    {
        if ( InteractUpdate( bay, bay->m_interactQueue.top( ).second ) )
            break;

        bay->m_interactQueue.pop( );
    }

    PrevIter prevIt = bay->m_prevInteractables.begin( );
    PrevIter prevEnd = bay->m_prevInteractables.end( );

    // left over prev interactables that a no longer being interacted with
    for ( ; prevIt != prevEnd; ++prevIt )
        ( *prevIt )->StopInteraction( bayOwner );

    bay->Update( );
}

bool InteractionBaySystem::InteractUpdate(InteractionBay* bay, Interactable* currInteractable)
{
    PrevIter prevInteractable = bay->m_prevInteractables.find( currInteractable );

    EntityHandle bayOwner = bay->GetOwner( );

    // new interactable found
    if ( prevInteractable == bay->m_prevInteractables.end( ) )
    {
        // start interaction
        currInteractable->StartInteraction( bayOwner );

        // add to prev of bay
        bay->m_currInteractables.insert( currInteractable );
    }

    // remove from prev list so we do not stop interaction later
    else
        bay->m_prevInteractables.erase( prevInteractable );

    // perform interaction with interactable
    currInteractable->Interact( bayOwner );

    // will end if interact type is end
    return currInteractable->GetInteractType( ) == Interactable::END;
}



