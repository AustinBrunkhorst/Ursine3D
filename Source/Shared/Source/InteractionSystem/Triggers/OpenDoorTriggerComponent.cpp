/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** WeaponPickup.cpp
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/

#include <Precompiled.h>
#include "OpenDoorTriggerComponent.h"
#include "CommandQueueComponent.h"
#include "PlayerIdComponent.h"
#include "GameEvents.h"

NATIVE_COMPONENT_DEFINITION( OpenDoorTrigger );

OpenDoorTrigger::OpenDoorTrigger(void) 
    : BaseComponent( )
    , m_clear( false )
{
    
}

OpenDoorTrigger::~OpenDoorTrigger(void)
{
    GetOwner( )->GetWorld( )->Listener(this)
        .Off(game::AREA_CLEAR, &OpenDoorTrigger::OnAreaClear);
}

void OpenDoorTrigger::OnInitialize(void)
{
    GetOwner( )->GetWorld( )->Listener( this )
        .On( game::AREA_CLEAR, &OpenDoorTrigger::OnAreaClear );
}

void OpenDoorTrigger::StartInteraction(const CommandQueue* queue, ursine::ecs::EntityID id)
{
    if ( queue->GetOwner( )->HasComponent<PlayerID>( ) && m_clear )
    {
        GetOwner( )->GetWorld( )->Dispatch(game::OPEN_DOOR, ursine::ecs::EntityEventArgs::Empty );
        GetOwner( )->Delete( );
    }
}

void OpenDoorTrigger::Interact(const CommandQueue* queue, ursine::ecs::EntityID id)
{
    if ( queue->GetOwner( )->HasComponent<PlayerID>( ) && m_clear )
    {
        GetOwner( )->GetWorld( )->Dispatch(game::OPEN_DOOR, ursine::ecs::EntityEventArgs::Empty);
        GetOwner( )->Delete( );
    }
}

void OpenDoorTrigger::StopInteraction(const CommandQueue* queue, ursine::ecs::EntityID id)
{
}

void OpenDoorTrigger::OnAreaClear(EVENT_HANDLER(game::AREA_CLEAR))
{
    m_clear = true;
}
