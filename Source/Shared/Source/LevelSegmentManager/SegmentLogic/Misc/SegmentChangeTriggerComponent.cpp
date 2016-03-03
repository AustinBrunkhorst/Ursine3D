/* ----------------------------------------------------------------------------
** Team Bear King
** 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** SegmentChangeTrigger.cpp
**
** Author:
** - Jason Burch - j.orion@digipen.edu
**
** --------------------------------------------------------------------------*/

#include <Precompiled.h>
#include "SegmentChangeTriggerComponent.h"
#include "PlayerIdComponent.h"
#include "LevelSegmentManagerComponent.h"
#include "GameEvents.h"
#include <Core\CoreSystem.h>
#include <Filter.h>
#include <CollisionEventArgs.h>
#include <World.h>

NATIVE_COMPONENT_DEFINITION(SegmentChangeTrigger);


SegmentChangeTrigger::SegmentChangeTrigger(void)
	: BaseComponent( )
	, m_newSegment( LevelSegments_enum::Empty )
	, m_currentSegment(LevelSegments_enum::Empty)
	, m_active( false )
{
}

SegmentChangeTrigger::~SegmentChangeTrigger(void)
{
	GetOwner( )->Listener( this )
		.Off( ursine::ecs::ENTITY_COLLISION_PERSISTED, &SegmentChangeTrigger::Activate );
}

LevelSegments SegmentChangeTrigger::GetCurrentLevelSegment(void) const
{
	return m_currentSegment;
}

void SegmentChangeTrigger::SetCurrentLevelSegment(LevelSegments levelSegment)
{
	m_currentSegment = levelSegment;
}

LevelSegments SegmentChangeTrigger::GetNewLevelSegment(void) const
{
	return m_newSegment;
}

void SegmentChangeTrigger::SetNewLevelSegment(LevelSegments levelSegment)
{
	m_newSegment = levelSegment;
}

void SegmentChangeTrigger::OnInitialize(void)
{
	GetOwner( )->Listener( this )
		.On( ursine::ecs::ENTITY_COLLISION_PERSISTED, &SegmentChangeTrigger::Activate );
}


void SegmentChangeTrigger::Activate(EVENT_HANDLER(ursine::ecs::Entity))
{
	EVENT_ATTRS( ursine::ecs::Entity, ursine::physics::CollisionEventArgs );

	if (m_newSegment == LevelSegments::Empty)
		return;

	if ( args->otherEntity->HasComponent<PlayerID>( ) )
	{
		// Change the segment
		auto levelSegmentManagers = GetOwner( )->GetWorld( )->
			GetEntitiesFromFilter(ursine::ecs::Filter( ).All<LevelSegmentManager>( ) );

		UAssert( 
			levelSegmentManagers.size( ), 
		    "Error: This component requires there to be"
			" a level segment manager in order to switch the segment" 
		);

		// get [0] element
		auto stateMachine = levelSegmentManagers[ 0 ]->GetComponent<LevelSegmentManager>( );

		if (m_newSegment != stateMachine->GetCurrentSegment( ))
			return;
		
		// call change state
		stateMachine->SetCurrentSegment( m_newSegment );
		
		// delete myself because this is a one time use ting
		//GetOwner()->Delete();
	}

}
