#pragma once

#include "EventDispatcher.h"
#include "GameContextEvent.h"

namespace ursine
{
    class GameContext : public EventDispatcher<GameContextEventType>
    {
    public:
        GameContext(void *defaultSender)
            : EventDispatcher( defaultSender ) { }

        virtual ~GameContext(void) { }

        virtual void ExitGame(void) = 0;
    };
}