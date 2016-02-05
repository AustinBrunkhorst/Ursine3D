#include "Precompiled.h"

#include "StartRoomDirectorSystem.h"

#include <CameraComponent.h>

#include "ElevatorLiftMoverComponent.h"
#include "CameraAnimatorComponent.h"

using namespace ursine;
using namespace ecs;

ENTITY_SYSTEM_DEFINITION( StartRoomDirectorSystem );

StartRoomDirectorSystem::StartRoomDirectorSystem(ursine::ecs::World *world)
    : EntitySystem( world )
{

}

void StartRoomDirectorSystem::OnAfterLoad(void)
{
    auto elevators = m_world->GetEntitiesFromFilter( Filter( ).All<ElevatorLiftMover>( ) );

	if (elevators.size( ) == 0)
		return;

	auto elevator = elevators[ 0 ];
	auto mover = elevator->GetComponent<ElevatorLiftMover>( );

	// Start the mover
	mover->StartMoving( );

	// Spawn the players at the spawn points
	auto s1 = elevator->GetChildByName( "SpawnPoint1" );
	auto s2 = elevator->GetChildByName( "SpawnPoint2" );

	auto p1 = m_world->CreateEntityFromArchetype(
		WORLD_ARCHETYPE_PATH "Player1.uatype", "Player1"
	)->GetTransform( );

	auto p2 = m_world->CreateEntityFromArchetype(
		WORLD_ARCHETYPE_PATH "Player2.uatype", "Player1"
	)->GetTransform( );

	p1->SetWorldPosition( s1->GetTransform( )->GetWorldPosition( ) );
	p1->SetWorldRotation( s1->GetTransform( )->GetWorldRotation( ) );

	p2->SetWorldPosition( s2->GetTransform( )->GetWorldPosition( ) );
	p2->SetWorldRotation( s2->GetTransform( )->GetWorldRotation( ) );

	// Turn off the player's camera's
	p1->GetOwner( )->GetComponentInChildren<Camera>( )->SetActive( false );
	p2->GetOwner( )->GetComponentInChildren<Camera>( )->SetActive( false );

	// Turn on the elevator camera
	elevator->GetComponentInChildren<Camera>( )->SetActive( true );

	// Tell the camera animator to animate
	auto animator = elevator->GetComponentInChildren<CameraAnimator>( );
	animator->Reset( );
	animator->Play( );
}

void StartRoomDirectorSystem::OnRemove(void)
{
    
}
