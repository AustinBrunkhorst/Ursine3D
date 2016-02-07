#include "Precompiled.h"

#include "StartRoomDirectorSystem.h"

#include <CameraComponent.h>
#include <Screen.h>
#include <AIMovementControllerComponent.h>

#include <KeyboardManager.h>

#include "ElevatorLiftMoverComponent.h"
#include "CameraAnimatorComponent.h"

using namespace ursine;
using namespace ecs;

ENTITY_SYSTEM_DEFINITION( StartRoomDirectorSystem );

StartRoomDirectorSystem::StartRoomDirectorSystem(ursine::ecs::World *world)
    : EntitySystem( world )
	, m_init( false )
{

}

void StartRoomDirectorSystem::onUpdate(EVENT_HANDLER(World))
{
    auto km = GetCoreSystem(KeyboardManager);

    if (km->IsDown(KeyboardKey::KEY_SPACE))
    {
        auto ais = m_world->GetEntitiesFromFilter( Filter( ).All<AIMovementController>( ) );

        for (auto ai : ais)
            ai->GetComponent<AIMovementController>( )->SetEnable( false );
    }

	if (!m_init)
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
		auto elevatorCam = elevator->GetComponentInChildren<Camera>( );
		elevatorCam->SetActive( true );
		elevatorCam->SetRenderLayer( 1 );

		// Tell the camera animator to animate
		auto animator = elevator->GetComponentInChildren<CameraAnimator>( );
		animator->Reset( );
		animator->Play( );

		m_timers.Create(TimeSpan::FromSeconds(18.5f)).Completed([=] {
			auto p1Cam = p1->GetOwner( )->GetComponentInChildren<Camera>( );
			auto p2Cam = p2->GetOwner( )->GetComponentInChildren<Camera>( );

			p1Cam->SetActive( true );
			p2Cam->SetActive( true );

			p1Cam->SetViewportSize( Vec2( 1.0f, 0.5f ) );
			p2Cam->SetViewportSize( Vec2( 1.0f, 0.5f ) );

			p1Cam->SetRenderLayer( 2 );
			p2Cam->SetRenderLayer( 2 );

			p1Cam->GetOwner( )->GetTweens( ).Create( ).BeginGroup( ).Setter(
				p1Cam, &Camera::SetViewportPosition, Vec2(0, -0.5f), Vec2::Zero( ), TimeSpan::FromSeconds( 1.0f )
			).EndGroup( )
			.Call( [=] {

				auto screen = m_world->GetOwner( );

				if (screen)
					screen->MessageUI( "RaidStart", Json( ) );

				auto ais = m_world->GetEntitiesFromFilter( Filter( ).All<AIMovementController>( ) );

				for (auto ai : ais)
					ai->GetComponent<AIMovementController>( )->SetEnable( true );

				elevatorCam->GetOwner( )->Delete( );
			} );

			p2Cam->GetOwner( )->GetTweens( ).Create( ).Setter(
				p2Cam, &Camera::SetViewportPosition, Vec2(0, 1.0f), Vec2(0, 0.5f), TimeSpan::FromSeconds( 1.0f )
			);
		});

		m_init = true;
	}
}

void StartRoomDirectorSystem::OnAfterLoad(void)
{
    m_world->Listener( this )
        .On( WORLD_UPDATE, &StartRoomDirectorSystem::onUpdate );
}

void StartRoomDirectorSystem::OnRemove(void)
{
    m_world->Listener( this )
        .Off( WORLD_UPDATE, &StartRoomDirectorSystem::onUpdate );
}
