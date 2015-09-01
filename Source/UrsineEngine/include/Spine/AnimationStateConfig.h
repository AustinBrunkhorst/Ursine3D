#pragma once

namespace ursine
{
    namespace spine
    {
        enum EventType
        {
            SP_ANIMATION_START,
            SP_ANIMATION_END,
            SP_ANIMATION_COMPLETE,
            SP_ANIMATION_EVENT
        };

        class AnimationState;
        struct SpineEvent;

        typedef void(*spAnimationStateListener)(AnimationState *state, int trackIndex, EventType type, SpineEvent *event,
            int loopCount);
    }
}