/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** FrameRateController.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "FrameRateController.h"

namespace Ursine
{
    FrameRateController::FrameRateController(void)
        : m_dt( 1.0f )
        , m_fpsElapsed( 0 )
        , m_fpsTotal( 0 )
        , m_lastUpdate( 0 )
        , m_currentTime( 0 ) { }

    void FrameRateController::FrameBegin(void)
    {
        m_currentTime = SDL_GetTicks( );

        auto delta = m_currentTime - m_lastUpdate;

        m_fpsElapsed += delta;

        m_dt = delta / 1000.0f;

        if (m_fpsElapsed >= 1000)
        {
            m_fps = m_fpsTotal;

            m_fpsTotal = 0;
            m_fpsElapsed = 0;
        }
    }

    void FrameRateController::FrameEnd(void)
    {
        ++m_fpsTotal;

        m_lastUpdate = m_currentTime;

        // wait until we can update next
        while (SDL_GetTicks( ) - m_lastUpdate < m_frameDuration);
    }

    uint32 FrameRateController::GetFPS(void)
    {
        return m_fps;
    }

    float FrameRateController::GetDeltaTime(void)
    {
        return m_dt;
    }

    void FrameRateController::SetTargetFPS(uint32 fps)
    {
        m_frameDuration = 1000 / fps;
    }
}
