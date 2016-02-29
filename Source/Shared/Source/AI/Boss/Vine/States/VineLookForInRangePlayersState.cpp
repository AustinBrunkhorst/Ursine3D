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

VineLookForInRanagePlayersState::VineLookForInRanagePlayersState(void)
    : BossVineState( "Vine Look For In Range Players" )
    , m_inRange( false )
{
}

void VineLookForInRanagePlayersState::Update(BossVineStateMachine *machine)
{
    auto ai = machine->GetAI( );
    auto range = ai->GetWhipRange( );
    auto world = ai->GetOwner( )->GetWorld( );
    auto players = world->GetEntitiesFromFilter( Filter( ).All<PlayerID>( ) );
    auto closestDir = std::numeric_limits<float>( ).max( );

    Transform *closestTrans = nullptr;

    for (auto &playerID : players)
    {
        auto dir = playerID->GetTransform( )->GetWorldPosition( ) - 
                   ai->GetOwner( )->GetTransform( )->GetWorldPosition( );

        if (range < 1.0f)
        {
            auto len = dir.Length( );

            if (len <= range)
                m_inRange = true;

            if (len < closestDir)
            {
                closestTrans = playerID->GetTransform( );
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
                closestTrans = playerID->GetTransform( );
                closestDir = len;
            }
        }
   }

    if (ai->GetFaceClosestPlayer( ))
    {
        ai->GetOwner( )->GetTransform( )->LookAt(
            closestTrans->GetWorldPosition( )
        );
    }
}
