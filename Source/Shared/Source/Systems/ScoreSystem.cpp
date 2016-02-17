/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ScoreSystem.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "ScoreSystem.h"

#include <SystemManager.h>
#include <WorldEvent.h>
#include <Components/TeamComponent.h>

using namespace ursine;


ENTITY_SYSTEM_DEFINITION( ScoreSystem );

ScoreSystem::ScoreSystem(ursine::ecs::World* world) 
    : EntitySystem(world)
    , m_player1Kills( 0 )
    , m_player2Kills( 0 )
    , m_player1Wins( 0 )
    , m_player2Wins( 0 )
    , m_currRound( 1 )
{
    return;
}

int ScoreSystem::GetKills(int player) const
{
    // just using this since if someone tries
    // for bad players score I don't mind wrong val
    return player == 1 ? m_player1Kills : m_player2Kills;
}

void ScoreSystem::AddKill(int player)
{
    if (player == 1)
    {
        ++m_player1Kills;
    }
    else if (player == 2)
    {
        ++m_player2Kills;
    }
}

int ScoreSystem::GetCurrentRound() const
{
    return m_currRound;
}

void ScoreSystem::OnInitialize(void)
{
}

void ScoreSystem::OnRemove()
{
}
