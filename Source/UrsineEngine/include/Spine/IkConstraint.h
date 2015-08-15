#pragma once

namespace Ursine
{
    namespace Spine
    {
        struct IkConstraintData;
        class Bone;

        class IkConstraint
        {
            friend class Skeleton;
            friend class SpineAnimation;
            friend class IkConstraintTimeline;

            IkConstraintData * const data;

            int bone_count;
            Bone **bones;

            Bone *target;
            int bend_direction;
            float mix;

            void apply(Bone *bone, float targetX, float targetY, float alpha);
            void apply(Bone *parent, Bone *child, float targetX, float targetY, int bendDirection, float alpha);

        public:
            IkConstraint(IkConstraintData *data, const Skeleton *skeleton);
            ~IkConstraint(void);

            void Apply(void);
        };
    }
}
