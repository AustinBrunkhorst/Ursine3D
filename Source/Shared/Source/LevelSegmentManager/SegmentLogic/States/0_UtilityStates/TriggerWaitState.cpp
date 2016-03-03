// throw smart assertions if there isn't a trigger by the name given

/* ---------------------------------------------------------------------------
** Team Bear King
** 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** TriggerWaitState.cpp
**
** Author:
** -Jason Burch - j.orion@digipen.edu
**
** Contributors:
** -<list in same format as author if applicable>
** ------------------------------------------------------------------------ - */

#include "Precompiled.h"

#include "TriggerWaitState.h"

#include "SegmentLogicStateIncludes.h"
#include <Game Engine/Scene/Entity/EntityEvent.h>
#include <PlayerLogic/PlayerIdComponent.h>
#include <CollisionEventArgs.h>

using namespace ursine;
using namespace ecs;

TriggerWaitState::TriggerWaitState(std::string& name)
	: SegmentLogicState( "Trigger Wait State" )
	, m_triggered( false )
	, m_entityName( name )
{ 
}

void TriggerWaitState::Enter(SegmentLogicStateMachine *machine)
{
	auto segmentManager = machine->GetSegmentManager( );
	auto world = segmentManager->GetOwner( )->GetWorld( );

	auto trigger = world->GetEntityFromName( m_entityName );

	UAssert( trigger, "Error: This state requires a trigger entity." );

	// subscribe to trigger
	trigger->Listener( this )
		.On( ENTITY_COLLISION_PERSISTED, &TriggerWaitState::onTrigger );
}

void TriggerWaitState::onTrigger(EVENT_HANDLER(ursine::ecs::World))
{
	EVENT_ATTRS( ursine::ecs::Entity, ursine::physics::CollisionEventArgs );

	if (args->otherEntity->HasComponent<PlayerID>( ))
	{
		m_triggered = true;
		args->thisEntity->Listener( this )
			.Off( ENTITY_COLLISION_PERSISTED, &TriggerWaitState::onTrigger );
	}
}
