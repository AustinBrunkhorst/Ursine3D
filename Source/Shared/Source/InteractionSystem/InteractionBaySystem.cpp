
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
    PrevIter prevIt = bay->m_prevInteractables.begin( );
    PrevIter prevEnd = bay->m_prevInteractables.end( );

    EntityHandle bayOwner = bay->GetOwner( );

    // loop through all interactables in bay
    while ( !bay->m_interactQueue.empty( ) )
    {
        if ( InteractUpdate( bay, bay->m_interactQueue.top( ), prevIt, prevEnd ) )
            break;

        bay->m_interactQueue.pop( );
    }

    // left over prev interactables that a no longer being interacted with
    for ( ; prevIt != prevEnd; ++prevIt )
        prevIt->second->StopInteraction( bayOwner );

    bay->Clear( );
}

bool InteractionBaySystem::InteractUpdate(InteractionBay* bay, const InteractInfo& currInteractInfo, PrevIter& prevIt, PrevIter& prevEnd)
{
    PrevIter temp;

    EntityHandle bayOwner = bay->GetOwner( );

    // if prev and curr not == then an inconsistency was found
    // loop until consistency restored or curr is new interaction
    while ( *prevIt != currInteractInfo && prevIt != prevEnd )
    {
        // new interactable found
        if ( currInteractInfo < *prevIt )
        {
            // start interction
            currInteractInfo.second->StartInteraction( bayOwner );

            // add to prev of bay
            bay->m_prevInteractables.insert( prevIt, currInteractInfo );

            // kick out of loop since new consistencey found
            break;
        }

        // no longer interacting w/ prev obj
        prevIt->second->StopInteraction( bayOwner );

        // no longer should be in prev
        bay->m_prevInteractables.erase( prevIt++ );
    }

    // new interactable found
    if ( prevIt == prevEnd )
    {
        // start interaction
        currInteractInfo.second->StartInteraction(bayOwner);

        // add to prev of bay
        bay->m_prevInteractables.push_back(currInteractInfo);
    }

    // curr is the prev so increment for next interaction in bay
    else if ( *prevIt == currInteractInfo )
        ++prevIt;

    // perform interaction with interactable
    currInteractInfo.second->Interact( bayOwner );

    // will end if interact type is end
    return currInteractInfo.second->GetInteractType( ) == Interactable::END;
}



