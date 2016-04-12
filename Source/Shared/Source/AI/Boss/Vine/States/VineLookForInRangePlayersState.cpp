/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** VineLookForInRangePlayersState.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "VineLookForInRangePlayersState.h"
#include "VineAIStateMachine.h"
#include "VineAIComponent.h"

#include <PlayerIdComponent.h>

using namespace ursine;
using namespace ecs;

VineLookForInRangePlayersState::VineLookForInRangePlayersState(void)
    : VineAIState( "Vine Look For In Range Players" )
    , m_inRange( false )
    , m_inView( false )
{
}

void VineLookForInRangePlayersState::Enter(VineAIStateMachine *machine)
{
    // save the original direction of the ai
    auto ai = machine->GetAI( );
    auto aiTrans = ai->GetOwner( )->GetTransform( );

    m_originalForward = aiTrans->GetForward( );

    // animate the vine to be idle
    ai->GetAnimator( )->SetCurrentState( "Idle" );
}

void VineLookForInRangePlayersState::Update(VineAIStateMachine *machine)
{
    auto ai = machine->GetAI( );
    auto aiOwner = ai->GetOwner( );
    auto aiTrans=  aiOwner->GetTransform( );
    auto range = ai->GetWhipRange( );
    auto world = aiOwner->GetWorld( );
    auto players = world->GetEntitiesFromFilter( Filter( ).All<PlayerID>( ) );
    auto closestDir = std::numeric_limits<float>( ).max( );

    Transform *closestTrans = nullptr;

    // reset m_inRange
    m_inRange = false;

    for (auto &playerID : players)
    {
        if (playerID->GetComponent<Health>( )->GetHealth( ) <= 0.0f)
            continue;

        auto playerTrans = playerID->GetTransform( );
        auto dir = playerTrans->GetWorldPosition( ) - 
                   aiTrans->GetWorldPosition( );

        if (range < 1.0f)
        {
            auto len = dir.Length( );

            m_inRange = m_inRange || len <= range;

            if (len < closestDir)
            {
                closestTrans = playerTrans;
                closestDir = len;
            }
        }
        else
        {
            auto len = dir.LengthSquared( );

            m_inRange = m_inRange || len <= range * range;

            if (len < closestDir)
            {
                closestTrans = playerTrans;
                closestDir = len;
            }
        }
    }

    if (!closestTrans)
        closestTrans = players[ rand( ) % players.size( ) ]->GetTransform( );

    ai->SetTarget( closestTrans->GetOwner( ) );

    auto aiPos = aiTrans->GetWorldPosition( );

    if (ai->GetFaceClosestPlayer( ) && m_inRange)
    {
        auto lookAtPosition = closestTrans->GetWorldPosition( );
        
        lookAtPosition.Y( ) = aiPos.Y( );

        aiTrans->LookAt( lookAtPosition, ai->GetWhipTurnSpeed( ) );

        // Check to see if we're in range of the look at position
        auto viewAngle = ai->GetWhipAngle( ) * 0.5f;
        auto currentAngle = math::RadiansToDegrees( acos( aiTrans->GetForward( ).Dot( 
            SVec3::Normalize( lookAtPosition - aiPos )
        ) ) );

        m_inView = viewAngle >= abs( currentAngle );
    }
    else
    {
        aiTrans->LookAt( aiPos + m_originalForward, ai->GetWhipTurnSpeed( ) );
        m_inView = false;
    }

    machine->SetBool( VineAIStateMachine::InRange, m_inRange );
    machine->SetBool( VineAIStateMachine::InView, m_inView );
}
