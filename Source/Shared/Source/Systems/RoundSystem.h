#pragma once

#include <EntitySystem.h>


enum RoundSystemEventType
{
    ROUND_OVER = 0x160,
    ROUND_START,
    MATCH_OVER
};

class RoundSystem : public ursine::ecs::EntitySystem
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

} Meta(Enable);
