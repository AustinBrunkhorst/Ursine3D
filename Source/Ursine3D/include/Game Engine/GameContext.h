#pragma once

#include "EventDispatcher.h"
#include "GameContextEvent.h"

namespace ursine
{
    class GameContext : public EventDispatcher<GameContextEventType>
    {
    public:
        virtual ~GameContext(void) { }

        virtual void ExitGame(void) = 0;
    };
}