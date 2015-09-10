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

#include "InternalApplicationArgs.h"

#include "FrameRateController.h"

namespace ursine
{
    class Application
    {
    protected:
        // command line arguments count
        int m_argc;

        // command line arguments
        char **m_argv;

        // determines if the application should continue updating
        bool m_isRunning;

        // delta frame time
        float m_dt;

        FrameRateController m_frameRateController;

        EventDispatcher<uint32> m_internalEvents;

    public:
        Application(int argc, char *argv []);

        ~Application(void);

        void Run(void);

        void Exit(void);

        int GetArgC(void) const;
        char **GetArgV(void) const;

        EventDispatcher<uint32> &GetInternalEvents(void);

        float GetDelta(void);

        void SetTargetFPS(uint32 fps);
        uint32 GetFPS(void);
    };
}