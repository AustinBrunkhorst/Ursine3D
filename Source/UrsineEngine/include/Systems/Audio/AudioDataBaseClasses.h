/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AudioDataBaseClasses.h
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

namespace Ursine
{
    ///////////////////////////////////////////////////////////////////
    // Generated base class definitions
    ///////////////////////////////////////////////////////////////////

    class AudioParam
    {
    public:
        AudioParam(const char *param_name, float max, float min)
            : name(param_name), maximum(max), minimum(min)
        {}

        const char *name;
        const float maximum;
        const float minimum;
    private:
        AudioParam &operator=(const AudioParam &rhs);
    };

    class AudioEventData
    {
    public:
        AudioEventData(const char *event_name)
            : name(event_name)
        {}

        const char *name;
    private:
        AudioEventData &operator=(const AudioEventData &rhs);
    };

    ///////////////////////////////////////////////////////////////////
    // End of base class definitions
    ///////////////////////////////////////////////////////////////////
}