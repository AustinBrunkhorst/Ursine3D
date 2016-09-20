/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** PlayerDeathCameraAnimatorComponent.cpp
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "PlayerDeathCameraAnimatorComponent.h"

#include "CharacterControllerComponent.h"
#include "HealthComponent.h"
#include "PlayerIDComponent.h"

#include <SweptControllerComponent.h>
#include <LocalTweenManager.h>

NATIVE_COMPONENT_DEFINITION( PlayerDeathCameraAnimator );

using namespace ursine;
using namespace ecs;

PlayerDeathCameraAnimator::PlayerDeathCameraAnimator(void)
    : BaseComponent( ) { }

void PlayerDeathCameraAnimator::AnimateDeath(void)
{
    // Turn on camera
    GetOwner( )->GetComponent<Camera>( )->SetActive( true );

    // Start the Out animation
    GetOwner( )->GetComponent<EntityAnimator>( )->Play( "Out" );

    // Animate the player's viewport
    auto root = GetOwner( )->GetRoot( );
    auto camEntity = root->GetChildByName( fpsCam );

    UAssert( camEntity, "This needs to be set" );

    auto cam = camEntity->GetComponent<Camera>( );
    auto &tweens = GetOwner( )->GetTweens( );

    tweens.Create( ).BeginGroup( ).Setter(
        cam, &Camera::SetViewportSize, Vec2( 1.0f, 0.5f ), Vec2( 0.0f, 0.0f ), 
        TimeSpan::FromSeconds( outTime ), GetFunction( outEase )
    ).EndGroup( ).Call( [=] { cam->SetActive( false ); } );

    // determine what camera we're tweening
    if (cam->GetViewportPosition( ).Y( ) == 0.0f)
    {
        // top viewport
        tweens.Create( ).Setter(
            cam, &Camera::SetViewportPosition, Vec2( 0.0f, 0.0f ), Vec2( 0.5f, 0.25f ), 
            TimeSpan::FromSeconds( outTime ), GetFunction( outEase )
        );
    }
    else
    {
        // bottom viewport
        tweens.Create( ).Setter(
            cam, &Camera::SetViewportPosition, Vec2( 0.0f, 0.5f ), Vec2( 0.5f, 0.75f ), 
            TimeSpan::FromSeconds( outTime ), GetFunction( outEase )
        );
    }
}

void PlayerDeathCameraAnimator::AnimateRevival(void)
{
    // Start the In animation
    auto animator = GetOwner( )->GetComponent<EntityAnimator>( );

    animator->Play( "In" );

    // On finish, unlock the charcter cotnroler, turn on swept controller, give health
    animator->Listener( this )
        .On( EntityAnimatorEvent::FinishedAnimating, &PlayerDeathCameraAnimator::onAnimationFinished );

    // Animate the player's viewport
    auto root = GetOwner( )->GetRoot( );
    auto camEntity = root->GetChildByName( fpsCam );

    UAssert( camEntity, "This needs to be set" );

    auto cam = camEntity->GetComponent<Camera>( );
    cam->SetActive( true );

    auto &tweens = GetOwner( )->GetTweens( );

    tweens.Create( ).BeginGroup( ).Setter(
        cam, &Camera::SetViewportSize, Vec2( 0.0f, 0.0f ), Vec2( 1.0f, 0.5f ), 
        TimeSpan::FromSeconds( inTime ), GetFunction( inEase )
    ).EndGroup( );

    // determine what camera we're tweening
    if (math::IsEqual( cam->GetViewportPosition( ).Y( ), 0.25f ))
    {
        // top viewport
        tweens.Create( ).Setter(
            cam, &Camera::SetViewportPosition, Vec2( 0.5f, 0.25f ), Vec2( 0.0f, 0.0f ),
            TimeSpan::FromSeconds( inTime ), GetFunction( inEase )
        );
    }
    else
    {
        // bottom viewport
        tweens.Create( ).Setter(
            cam, &Camera::SetViewportPosition, Vec2( 0.5f, 0.75f ), Vec2( 0.0f, 0.5f ),
            TimeSpan::FromSeconds( inTime ), GetFunction( inEase )
        );
    }
}

void PlayerDeathCameraAnimator::onAnimationFinished(EVENT_HANDLER(EntityAnimator))
{
    EVENT_SENDER(EntityAnimator, sender);

    sender->Listener( this )
        .Off( EntityAnimatorEvent::FinishedAnimating, &PlayerDeathCameraAnimator::onAnimationFinished );

    auto root = GetOwner( )->GetRoot( );

    // unlock character controller
    auto cc = root->GetComponent<CharacterController>( );

    cc->SetLockLooking( false );
    cc->SetLockMovement( false );

    // turn on swept cotnroller
    root->GetComponent<SweptController>( )->SetEnableImpulses( true );

    // give back health
    auto health = root->GetComponent<Health>( );

    health->SetHealth( health->GetMaxHealth( ) );
    health->SetShieldHealth( health->GetMaxShieldHealth( ) );

    // Turn off camera
    GetOwner( )->GetComponent<Camera>( )->SetActive( false );
}
