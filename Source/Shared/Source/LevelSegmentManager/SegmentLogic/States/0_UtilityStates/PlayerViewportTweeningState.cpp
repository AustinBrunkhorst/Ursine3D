/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** PlayerViewportTweeningState.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "PlayerViewportTweeningState.h"

#include "SegmentLogicStateIncludes.h"
#include <CameraComponent.h>

using namespace ursine;
using namespace ecs;

PlayerViewportTweeningState::PlayerViewportTweeningState(ViewportTweenType type, bool blocking, bool camerasActive)
    : SegmentLogicState( "Player Viewport Tween" )
    , m_blocking( blocking )
    , m_camerasActive( camerasActive )
{
    m_type = type;
}

void PlayerViewportTweeningState::Enter(SegmentLogicStateMachine *machine)
{
    static const float tweenTime = 1.0f;
    static const ease::Function easeFunc = ease::Linear;
    auto segmentManager = machine->GetSegmentManager( );

    UAssert( segmentManager->GetPlayer1( ) && segmentManager->GetPlayer2( ), 
            "Error: This state requires the level to have player's present."
            "  Use the \"SpawnPlayersState\" state and have player spawn points in the world." );

    auto &tweens = segmentManager->GetOwner( )->GetTweens( );
    
    auto p1 = segmentManager->GetPlayer1( );
    auto p2 = segmentManager->GetPlayer2( );

    auto c1 = p1->GetComponentInChildren<Camera>( );
    auto c2 = p2->GetComponentInChildren<Camera>( );

    if (m_camerasActive)
    {
        c1->SetActive( true );
        c2->SetActive( true );
    }

    switch (m_type)
    {
        case ViewportTweenType::SplitOutUpDown:
        {
            tweens.Create( ).BeginGroup( ).Setter(
                c1, &Camera::SetViewportPosition, Vec2::Zero( ), Vec2( 0.0f, -0.5f ), TimeSpan::FromSeconds( tweenTime ), easeFunc
            ).EndGroup( ).Call( [=] { m_blocking = false; } );

            tweens.Create().Setter(
                c2, &Camera::SetViewportPosition, Vec2( 0.0f, 0.5f ), Vec2( 0.0f, 1.0f ), TimeSpan::FromSeconds( tweenTime ), easeFunc
            );

            break;
        }
        case ViewportTweenType::SplitInUpDown:
        {
            tweens.Create( ).BeginGroup( ).Setter(
                c1, &Camera::SetViewportPosition, Vec2( 0.0f, -0.5f ),  Vec2::Zero( ), TimeSpan::FromSeconds( tweenTime ), easeFunc
            ).EndGroup( ).Call( [=] { m_blocking = false; } );

            tweens.Create().Setter(
                c2, &Camera::SetViewportPosition, Vec2( 0.0f, 1.0f ), Vec2( 0.0f, 0.5f ), TimeSpan::FromSeconds( tweenTime ), easeFunc
            );

            break;
        }
        case ViewportTweenType::SplitOutRightLeft:
        {
            tweens.Create( ).BeginGroup( ).Setter(
                c1, &Camera::SetViewportPosition, Vec2::Zero( ), Vec2( 1.0f, 0.0f ), TimeSpan::FromSeconds( tweenTime ), easeFunc
            ).EndGroup( ).Call( [=] { m_blocking = false; } );

            tweens.Create().Setter(
                c2, &Camera::SetViewportPosition, Vec2( 0.0f, 0.5f ), Vec2( -1.0f, 0.5f ), TimeSpan::FromSeconds( tweenTime ), easeFunc
            );

            break;
        }
        case ViewportTweenType::SplitInRightLeft:
        {
            tweens.Create( ).BeginGroup( ).Setter(
                c1, &Camera::SetViewportPosition, Vec2( 1.0f, 0.0f ), Vec2::Zero( ), TimeSpan::FromSeconds( tweenTime ), easeFunc
            ).EndGroup( ).Call( [=] { m_blocking = false; } );

            tweens.Create().Setter(
                c2, &Camera::SetViewportPosition, Vec2( -1.0f, 0.5f ), Vec2( 0.0f, 0.5f ), TimeSpan::FromSeconds( tweenTime ), easeFunc
            );

            break;
        }
        case ViewportTweenType::SplitOutLeftRight:
        {
            tweens.Create( ).BeginGroup( ).Setter(
                c1, &Camera::SetViewportPosition, Vec2::Zero( ), Vec2( -1.0f, 0.0f ), TimeSpan::FromSeconds( tweenTime ), easeFunc
            ).EndGroup( ).Call( [=] { m_blocking = false; } );

            tweens.Create().Setter(
                c2, &Camera::SetViewportPosition, Vec2( 0.0f, 0.5f ), Vec2( 1.0f, 0.5f ), TimeSpan::FromSeconds( tweenTime ), easeFunc
            );

            break;
        }
        case ViewportTweenType::SplitInLeftRight:
        {
            tweens.Create( ).BeginGroup( ).Setter(
                c1, &Camera::SetViewportPosition, Vec2( -1.0f, 0.0f ), Vec2::Zero( ), TimeSpan::FromSeconds( tweenTime ), easeFunc
            ).EndGroup( ).Call( [=] { m_blocking = false; } );

            tweens.Create().Setter(
                c2, &Camera::SetViewportPosition, Vec2( 1.0f, 0.5f ), Vec2( 0.0f, 0.5f ), TimeSpan::FromSeconds( tweenTime ), easeFunc
            );

            break;
        }
        case ViewportTweenType::OutUp:
        {
            tweens.Create( ).BeginGroup( ).Setter(
                c1, &Camera::SetViewportPosition, Vec2::Zero( ), Vec2( 0.0f, -1.0f ), TimeSpan::FromSeconds( tweenTime ), easeFunc
            ).EndGroup( ).Call( [=] { m_blocking = false; } );

            tweens.Create( ).Setter(
                c2, &Camera::SetViewportPosition, Vec2( 0.0f, 0.5f ), Vec2( 0.0f, -0.5f ), TimeSpan::FromSeconds( tweenTime ), easeFunc
            );

            break;
        }
        case ViewportTweenType::OutDown:
        {
            tweens.Create( ).BeginGroup( ).Setter(
                c1, &Camera::SetViewportPosition, Vec2::Zero( ), Vec2( 0.0f, 1.0f ), TimeSpan::FromSeconds( tweenTime ), easeFunc
            ).EndGroup( ).Call( [=] { m_blocking = false; } );

            tweens.Create( ).Setter(
                c2, &Camera::SetViewportPosition, Vec2( 0.0f, 0.5f ), Vec2( 0.0f, 1.5f ), TimeSpan::FromSeconds( tweenTime ), easeFunc
            );

            break;
        }
        case ViewportTweenType::InUp:
        {
            tweens.Create( ).BeginGroup( ).Setter(
                c1, &Camera::SetViewportPosition, Vec2( 0.0f, -1.0f ), Vec2::Zero( ), TimeSpan::FromSeconds( tweenTime ), easeFunc
            ).EndGroup( ).Call( [=] { m_blocking = false; } );

            tweens.Create( ).Setter(
                c2, &Camera::SetViewportPosition, Vec2( 0.0f, -0.5f ), Vec2( 0.0f, 0.5f ), TimeSpan::FromSeconds( tweenTime ), easeFunc
            );

            break;
        }
        case ViewportTweenType::InDown:
        {
            tweens.Create( ).BeginGroup( ).Setter(
                c1, &Camera::SetViewportPosition, Vec2( 0.0f, 1.0f ), Vec2::Zero( ), TimeSpan::FromSeconds( tweenTime ), easeFunc
            ).EndGroup( ).Call( [=] { m_blocking = false; } );

            tweens.Create( ).Setter(
                c2, &Camera::SetViewportPosition, Vec2( 0.0f, 1.5f ), Vec2( 0.0f, 0.5f ), TimeSpan::FromSeconds( tweenTime ), easeFunc
            );

            break;
        }
        case ViewportTweenType::OutRight:
        {
            tweens.Create( ).BeginGroup( ).Setter(
                c1, &Camera::SetViewportPosition, Vec2::Zero( ), Vec2( 1.0f, 0.0f ), TimeSpan::FromSeconds( tweenTime ), easeFunc
            ).EndGroup( ).Call( [=] { m_blocking = false; } );

            tweens.Create( ).Setter(
                c2, &Camera::SetViewportPosition, Vec2( 0.0f, 0.5f ), Vec2( 1.0f, 0.5f ), TimeSpan::FromSeconds( tweenTime ), easeFunc
            );

            break;
        }
        case ViewportTweenType::OutLeft:
        {
            tweens.Create( ).BeginGroup( ).Setter(
                c1, &Camera::SetViewportPosition, Vec2::Zero( ), Vec2( -1.0f, 0.0f ), TimeSpan::FromSeconds( tweenTime ), easeFunc
            ).EndGroup( ).Call( [=] { m_blocking = false; } );

            tweens.Create( ).Setter(
                c2, &Camera::SetViewportPosition, Vec2( 0.0f, 0.5f ), Vec2( -1.0f, 0.5f ), TimeSpan::FromSeconds( tweenTime ), easeFunc
            );

            break;
        }
        case ViewportTweenType::InRight:
        {
            tweens.Create( ).BeginGroup( ).Setter(
                c1, &Camera::SetViewportPosition, Vec2( 1.0f, 0.0f ), Vec2::Zero( ), TimeSpan::FromSeconds( tweenTime ), easeFunc
            ).EndGroup( ).Call( [=] { m_blocking = false; } );

            tweens.Create( ).Setter(
                c2, &Camera::SetViewportPosition, Vec2( 1.0f, 0.5f ), Vec2( 0.0f, 0.5f ), TimeSpan::FromSeconds( tweenTime ), easeFunc
            );

            break;
        }
        case ViewportTweenType::InLeft:
        {
            tweens.Create( ).BeginGroup( ).Setter(
                c1, &Camera::SetViewportPosition, Vec2( -1.0f, 0.0f ), Vec2::Zero( ), TimeSpan::FromSeconds( tweenTime ), easeFunc
            ).EndGroup( ).Call( [=] { m_blocking = false; } );

            tweens.Create( ).Setter(
                c2, &Camera::SetViewportPosition, Vec2( -1.0f, 0.5f ), Vec2( 0.0f, 0.5f ), TimeSpan::FromSeconds( tweenTime ), easeFunc
            );

            break;
        }
        case ViewportTweenType::SqueezeInHorizontal:
        {
            tweens.Create( ).BeginGroup( ).Setter(
                c1, &Camera::SetViewportSize, Vec2( 1.0f, 0.5f ), Vec2( 0.0f, 0.5f ), TimeSpan::FromSeconds( tweenTime ), easeFunc
            ).EndGroup( ).Call( [=] { m_blocking = false; } );

            tweens.Create().Setter(
                c2, &Camera::SetViewportSize, Vec2( 1.0f, 0.5f ), Vec2( 0.0f, 0.5f ), TimeSpan::FromSeconds( tweenTime ), easeFunc
            );

            tweens.Create( ).Setter(
                c1, &Camera::SetViewportPosition, Vec2::Zero( ), Vec2( 0.5f, 0.0f ), TimeSpan::FromSeconds( tweenTime ), easeFunc
            );

            tweens.Create().Setter(
                c2, &Camera::SetViewportPosition, Vec2( 0.0f, 0.5f ), Vec2( 0.5f, 0.5f ), TimeSpan::FromSeconds( tweenTime ), easeFunc
            );

            break;
        }
        case ViewportTweenType::SqueezeOutHorizontal:
        {
            tweens.Create( ).BeginGroup( ).Setter(
                c1, &Camera::SetViewportSize, Vec2( 0.0f, 0.5f ), Vec2( 1.0f, 0.5f ), TimeSpan::FromSeconds( tweenTime ), easeFunc
            ).EndGroup( ).Call( [=] { m_blocking = false; } );

            tweens.Create().Setter(
                c2, &Camera::SetViewportSize, Vec2( 0.0f, 0.5f ), Vec2( 1.0f, 0.5f ), TimeSpan::FromSeconds( tweenTime ), easeFunc
            );

            tweens.Create( ).Setter(
                c1, &Camera::SetViewportPosition, Vec2( 0.5f, 0.0f ), Vec2::Zero( ), TimeSpan::FromSeconds( tweenTime ), easeFunc
            );

            tweens.Create().Setter(
                c2, &Camera::SetViewportPosition, Vec2( 0.5f, 0.5f ), Vec2( 0.0f, 0.5f ), TimeSpan::FromSeconds( tweenTime ), easeFunc
            );

            break;
        }
        case ViewportTweenType::SqueezeInVertical:
        {
            tweens.Create( ).BeginGroup( ).Setter(
                c1, &Camera::SetViewportSize, Vec2( 1.0f, 0.5f ), Vec2( 1.0f, 0.0f ), TimeSpan::FromSeconds( tweenTime ), easeFunc
            ).EndGroup( ).Call( [=] { m_blocking = false; } );

            tweens.Create().Setter(
                c2, &Camera::SetViewportSize, Vec2( 1.0f, 0.5f ), Vec2( 1.0f, 0.0f ), TimeSpan::FromSeconds( tweenTime ), easeFunc
            );

            tweens.Create( ).Setter(
                c1, &Camera::SetViewportPosition, Vec2::Zero( ), Vec2( 0.0f, 0.5f ), TimeSpan::FromSeconds( tweenTime ), easeFunc
            );

            tweens.Create().Setter(
                c2, &Camera::SetViewportPosition, Vec2( 0.0f, 0.5f ), Vec2( 0.0f, 0.5f ), TimeSpan::FromSeconds( tweenTime ), easeFunc
            );

            break;
        }
        case ViewportTweenType::SqueezeOutVertical:
        {
            tweens.Create( ).BeginGroup( ).Setter(
                c1, &Camera::SetViewportSize, Vec2( 1.0f, 0.0f ), Vec2( 1.0f, 0.5f ), TimeSpan::FromSeconds( tweenTime ), easeFunc
            ).EndGroup( ).Call( [=] { m_blocking = false; } );

            tweens.Create().Setter(
                c2, &Camera::SetViewportSize, Vec2( 1.0f, 0.0f ), Vec2( 1.0f, 0.5f ), TimeSpan::FromSeconds( tweenTime ), easeFunc
            );

            tweens.Create( ).Setter(
                c1, &Camera::SetViewportPosition, Vec2( 0.0f, 0.5f ), Vec2::Zero( ), TimeSpan::FromSeconds( tweenTime ), easeFunc
            );

            tweens.Create().Setter(
                c2, &Camera::SetViewportPosition, Vec2( 0.0f, 0.5f ), Vec2( 0.0f, 0.5f ), TimeSpan::FromSeconds( tweenTime ), easeFunc
            );

            break;
        }
        case ViewportTweenType::SqueezeIn:
        {
            tweens.Create( ).BeginGroup( ).Setter(
                c1, &Camera::SetViewportSize, Vec2( 1.0f, 0.5f ), Vec2( 0.0f, 0.0f ), TimeSpan::FromSeconds( tweenTime ), easeFunc
            ).EndGroup( ).Call( [=] { m_blocking = false; } );

            tweens.Create( ).Setter(
                c2, &Camera::SetViewportSize, Vec2( 1.0f, 0.5f ), Vec2( 0.0f, 0.0f ), TimeSpan::FromSeconds( tweenTime ), easeFunc
            );

            tweens.Create( ).Setter(
                c1, &Camera::SetViewportPosition, Vec2::Zero( ), Vec2( 0.5f, 0.5f ), TimeSpan::FromSeconds( tweenTime ), easeFunc
            );

            tweens.Create().Setter(
                c2, &Camera::SetViewportPosition, Vec2( 0.0f, 0.5f ), Vec2( 0.5f, 0.5f ), TimeSpan::FromSeconds( tweenTime ), easeFunc
            );

            break;
        }
        case ViewportTweenType::SqueezeOut:
        {
            tweens.Create( ).BeginGroup( ).Setter(
                c1, &Camera::SetViewportSize, Vec2( 0.0f, 0.0f ), Vec2( 1.0f, 0.5f ), TimeSpan::FromSeconds( tweenTime ), easeFunc
            ).EndGroup( ).Call( [=] { m_blocking = false; } );

            tweens.Create( ).Setter(
                c2, &Camera::SetViewportSize, Vec2( 0.0f, 0.0f ), Vec2( 1.0f, 0.5f ), TimeSpan::FromSeconds( tweenTime ), easeFunc
            );

            tweens.Create( ).Setter(
                c1, &Camera::SetViewportPosition, Vec2( 0.5f, 0.5f ), Vec2::Zero( ), TimeSpan::FromSeconds( tweenTime ), easeFunc
            );

            tweens.Create().Setter(
                c2, &Camera::SetViewportPosition, Vec2( 0.5f, 0.5f ), Vec2( 0.0f, 0.5f ), TimeSpan::FromSeconds( tweenTime ), easeFunc
            );

            break;
        }
    };
}

void PlayerViewportTweeningState::Exit(SegmentLogicStateMachine *machine)
{
    auto segmentManager = machine->GetSegmentManager( );
    auto p1 = segmentManager->GetPlayer1( );
    auto p2 = segmentManager->GetPlayer2( );

    auto c1 = p1->GetComponentInChildren<Camera>( );
    auto c2 = p2->GetComponentInChildren<Camera>( );

    if (!m_camerasActive)
    {
        c1->SetActive( false );
        c2->SetActive( false );
    }
}
