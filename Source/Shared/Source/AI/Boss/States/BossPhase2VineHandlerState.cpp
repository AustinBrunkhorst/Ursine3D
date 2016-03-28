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

BossPhase2VineHandlerState::BossPhase2VineHandlerState(void)
    : BossAIState( "Boss Phase2 Vine Handler" ) { }

void BossPhase2VineHandlerState::Update(BossAIStateMachine *machine)
{
    // get the vines
    auto vines = machine->GetBoss( )->GetVines( );

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
        homeVines[ 0 ]->PursueTarget( );
}
