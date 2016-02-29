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
#include "BossVineStateMachine.h"
#include "BossVineAIComponent.h"

#include <PlayerIdComponent.h>

using namespace ursine;
using namespace ecs;

VineLookForInRangePlayersState::VineLookForInRangePlayersState(void)
    : BossVineState( "Vine Look For In Range Players" )
    , m_inRange( false )
{
}

void VineLookForInRangePlayersState::Enter(BossVineStateMachine *machine)
{
    // save the original direction of the ai
    auto ai = machine->GetAI( );
    auto aiTrans = ai->GetOwner( )->GetTransform( );

    m_originalForward = aiTrans->GetForward( );
}

void VineLookForInRangePlayersState::Update(BossVineStateMachine *machine)
{
    auto ai = machine->GetAI( );
    auto aiOwner = ai->GetOwner( );
    auto aiTrans=  aiOwner->GetTransform( );
    auto range = ai->GetWhipRange( );
    auto world = aiOwner->GetWorld( );
    auto players = world->GetEntitiesFromFilter( Filter( ).All<PlayerID>( ) );
    auto closestDir = std::numeric_limits<float>( ).max( );

    Transform *closestTrans = nullptr;

    for (auto &playerID : players)
    {
        auto playerTrans = playerID->GetTransform( );
        auto dir = playerTrans->GetWorldPosition( ) - 
                   aiTrans->GetWorldPosition( );

        if (range < 1.0f)
        {
            auto len = dir.Length( );

            if (len <= range)
                m_inRange = true;

            if (len < closestDir)
            {
                closestTrans = playerTrans;
                closestDir = len;
            }
        }
        else
        {
            auto len = dir.LengthSquared( );

            if (dir.LengthSquared( ) <= range * range)
                m_inRange = true;

            if (len < closestDir)
            {
                closestTrans = playerTrans;
                closestDir = len;
            }
        }
    }

    if (ai->GetFaceClosestPlayer( ) && m_inRange)
    {
        auto aiPos = aiTrans->GetWorldPosition( );
        auto lookAtPosition = closestTrans->GetWorldPosition( );
        
        lookAtPosition.Y( ) = aiPos.Y( );

        aiTrans->LookAt( lookAtPosition, ai->GetTurnSpeed( ) );
    }
    else
    {
        aiTrans->LookAt( m_originalForward, ai->GetTurnSpeed( ) );
    }
}
