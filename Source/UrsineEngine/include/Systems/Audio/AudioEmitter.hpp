/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AudioEmitter.hpp
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

namespace ursine
{
    inline const std::string &AudioEmitter::GetEvent() const
    {
        return _event_name;
    }

    inline int AudioEmitter::GetWeight() const
    {
        return _weight;
    }

    inline float AudioEmitter::GetPitch() const
    {
        return _event._pitch;
    }

    inline float AudioEmitter::GetVolume() const
    {
        return _event._volume;
    }

    inline bool AudioEmitter::GetMute() const
    {
        return _mute;
    }

    inline PlayMode AudioEmitter::GetMode() const
    {
        return _event._mode;
    }

    inline float AudioEmitter::GetDerpVal()
    {
        if (_event._playing && _derp)
        {
            int timeline;
            FMODERRCHK(_event._instance->getTimelinePosition(&timeline));
            
            return _derp->GetIntensity(timeline);
        }
        return 0.0f;
    }
}
