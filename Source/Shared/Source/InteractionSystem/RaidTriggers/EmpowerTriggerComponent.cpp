/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EmpowerTriggerComponent.cpp
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/

#include <Precompiled.h>
#include "EmpowerTriggerComponent.h"
#include "ComponentIncludes.h"
#include "GameEvents.h"

NATIVE_COMPONENT_DEFINITION( EmpowerTrigger ) ;

using namespace ursine;


EmpowerTrigger::EmpowerTrigger(void) :
    BaseComponent( ),
    MadRaidTrigger( ),
    m_disempowerActive( false ),
    m_playerActive( false )
{
}

EmpowerTrigger::~EmpowerTrigger(void)
{
    // no longer want to be updated
    GetOwner( )->GetWorld( )->Listener(this)
        .Off(ursine::ecs::WORLD_UPDATE, &EmpowerTrigger::onUpdate);
    
    // unlisten for me death
    GetOwner( )->Listener(this)
        .Off(ursine::ecs::ENTITY_REMOVED, &EmpowerTrigger::onDeath);
}


void EmpowerTrigger::OnInitialize(void)
{
    BaseRaidTrigger::Initialize( GetOwner( ) );

    // want to be updated
    GetOwner( )->GetWorld( )->Listener(this)
        .On(ursine::ecs::WORLD_UPDATE, &EmpowerTrigger::onUpdate);
    
    // listen for me death
    GetOwner( )->Listener(this)
        .On( ursine::ecs::ENTITY_REMOVED, &EmpowerTrigger::onDeath )
        .On( game::DISEMPOWER_ACTIVE, &EmpowerTrigger::OnDisempowerActive)
        .On( game::DISEMPOWER_UNACTIVE, &EmpowerTrigger::OnDisempowerUnactive);

    GetOwner( )->GetComponent<Interactable>( )->SetUpInteractable(this);
}


void EmpowerTrigger::ChildStartInteractionLogic(void)
{
    if ( !m_playerCount )
        GetOwner( )->GetRoot( )->Dispatch( game::EMPOWER_ACTIVE, ursine::ecs::EntityEventArgs::Empty );
}

void EmpowerTrigger::ChildStopInteractionLogic(void)
{
    if ( !m_playerCount )
        GetOwner( )->GetRoot( )->Dispatch(game::EMPOWER_UNACTIVE, ursine::ecs::EntityEventArgs::Empty);
}

void EmpowerTrigger::ChildSuccessLogic(int id)
{
    if ( m_disempowerActive )
        ChangeToSuccessState( );

    else
        ChangeToMadState( -1 );
} 

void EmpowerTrigger::ChildUpdate(void)
{
}


void EmpowerTrigger::OnDisempowerActive(EVENT_HANDLER(ursine::ecs::Entity))
{
    m_disempowerActive = true;
}

void EmpowerTrigger::OnDisempowerUnactive(EVENT_HANDLER(ursine::ecs::Entity))
{
    m_disempowerActive = false;
}
