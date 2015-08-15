#pragma once

#include "Spine/SpineAnimation.h"
#include "Spine/AnimationStateData.h"
#include "Spine/AnimationStateConfig.h"
#include "Spine/TrackEntry.h"
#include "Spine/Event.h"

namespace Ursine
{
    class SkeletonRenderer;

    namespace Spine
    {
        class AnimationState
        {
            friend SkeletonRenderer;

            AnimationStateData * const data;
            float time_scale;
            spAnimationStateListener listener;

            int track_count;
            TrackEntry **tracks;

            void *renderer_object;

            SpineEvent **events;

            void disposeAllEntries(TrackEntry *entry);

            TrackEntry *expandToIndex(int index);

            void setCurrent(int index, TrackEntry *entry);

        public:
            AnimationState(AnimationStateData *data);
            ~AnimationState(void);

            void Update(float delta);
            void Apply(Skeleton *skeleton);

            void ClearTracks(AnimationState *self);
            void ClearTrack(int trackIndex);

            TrackEntry *SetAnimation(int trackIndex, const char *animationName, bool loop);
            TrackEntry *SetAnimation(int trackIndex, SpineAnimation *animation, bool loop);

            TrackEntry *AddAnimation(int trackIndex, const char *animationName, bool loop, float delay);
            TrackEntry *AddAnimation(int trackIndex, SpineAnimation *animation, bool loop, float delay);

            TrackEntry *GetCurrent(int trackIndex);
        };
    }
}