#include "UrsinePrecompiled.h"

#include "Spine/Timelines/RotateTimeline.h"
#include "Spine/SpineAnimationConfig.h"

namespace ursine
{
    namespace spine
    {
        RotateTimeline::RotateTimeline(int frame_count)
            : BaseTimeline(SP_TIMELINE_ROTATE, frame_count, 2) {}

        RotateTimeline::~RotateTimeline(void) {}

        void RotateTimeline::Apply(Skeleton *skeleton, float lastTime, float time, SpineEvent **firedEvents, int *eventsCount, float alpha)
        {
            Bone *bone;
            int frameIndex;
            float prevFrameValue, frameTime, percent, amount;

            if (time < frames[0])
                return; /* Time is before first frame. */

            bone = skeleton->GetBones()[bone_index];

            if (time >= frames[frame_count - 2])
            { /* Time is after last frame. */
                float amount = bone->GetData()->rotation + frames[frame_count - 1] - bone->GetRotation();
                while (amount > 180)
                    amount -= 360;
                while (amount < -180)
                    amount += 360;
                bone->SetRotation(bone->GetRotation() + amount * alpha);
                return;
            }

            /* Interpolate between the previous frame and the current frame. */
            frameIndex = BinarySearch(&(frames[0]), frame_count, time, 2);
            prevFrameValue = frames[frameIndex - 1];
            frameTime = frames[frameIndex];
            percent = 1 - (time - frameTime) / (frames[frameIndex + ROTATE_PREV_FRAME_TIME] - frameTime);
            percent = GetCurvePercent((frameIndex >> 1) - 1, percent < 0 ? 0 : (percent > 1 ? 1 : percent));

            amount = frames[frameIndex + ROTATE_FRAME_VALUE] - prevFrameValue;
            while (amount > 180)
                amount -= 360;
            while (amount < -180)
                amount += 360;
            amount = bone->GetData()->rotation + (prevFrameValue + amount * percent) - bone->GetRotation();
            while (amount > 180)
                amount -= 360;
            while (amount < -180)
                amount += 360;
            bone->SetRotation(bone->GetRotation() + amount * alpha);
        }

        void RotateTimeline::SetFrame(int frame_index, float time, float angle)
        {
            frame_index *= 2;

            frames[frame_index] = time;
            frames[frame_index + 1] = angle;
        }
    }
}