#include "UrsinePrecompiled.h"

#include "Spine/Spine.h"

namespace ursine
{
    namespace spine
    {
        SpineEvent::SpineEvent(EventData *data)
            : data(data)
            , int_value(0)
            , float_value(0)
            , string_value(nullptr)
        {

        }

        SpineEvent::~SpineEvent(void)
        {

        }

    }
}
