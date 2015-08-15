#include "UrsinePrecompiled.h"

#include "Spine/Timelines/IkConstraintTimeline.h"
#include "Spine/SpineAnimationConfig.h"

namespace Ursine
{
    namespace Spine
    {
        IkConstraintTimeline::IkConstraintTimeline(int framesCount)
            : BaseTimeline(SP_TIMELINE_IKCONSTRAINT, framesCount, 3)
            , constraint_index(0) {}

        IkConstraintTimeline::~IkConstraintTimeline(void) {}

        void IkConstraintTimeline::Apply(Skeleton *skeleton, float lastTime, float time, SpineEvent **firedEvents, int *eventsCount, float alpha)
        {
            int frameIndex;
            float prevFrameMix, frameTime, percent, mix;
            IkConstraint *ikConstraint;

            if (time < frames[0])
                return; /* Time is before first frame. */

            ikConstraint = skeleton->GetIKConstraints()[constraint_index];

            if (time >= frames[frame_count - 3])
            { /* Time is after last frame. */
                ikConstraint->mix += (frames[frame_count - 2] - ikConstraint->mix) * alpha;

                ikConstraint->bend_direction = (int)frames[frame_count - 1];
                return;
            }

            /* Interpolate between the previous frame and the current frame. */
            frameIndex = BinarySearch(&(frames[0]), frame_count, time, 3);

            prevFrameMix = frames[frameIndex + IKCONSTRAINT_PREV_FRAME_MIX];

            frameTime = frames[frameIndex];

            percent = 1 - (time - frameTime) / (frames[frameIndex + IKCONSTRAINT_PREV_FRAME_TIME] - frameTime);

            percent = GetCurvePercent(frameIndex / 3 - 1, percent < 0 ? 0 : (percent > 1 ? 1 : percent));

            mix = prevFrameMix + (frames[frameIndex + IKCONSTRAINT_FRAME_MIX] - prevFrameMix) * percent;

            ikConstraint->mix += (mix - ikConstraint->mix) * alpha;

            ikConstraint->bend_direction = (int)frames[frameIndex + IKCONSTRAINT_PREV_FRAME_BEND_DIRECTION];
        }

        void IkConstraintTimeline::SetFrame(int frameIndex, float time, float mix, int bendDirection)
        {
            frameIndex *= 3;

            frames[frameIndex] = time;
            frames[frameIndex + 1] = mix;
            frames[frameIndex + 2] = (float)bendDirection;
        }
    }
}
