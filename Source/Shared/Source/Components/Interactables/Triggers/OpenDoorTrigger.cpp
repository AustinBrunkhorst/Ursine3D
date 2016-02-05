/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** WeaponPickup.cpp
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/

#include <Precompiled.h>
#include "OpenDoorTrigger.h"
#include "CommandQueueComponent.h"
#include "PlayerIdComponent.h"
#include "GameEvents.h"

NATIVE_COMPONENT_DEFINITION(OpenDoorTrigger);


OpenDoorTrigger::OpenDoorTrigger(void) :
    BaseComponent( ),
    m_active( true )
{
    
}

OpenDoorTrigger::~OpenDoorTrigger(void)
{
    
}



void OpenDoorTrigger::StartInteraction(const CommandQueue* queue, ursine::ecs::EntityUniqueID id)
{
    if ( queue->GetOwner( )->HasComponent<PlayerID>( ) && m_active )
    {
        GetOwner( )->Dispatch(game::OPEN_DOOR, ursine::ecs::EntityEventArgs::Empty );
        m_active = false;
    }
}

void OpenDoorTrigger::Interact(const CommandQueue* queue, ursine::ecs::EntityUniqueID id)
{
    
}

void OpenDoorTrigger::StopInteraction(const CommandQueue* queue, ursine::ecs::EntityUniqueID id)
{
}
