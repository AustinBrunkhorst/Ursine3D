#include "UrsinePrecompiled.h"

#include "Spine/Spine.h"

#include <string.h>

namespace ursine
{
    namespace spine
    {
        AnimationState::AnimationState(AnimationStateData *data)
            : data(data)
            , time_scale(0)
            , listener(nullptr)
            , track_count(0)
            , tracks(nullptr)
            , renderer_object(nullptr)
        {
            events = new SpineEvent *[64];
            time_scale = 1;
        }

        AnimationState::~AnimationState(void)
        {
            int i;

            delete []events;

            for (i = 0; i < track_count; ++i)
                disposeAllEntries(tracks[i]);

            delete []tracks;
        }

        void AnimationState::Update(float delta)
        {
            int i;

            float previousDelta;

            delta *= time_scale;

            for (i = 0; i < track_count; ++i)
            {
                TrackEntry *current = tracks[i];

                if (!current)
                    continue;

                current->time += delta * current->time_scale;

                if (current->previous)
                {
                    previousDelta = delta * current->previous->time_scale;
                    current->previous->time += previousDelta;
                    current->mix_time += previousDelta;
                }

                if (current->next)
                {
                    current->next->time = current->last_time - current->next->delay;

                    if (current->next->time >= 0)
                        setCurrent(i, current->next);
                }
                else
                {
                    /* End non-looping animation when it reaches its end time and there is no next entry. */
                    if (!current->loop && current->last_time >= current->end_time)
                        ClearTrack(i);
                }
            }
        }

        void AnimationState::Apply(Skeleton *skeleton)
        {
            int i, ii;
            int eventsCount;
            int entryChanged;
            float time;
            TrackEntry *previous;

            for (i = 0; i < track_count; ++i)
            {
                TrackEntry *current = tracks[i];

                if (!current)
                    continue;

                eventsCount = 0;

                time = current->time;

                if (!current->loop && time > current->end_time)
                    time = current->end_time;

                previous = current->previous;

                if (!previous)
                {
                    if (current->mix == 1)
                    {
                        current->animation->Apply(skeleton, current->last_time, time,
                            current->loop, events, &eventsCount);
                    }
                    else
                    {
                        current->animation->Mix(skeleton, current->last_time, time,
                            current->loop, events, &eventsCount, current->mix);
                    }
                }
                else
                {
                    float alpha = current->mix_time / current->mix_duration * current->mix;

                    float previousTime = previous->time;
                    if (!previous->loop && previousTime > previous->end_time)
                        previousTime = previous->end_time;
                    current->animation->Apply(skeleton, previousTime, previousTime, previous->loop, 0, 0);

                    if (alpha >= 1)
                    {
                        alpha = 1;

                        delete current->previous;

                        current->previous = 0;
                    }
                    current->animation->Mix(skeleton, current->last_time, time,
                        current->loop, events, &eventsCount, alpha);
                }

                entryChanged = 0;

                for (ii = 0; ii < eventsCount; ++ii)
                {
                    SpineEvent *event = events[ii];

                    if (current->listener)
                    {
                        current->listener(this, i, SP_ANIMATION_EVENT, event, 0);

                        if (tracks[i] != current)
                        {
                            entryChanged = 1;
                            break;
                        }
                    }

                    if (listener)
                    {
                        listener(this, i, SP_ANIMATION_EVENT, event, 0);

                        if (tracks[i] != current)
                        {
                            entryChanged = 1;
                            break;
                        }
                    }
                }

                if (entryChanged)
                    continue;

                /* Check if completed the animation or a loop iteration. */
                if (current->loop ? (fmod(current->last_time, current->end_time) > fmod(time, current->end_time))
                    : (current->last_time < current->end_time && time >= current->end_time))
                {
                    int count = (int)(time / current->end_time);
                    if (current->listener)
                    {
                        current->listener(this, i, SP_ANIMATION_COMPLETE, 0, count);
                        if (tracks[i] != current)
                            continue;
                    }
                    if (listener)
                    {
                        listener(this, i, SP_ANIMATION_COMPLETE, 0, count);
                        if (tracks[i] != current)
                            continue;
                    }
                }

                current->last_time = current->time;
            }
        }

        void AnimationState::ClearTracks(AnimationState *self)
        {
            int i;

            for (i = 0; i < track_count; ++i)
                ClearTrack(i);

            track_count = 0;
        }

        void AnimationState::ClearTrack(int trackIndex)
        {
            TrackEntry *current;

            if (trackIndex >= track_count)
                return;

            current = tracks[trackIndex];

            if (!current)
                return;

            if (current->listener)
                current->listener(this, trackIndex, SP_ANIMATION_END, 0, 0);

            if (listener)
                listener(this, trackIndex, SP_ANIMATION_END, 0, 0);

            tracks[trackIndex] = nullptr;

            disposeAllEntries(current);
        }

        TrackEntry *AnimationState::SetAnimation(int trackIndex, const char *animationName, bool loop)
        {
            SpineAnimation *animation = data->skeleton_data->FindAnimation(animationName);

            return SetAnimation(trackIndex, animation, loop);
        }

        TrackEntry *AnimationState::SetAnimation(int trackIndex, SpineAnimation *animation, bool loop)
        {
            TrackEntry *entry;
            TrackEntry *current = expandToIndex(trackIndex);

            if (current)
                disposeAllEntries(current->next);

            entry = new TrackEntry(this);

            entry->animation = animation;
            entry->loop = loop;
            entry->end_time = animation->GetDuration();

            setCurrent(trackIndex, entry);

            return entry;
        }

        TrackEntry *AnimationState::AddAnimation(int trackIndex, const char *animationName, bool loop, float delay)
        {
            SpineAnimation *animation = data->skeleton_data->FindAnimation(animationName);

            return AddAnimation(trackIndex, animation, loop, delay);
        }

        TrackEntry *AnimationState::AddAnimation(int trackIndex, SpineAnimation *animation, bool loop, float delay)
        {
            TrackEntry *last;

            TrackEntry *entry = new TrackEntry(this);

            entry->animation = animation;
            entry->loop = loop;
            entry->end_time = animation->GetDuration();

            last = expandToIndex(trackIndex);

            if (last)
            {
                while (last->next)
                    last = last->next;
                last->next = entry;
            }
            else
                tracks[trackIndex] = entry;

            if (delay <= 0)
            {
                if (last)
                    delay += last->end_time - data->GetMix(last->animation, animation);
                else
                    delay = 0;
            }

            entry->delay = delay;

            return entry;
        }

        TrackEntry *AnimationState::GetCurrent(int track_index)
        {
            if (track_index >= track_count)
                return nullptr;

            return tracks[track_index];
        }

        void AnimationState::disposeAllEntries(TrackEntry *entry)
        {
            while (entry)
            {
                TrackEntry *next = entry->next;

                delete entry;

                entry = next;
            }
        }

        TrackEntry *AnimationState::expandToIndex(int index)
        {
            TrackEntry * *newTracks;

            if (index < track_count)
                return tracks[index];

            newTracks = new TrackEntry *[index + 1]();

            memcpy(newTracks, tracks, track_count * sizeof(TrackEntry*));

            delete []tracks;

            tracks = newTracks;
            track_count = index + 1;

            return nullptr;
        }

        void AnimationState::setCurrent(int index, TrackEntry *entry)
        {
            TrackEntry *current = expandToIndex(index);

            if (current)
            {
                TrackEntry *previous = current->previous;

                current->previous = nullptr;

                if (current->listener)
                    current->listener(this, index, SP_ANIMATION_END, 0, 0);

                if (listener)
                    listener(this, index, SP_ANIMATION_END, 0, 0);

                entry->mix_duration = data->GetMix(current->animation, entry->animation);

                if (entry->mix_duration > 0)
                {
                    entry->mix_time = 0;

                    /* If a mix is in progress, mix from the closest animation. */
                    if (previous && current->mix_time / current->mix_duration < 0.5f)
                    {
                        entry->previous = previous;
                        previous = current;
                    }
                    else
                        entry->previous = current;
                }
                else
                    delete current;

                if (previous)
                    delete previous;
            }

            tracks[index] = entry;

            if (entry->listener)
            {
                entry->listener(this, index, SP_ANIMATION_START, 0, 0);

                if (tracks[index] != entry)
                    return;
            }

            if (listener)
                listener(this, index, SP_ANIMATION_START, 0, 0);
        }
    }
}