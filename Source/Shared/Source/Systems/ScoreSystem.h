/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ScoreSystem.h
**
** Author:
** - Joshua Shlemmer - joshua.shlemmer@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include <EntitySystem.h>
#include "RoundSystem.h"

class ScoreSystem : public ursine::ecs::EntitySystem
{
    ENTITY_SYSTEM;

public:
    ScoreSystem(ursine::ecs::World *world);

    int GetKills(int player) const;
    void AddKill(int player);

    int GetCurrentRound(void) const;


private:

    void OnInitialize(void) override;
    void OnRemove(void) override;

    void onRoundOver(EVENT_HANDLER(RoundSystem));

    void onPlayerDied(EVENT_HANDLER(RoundSystem));

    int m_player1Kills;
    int m_player2Kills;
            
    int m_player1Wins;
    int m_player2Wins;
            
    int m_currRound;

} Meta(Enable);
