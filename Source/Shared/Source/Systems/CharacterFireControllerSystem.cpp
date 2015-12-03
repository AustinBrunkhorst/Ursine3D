#include "Precompiled.h"

#include "CharacterFireControllerSystem.h"
#include "CharacterFireControllerComponent.h"

#include "RigidbodyComponent.h"

#include "PlayerInputComponent.h"

#include <GamepadManager.h>
#include <MouseManager.h>
#include <KeyboardManager.h>
#include <UrsineMath.h>
#include <Core/Physics/Interop/Raycasting.h>
#include <PhysicsSystem.h>
#include <SystemManager.h>
#include <Components/HealthComponent.h>


using namespace ursine;
using namespace ursine::ecs;

ENTITY_SYSTEM_DEFINITION( CharacterFireControllerSystem );

CharacterFireControllerSystem::CharacterFireControllerSystem( ursine::ecs::World *world )
    : FilterSystem( world, Filter( ).All<CharacterFireController, PlayerInput>( ) )
{

}

void CharacterFireControllerSystem::Process( Entity *entity )
{
    auto *input = entity->GetComponent<PlayerInput>( );
    auto *fireController = entity->GetComponent<CharacterFireController>( );
    auto *entityTransform = entity->GetTransform( );

    // check our states
    if ( input->ResetTrigger( ) )
    {
        fireController->SetFireState( true );
    }

    // update fire timer
    fireController->DecrementFireTimer( 0.016 );

    // firing a ray
    if ( input->Fire( ) && fireController->CanFire() )
    {
        printf( "BANG!\n\n" );
        fireController->Fire( );

        //// get the camera, then get the arm connected to the camera
        //auto children = entity->GetChildren( );
        //auto camera = m_world->GetEntity( (*children)[ 0 ] );
        //auto camChildren = camera->GetChildren( );
        //auto armGun = m_world->GetEntity( (*camChildren)[ 0 ] );

        m_rng.SetMin( -fireController->GetAccuracy( ) );
        m_rng.SetMax( fireController->GetAccuracy( ) );

        float randomPitch = m_rng.GetValue( );
        float randomYaw = m_rng.GetValue( );

        SVec3 spray = entityTransform->GetUp( ) * randomPitch + entityTransform->GetRight( ) * randomYaw;

        //auto *gunTransform = armGun->GetTransform( );
        physics::RaycastInput rayInput = physics::RaycastInput(
            entityTransform->GetWorldPosition( ) + SVec3( 0, 20, 0 ) + entityTransform->GetForward( ) * fireController->GetFireOffset(),
            entityTransform->GetWorldPosition( ) + SVec3( 0, 20, 0 ) + entityTransform->GetForward( ) * fireController->GetFireRange() + spray
        );

        physics::RaycastOutput rayOutput;

        m_world->GetEntitySystem( PhysicsSystem )->Raycast( 
            rayInput, 
            rayOutput, 
            physics::RAYCAST_ALL_HITS, 
            true, 
            0.1, 
            true, 
            fireController->GetShotStartColor(),
            fireController->GetShotEndColor( )
        );

        for ( auto &x : rayOutput.entity )
        {
            auto *hitObj = m_world->GetEntity( x );
            auto *health = hitObj->GetComponent<Health>( );
            if ( health != nullptr )
            {
                health->DealDamage( fireController->GetDamage() );
            }
        }
    }
}
