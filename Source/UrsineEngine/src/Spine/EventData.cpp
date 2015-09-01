#include "UrsinePrecompiled.h"

#include "Spine/Spine.h"

namespace ursine
{
    namespace spine
    {
        EventData::EventData(const char *name)
            : name(name)
            , int_value(0)
            , float_value(0)
            , string_value(nullptr)
        {

        }

        EventData::~EventData(void)
        {

        }
    }
}
