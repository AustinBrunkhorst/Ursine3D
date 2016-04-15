#pragma once

#include "EventDispatcher.h"
#include "GameContextEvent.h"

#include "GameManager.h"

namespace ursine
{
    class GameContext : public EventDispatcher<GameContextEventType>
    {
    public:
        GameContext(Scene *scene, void *defaultSender);

        virtual ~GameContext(void) { }

        virtual bool GetWindowFullScreen(void) const = 0;
        virtual void SetWindowFullScreen(bool fullScreen) = 0;

        virtual float GetVolume(const std::string &outputType) const = 0;
        virtual void SetVolume(float volume, const std::string &outputType) = 0;

        virtual void ExitGame(void) = 0;

        Scene *GetScene(void);

        GameManager *GetManager(void);
        void SetManager(GameManager *manager);

    protected:
        Scene *m_scene;
        GameManager *m_manager;
    };
}
