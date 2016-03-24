/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** DisempowerTriggerComponent.cpp
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/

#include <Precompiled.h>
#include "DisempowerTriggerComponent.h"
#include "ComponentIncludes.h"
#include "EmpowerTriggerComponent.h"

NATIVE_COMPONENT_DEFINITION(DisempowerTrigger);

using namespace ursine;


DisempowerTrigger::DisempowerTrigger(void) :
    BaseComponent( ),
    MadRaidTrigger( )
{
}

DisempowerTrigger::~DisempowerTrigger(void)
{
    // no longer want to be updated
    GetOwner( )->GetWorld( )->Listener(this)
        .Off(ursine::ecs::WORLD_UPDATE, &DisempowerTrigger::onUpdate);

    // unlisten for me death
    GetOwner( )->Listener(this)
        .Off(ursine::ecs::ENTITY_REMOVED, &DisempowerTrigger::onDeath);
}


void DisempowerTrigger::OnInitialize(void)
{
    BaseRaidTrigger::Initialize( GetOwner( ) );

    // want to be updated
    GetOwner( )->GetWorld( )->Listener(this)
        .On(ursine::ecs::WORLD_UPDATE, &DisempowerTrigger::onUpdate);

    // listen for me death
    GetOwner( )->Listener(this)
        .On(ursine::ecs::ENTITY_REMOVED, &DisempowerTrigger::onDeath)
        .On(game::EMPOWER_ACTIVE, &DisempowerTrigger::OnEmpowerActive)
        .On(game::EMPOWER_UNACTIVE, &DisempowerTrigger::OnEmpowerUnactive);;

    GetOwner( )->GetComponent<Interactable>( )->SetUpInteractable(this, Interactable::CONTINUE);
}

void DisempowerTrigger::OnSceneReady(ursine::Scene* scene)
{
    BaseRaidTrigger::Initialize(GetOwner( ));
}



void DisempowerTrigger::ChildStartInteractionLogic( )
{
    if ( !m_playerCount )
    {
        auto child = GetOwner( )->GetComponentInChildren< EmpowerTrigger >( );

        if ( child )
            child->GetOwner( )->Dispatch(game::DISEMPOWER_ACTIVE, ursine::ecs::EntityEventArgs::Empty);
    }
}

void DisempowerTrigger::ChildStopInteractionLogic( )
{
    if ( !m_playerCount )
    {
        auto child = GetOwner( )->GetComponentInChildren< EmpowerTrigger>( );

        if ( child )
            child->GetOwner( )->Dispatch(game::DISEMPOWER_UNACTIVE, ursine::ecs::EntityEventArgs::Empty);
    }
}

void DisempowerTrigger::ChildSuccessLogic(int id)
{
    ChangeToSuccessState( );
}

void DisempowerTrigger::ChildUpdate(void)
{
    if ( m_empowerActive && MAD )
        m_madTimer = m_madTime;

    else if ( m_empowerActive && !m_playerCount )
        ChangeToMadState( -1 );
}

void DisempowerTrigger::OnEmpowerActive(EVENT_HANDLER(ursine::ecs::Entity))
{
    m_empowerActive = true;

    if ( !m_playerCount )
        ChangeToMadState( -1 );
}

void DisempowerTrigger::OnEmpowerUnactive(EVENT_HANDLER(ursine::ecs::Entity))
{
    m_empowerActive = false;
}
