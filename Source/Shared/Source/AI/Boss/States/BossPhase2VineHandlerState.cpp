/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** BossPhase2VineHandlerState.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "BossPhase2VineHandlerState.h"

#include "BossAIStateMachine.h"
#include "BossAIComponent.h"
#include "VineAIComponent.h"

#include <TimeSpan.h>

#include <functional> 

using namespace ursine;

BossPhase2VineHandlerState::BossPhase2VineHandlerState(void)
    : BossAIState( "Boss Phase2 Vine Handler" ) { }

void BossPhase2VineHandlerState::Enter(BossAIStateMachine *machine)
{
    // set the callback for when the health thresholds are reached
    auto boss = machine->GetBoss( );

    boss->SetVineHealthThresholdCallback( 
        std::bind( &BossPhase2VineHandlerState::onHealthThresholdReached, this, std::placeholders::_1 ) 
    );
}

void BossPhase2VineHandlerState::Update(BossAIStateMachine *machine)
{
    // get the vines
    auto &vines = machine->GetBoss( )->GetVines( );

    // check to see how many are away from home
    int numAwayFromHome = 0;
    std::vector<VineAI *> homeVines;

    for (auto &vine : vines)
    {
        auto vineAI = vine->GetComponent<VineAI>( );

        if (!vineAI->IsHome( ))
            ++numAwayFromHome;
        else
            homeVines.push_back( vineAI );
    }

    // if there are less than two away from home, send one out
    if (numAwayFromHome < 2 && homeVines.size( ) > 0)
    {
        // Find the vine that has been sitting at home the longest
        TimeSpan latestTime = homeVines[ 0 ]->GetTimeOfLastPursue( );
        int index = 0;

        for (int i = 1; i < homeVines.size( ); ++i)
        {
            auto &time = homeVines[ i ]->GetTimeOfLastPursue( );

            if (time < latestTime)
            {
                index = i;
                latestTime = time;
            }
        }

        homeVines[ index ]->PursueTarget( );
    }
}

void BossPhase2VineHandlerState::Exit(BossAIStateMachine *machine)
{
    machine->GetBoss( )->SetVineHealthThresholdCallback( nullptr );
}

#include <iostream>
void BossPhase2VineHandlerState::onHealthThresholdReached(VineAI *vine)
{
    // If the vine has taken damage to make it's health threshold go to a certain point, 
    // tell it to go back home or pursue a target
    if (vine->IsHome( ))
    {
        vine->PursueTarget( );
        std::cout << "Pursue Enemy" << std::endl;
    }
    else
    {
        vine->GoToHomeLocation( );
        std::cout << "Go Home" << std::endl;
    }
}
