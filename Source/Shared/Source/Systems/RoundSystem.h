/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** RoundSystem.h
**
** Author:
** - Joshua Shlemmer - joshua.shlemmer@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include <EntitySystem.h>

enum RoundSystemEventType
{
    // args tell what team died
    ROUND_OVER = 0x160,
    // args tell what round it is
    ROUND_START,
    // args say what team won
    MATCH_OVER
};

class RoundSystem 
    : public ursine::ecs::EntitySystem
    , public ursine::EventDispatcher<RoundSystemEventType>
{
    ENTITY_SYSTEM;

public:

    struct RoundEventArgs : ursine::EventArgs
    {
        int team;

        RoundEventArgs(int team)
            : team(team) { }
    };

    RoundSystem(ursine::ecs::World *world);

    int GetCurrentRound(void) const;

    int GetMaxRoundCount(void) const;
    void SetMaxRoundCount(int round);

private:
    void OnInitialize(void) override;
    void OnRemove(void) override;
    
    void onRoundOver(EVENT_HANDLER(ursine::ecs:::World));

    int m_round;
    int m_maxRound;

} Meta(Enable, DisableEntitySystemAutoAdd);
