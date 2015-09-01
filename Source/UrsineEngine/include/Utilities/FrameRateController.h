/* ---------------------------------------------------------------------------
** Team Bear King
** � 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** FrameRateController.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

namespace ursine
{
    class FrameRateController final
    {
        // delta time
        float m_dt;

        // duration for each frame (FPS) in milliseconds
        uint32 m_frameDuration;

        // duration elapsed (used for calculating FPS)
        uint32 m_fpsElapsed;

        // total number of fps based on elapsed
        uint32 m_fpsTotal;

        // current fps
        uint32 m_fps;

        // last frame begin
        uint32 m_lastUpdate;

        // last frame end
        uint32 m_currentTime;

    public:
        FrameRateController(void);

        void FrameBegin(void);
        void FrameEnd(void);

        uint32 GetFPS(void);
        float GetDeltaTime(void);

        void SetTargetFPS(uint32 fps);
    };
}