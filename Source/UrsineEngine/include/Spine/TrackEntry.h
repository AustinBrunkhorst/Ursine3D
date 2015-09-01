#pragma once

#include "AnimationStateConfig.h"

namespace ursine
{
    namespace spine
    {
        class SpineAnimation;

        class TrackEntry
        {
            friend class AnimationState;

            AnimationState * const state;
            TrackEntry *next;
            TrackEntry *previous;
            SpineAnimation *animation;
            bool loop;

            float delay;
            float time;
            float last_time;
            float end_time;
            float time_scale;

            spAnimationStateListener listener;

            float mix_time;
            float mix_duration;
            float mix;

            void *renderer_object;

        public:
            TrackEntry(AnimationState *state);
            ~TrackEntry(void);
        };
    }
}