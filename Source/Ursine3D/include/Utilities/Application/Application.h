/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Application.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "EventDispatcher.h"

#include "ApplicationEvent.h"
#include "PlatformEventArgs.h"

#include "FrameRateController.h"
#include "TimeSpan.h"

#include <mutex>

namespace ursine
{
    namespace core
    {
        class CoreSystem;
    }

    class Application 
        : public EventDispatcher<ApplicationEventType>
    {
    public:
        typedef std::function<void(void)> Task;

        static Application *Instance;

        Application(int argc, char *argv[]);
        ~Application(void);

        void Run(void);

        void Exit(void);

        int GetArgC(void) const;
        char **GetArgV(void) const;

        bool IsActive(void) const;
        void SetActive(bool active);

        DeltaTime GetDeltaTime(void) const;

        TimeSpan GetTimeSinceStartup(void) const;

        FrameRateController &GetFrameRateController(void);

        EventDispatcher<uint32> &GetPlatformEvents(void);

        template<typename SystemType>
        SystemType *CoreSystem(void);

        static void PostMainThread(Task task);

    protected:
        // determines if the application should continue updating
        bool m_isRunning;

        // determines if the application is actively updating
        bool m_isActive;

        // command line arguments count
        int m_argc;

        // delta time
        DeltaTime m_dt;

        // command line arguments
        char **m_argv;

        FrameRateController m_frameRateController;

        std::vector<core::CoreSystem *> m_systems;

        EventDispatcher<uint32> m_platformEvents;

        std::mutex m_mutex;

        std::vector<Task> m_tasks;

        void flushTasks(void);
    };
}

#include "Application.hpp"