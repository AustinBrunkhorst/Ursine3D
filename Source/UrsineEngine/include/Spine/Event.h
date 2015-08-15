#pragma once

#include "Spine/EventData.h"

namespace Ursine
{
    namespace Spine
    {
        struct SpineEvent
        {
            EventData * const data;

            int int_value;
            float float_value;
            std::string string_value;

            SpineEvent(EventData *data);
            ~SpineEvent(void);
        };
    }
}
