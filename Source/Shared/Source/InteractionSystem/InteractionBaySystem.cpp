
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

namespace
{
    // find the closest interactable to interaction bay
    int SortBayInteractables(Transform* trans, const InteractionBay::TransVec interactables)
    {
        float minDist = FLT_MAX;
        float distSquared;
        int closestIndex = -1;
        int index = 0;

        for ( auto it : interactables )
        {
            distSquared = trans->GetWorldPosition( ).DistanceSquared( (*it).GetWorldPosition( ) );

            if ( distSquared < minDist )
            {
                minDist = distSquared;
                closestIndex = index;
            }

            ++index;
        }

        return closestIndex;
    }

} // unnamed namespace


//////////////////////////////////
////  Interaction Bay System  ////
//////////////////////////////////

InteractionBaySystem::InteractionBaySystem(ursine::ecs::World* world)
    : FilterSystem( world, Filter( ).One< InteractionBay >( ) )
{
}

void InteractionBaySystem::Enable(ursine::ecs::Entity* entity)
{
    auto uniqueID = entity->GetUniqueID( );

    // grab all comps needed
    m_interactionBays[ uniqueID ] = entity->GetComponent< InteractionBay >( );
}

void InteractionBaySystem::Disable(ursine::ecs::Entity* entity)
{
    auto uniqueID = entity->GetUniqueID( );

    m_interactionBays.erase( uniqueID );
}

void InteractionBaySystem::onUpdate(EVENT_HANDLER(World))
{
    //float dt = Application::Instance->GetDeltaTime( );

    InteractionBay* bay;

    int closestIndex = 0;

    for ( auto it : m_interactionBays )
    {
        // grab current Interaction bay
        bay = it.second;

        // find closest interactable index to bay
        closestIndex = SortBayInteractables( bay->GetOwner( )->GetTransform( ), bay->m_transforms );

        // update le bay and its interaction
        UpdateBay( bay, closestIndex );
    }

}

void InteractionBaySystem::UpdateBay(InteractionBay* bay, const int closestIndex)
{
    Interactable* currInteractable;
    CommandQueue* queue = bay->GetOwner( )->GetComponent<CommandQueue>( );
    ursine::ecs::EntityUniqueID id = bay->GetOwner( )->GetUniqueID( );

    if ( !bay->m_prevInteractable && closestIndex == -1 )
        return;

    if ( closestIndex == -1 )
    {
        bay->m_prevInteractable->StopInteraction(queue, id);
        bay->m_prevInteractable = nullptr;
        return;
    }

    // get new current interactable
    currInteractable = bay->m_interactables[ closestIndex ];

    // was a new interactable found
    if ( bay->m_prevInteractable != currInteractable )
    {
        // end old interaction
        if ( bay->m_prevInteractable )
            bay->m_prevInteractable->StopInteraction( queue, id );

        // Start new interaction
        currInteractable->StartInteraction( queue, id );

        // update previous
        bay->m_prevInteractable = currInteractable;
    }
    
    // perform interaction with interactable
    currInteractable->Interact( queue, id );

    bay->Clear( );
}


