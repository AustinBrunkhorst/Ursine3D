#include "UrsinePrecompiled.h"

#include "Spine/spine.h"

namespace Ursine
{
    namespace Spine
    {
        AnimationStateData::ToEntry::ToEntry(SpineAnimation *to, float Duration)
        {
            animation = to;
            duration = Duration;
        }

        AnimationStateData::ToEntry::~ToEntry(void) {}

        AnimationStateData::FromEntry::FromEntry(SpineAnimation *from)
        {
            animation = from;
        }

        AnimationStateData::FromEntry::~FromEntry(void) {}

        AnimationStateData::AnimationStateData(SkeletonData *skeleton_data)
            : skeleton_data(skeleton_data)
            , default_mix(0)
        {

        }

        AnimationStateData::~AnimationStateData(void)
        {
            for (auto from_entry : entries)
            {
                for (auto to_entry : from_entry->to_entries)
                    delete to_entry;

                delete from_entry;
            }
        }

        void AnimationStateData::SetMix(const char *from_name, const char *to_name, float duration)
        {
            SpineAnimation *to;

            SpineAnimation *from = skeleton_data->FindAnimation(from_name);

            if (!from)
                return;

            to = skeleton_data->FindAnimation(to_name);

            if (!to)
                return;

            SetMix(from, to, duration);
        }

        void AnimationStateData::SetMix(SpineAnimation *from, SpineAnimation *to, float Duration)
        {
            URSINE_TODO("test this function");

            if (entries.empty())
            {
                entries.push_back(new FromEntry(from));
            }
            else
            {
                for (auto from_entry : entries)
                {
                    if (from_entry->animation == from)
                    {
                        for (auto to_entry : from_entry->to_entries)
                        {
                            if (to_entry->animation == to)
                            {
                                to_entry->duration = Duration;

                                return;
                            }
                        }

                        break;
                    }
                }
            }

            entries.back()->to_entries.push_back(new ToEntry(to, Duration));
        }

        float AnimationStateData::GetMix(SpineAnimation *from, SpineAnimation *to)
        {
            for (auto from_entry : entries)
            {
                if (from_entry->animation == from)
                {
                    for (auto to_entry : from_entry->to_entries)
                    {
                        if (to_entry->animation == to)
                            return to_entry->duration;
                    }
                }
            }

            return default_mix;
        }
    }
}
