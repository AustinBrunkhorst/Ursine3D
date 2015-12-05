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
#include <Game Engine/Scene/Component/Native Components/AnimatorComponent.h>


using namespace ursine;
using namespace ursine::ecs;

ENTITY_SYSTEM_DEFINITION( CharacterFireControllerSystem );

CharacterFireControllerSystem::CharacterFireControllerSystem( ursine::ecs::World *world )
    : FilterSystem( world, Filter( ).All<CharacterFireController, PlayerInput>( ) )
{

}

void CharacterFireControllerSystem::Process( Entity *entity )
{
    auto *fireController = entity->GetComponent<CharacterFireController>( );
    auto *entityTransform = entity->GetTransform( );

    // update fire timer
    URSINE_TODO("Get acutal delta time for firing timer reduction");
    fireController->DecrementFireTimer( 0.016 );

    // find the child hotspot for firing
    auto childrenVector = entity->GetChildren( );

    Entity *hotspot = nullptr;
    Entity *armGun = nullptr;

    for ( auto &x : *childrenVector )
    {
        auto *currentChild = m_world->GetEntity( x );

        if ( currentChild->GetName( ) == "FiringHotspot" )
        {
            hotspot = currentChild;
        }
        if ( currentChild->GetName( ) == "FPSArm" )
        {
            armGun = currentChild;
        }
    }

    // firing a ray
    // if we had a hotspot, firing controller is ready to fire (firing timer is down), AND we received input from
    // command (IsFiring), fire
    if ( hotspot != nullptr &&  fireController->CanFire() )
    {
        // animation stuff
        auto *armAnimator = armGun->GetComponent<Animator>( );
        float animationScalar = armAnimator->GetTimeScalar( );

        // reset firing sequence
        armAnimator->SetAnimationTimePosition( 0.1 );
        armAnimator->SetTimeScalar( 1.0f / fireController->GetFireRate( ) );

        // actually firing
        fireController->Fire( );

        //// get the camera, then get the arm connected to the camera
        //auto children = entity->GetChildren( );
        //auto camera = m_world->GetEntity( (*children)[ 0 ] );
        //auto camChildren = camera->GetChildren( );
        //auto armGun = m_world->GetEntity( (*camChildren)[ 0 ] );

        // calculate offset for firing accuracy
        m_rng.SetMin( -fireController->GetAccuracy( ) );
        m_rng.SetMax( fireController->GetAccuracy( ) );

        float randomPitch = m_rng.GetValue( );
        float randomYaw = m_rng.GetValue( );

        // calculate spray, as well as the offset from firing point
        SVec3 spray = entityTransform->GetUp( ) * randomPitch + entityTransform->GetRight( ) * randomYaw;
        SVec3 offset = entityTransform->GetWorldRotation( ) * fireController->GetFireOffset( );

        auto *childTransform = hotspot->GetTransform( );

        // setting up the input to the raycast system
        physics::RaycastInput rayInput = physics::RaycastInput(
            childTransform->GetWorldPosition( ) + offset,
            childTransform->GetWorldPosition( ) + entityTransform->GetForward( ) * fireController->GetFireRange() + spray
        );

        physics::RaycastOutput rayOutput;

        // raycast
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

        // search through hit objects, looking for stuff we can damage
        for ( auto &x : rayOutput.entity )
        {
            auto *hitObj = m_world->GetEntityUnique( x );
            auto *health = hitObj->GetComponent<Health>( );
            if ( health != nullptr )
            {
                health->DealDamage( fireController->GetDamage() );
            }
        }
    }
}
