#include "UrsinePrecompiled.h"

#include "Spine/Spine.h"

namespace ursine
{
    namespace spine
    {
        TrackEntry::TrackEntry(AnimationState *state)
            : state(state)
            , next(nullptr)
            , previous(nullptr)
            , animation(nullptr)
            , loop(0)
            , delay(0)
            , time(0)
            , last_time(-1)
            , end_time(0)
            , time_scale(1)
            , listener(nullptr)
            , mix_time(0)
            , mix_duration(0)
            , mix(1)
            , renderer_object(nullptr)
        {

        }

        TrackEntry::~TrackEntry(void)
        {
            URSINE_TODO("test this");
            delete previous;
        }
    }
}
