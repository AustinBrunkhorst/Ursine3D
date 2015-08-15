#pragma once

#include <vector>
#include "Spine/SpineAnimation.h"
#include "Spine/SkeletonData.h"

namespace Ursine
{
    namespace Spine
    {
        class AnimationStateData
        {
            friend class AnimationState;

            struct ToEntry
            {
                SpineAnimation *animation;

                float duration;

                ToEntry(SpineAnimation *to, float duration);
                ~ToEntry(void);
            };

            struct FromEntry
            {
                SpineAnimation *animation;

                std::vector<ToEntry*> to_entries;

                FromEntry(SpineAnimation *from);
                ~FromEntry(void);
            };

            SkeletonData * const skeleton_data;

            float default_mix;

            std::vector<FromEntry *> entries;

        public:
            AnimationStateData(SkeletonData *skeleton_data);
            ~AnimationStateData(void);

            void SetMix(const char *from_name, const char *to_name, float duration);
            void SetMix(SpineAnimation *from, SpineAnimation *to, float duration);

            float GetMix(SpineAnimation *from, SpineAnimation *to);
        };
    }
}
