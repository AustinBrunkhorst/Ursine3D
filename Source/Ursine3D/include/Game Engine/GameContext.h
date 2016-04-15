#pragma once

#include "EventDispatcher.h"
#include "GameContextEvent.h"

#include "GameManager.h"

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

        virtual void SetVolume(float volume, const std::string &outputType) const = 0;

        virtual void ExitGame(void) = 0;

        void SetManager(GameManager *manager);

    protected:
        GameManager *m_manager;
    } Meta(Register);
}
