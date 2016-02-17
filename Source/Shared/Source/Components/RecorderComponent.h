/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** RecorderComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

namespace ursine
{
    // This is the recorder component for all entities that are recording their commands
    class Recorder : public ecs::Component
    {
    public:
        // Enum for this recorder's state (recording, stopped, playback)
        // ID for the recording index
        // Timer for the current recording time

    private:
    };
}
