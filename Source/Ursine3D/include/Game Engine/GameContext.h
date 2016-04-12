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

        virtual bool GetWindowFullScreen(void) const = 0;
        virtual void SetWindowFullScreen(bool fullScreen) = 0;

        virtual bool IsMusicMuted(void) const = 0;
        virtual void MuteMusic(bool muted) = 0;

        virtual bool IsAudioMuted(void) const = 0;
        virtual void MuteAudio(bool muted) = 0;

        virtual void ExitGame(void) = 0;
    };
}
