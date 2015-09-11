/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
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
#include "InternalApplicationArgs.h"

#include "CoreSystem.h"

#include "FrameRateController.h"

namespace ursine
{
    class Application 
        : public EventDispatcher<ApplicationEvent>
    {
    public:
        static Application *Instance;

        Application(int argc, char *argv[]);

        ~Application(void);

        void Run(void);

        void Exit(void);

        int GetArgC(void) const;
        char **GetArgV(void) const;

        bool IsActive(void) const;
        void SetActive(bool active);

        FrameRateController &GetFrameRateController(void);

        EventDispatcher<uint32> &GetPlatformEvents(void);

        template<typename SystemType>
        SystemType *GetCoreSystem(void);

    protected:
        // determines if the application should continue updating
        bool m_isRunning;

        // determines if the application is actively updating
        bool m_isActive;

        // command line arguments count
        int m_argc;

        // command line arguments
        char **m_argv;

        FrameRateController m_frameRateController;

        std::vector<core::CoreSystem *> m_systems;

        EventDispatcher<uint32> m_platformEvents;
    };
}

#include "Application.hpp"