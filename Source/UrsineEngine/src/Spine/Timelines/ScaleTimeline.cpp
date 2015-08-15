#include "UrsinePrecompiled.h"

#include "Spine/Timelines/ScaleTimeline.h"
#include "Spine/SpineAnimationConfig.h"

namespace Ursine
{
    namespace Spine
    {
        ScaleTimeline::ScaleTimeline(int frame_count)
            : BaseTimeline(SP_TIMELINE_SCALE, frame_count, 3) { }

        ScaleTimeline::~ScaleTimeline(void) { }

        void ScaleTimeline::Apply(Skeleton *skeleton, float lastTime, float time, SpineEvent **firedEvents, int *eventsCount, float alpha)
        {
            Bone *bone;
            int frameIndex;
            float prevFrameX, prevFrameY, frameTime, percent;

            if (time < frames[0])
                return; /* Time is before first frame. */

            bone = skeleton->GetBones()[bone_index];

            if (time >= frames[frame_count - 3])
            { /* Time is after last frame. */
                bone->SetScaleX(bone->GetScaleX() + (bone->GetData()->scale_x * frames[frame_count - 2] - bone->GetScaleX()) * alpha);
                bone->SetScaleY(bone->GetScaleY() + (bone->GetData()->scale_y * frames[frame_count - 1] - bone->GetScaleY()) * alpha);
                return;
            }

            /* Interpolate between the previous frame and the current frame. */
            frameIndex = BinarySearch(&(frames[0]), frame_count, time, 3);
            prevFrameX = frames[frameIndex - 2];
            prevFrameY = frames[frameIndex - 1];
            frameTime = frames[frameIndex];
            percent = 1 - (time - frameTime) / (frames[frameIndex + TRANSLATE_PREV_FRAME_TIME] - frameTime);
            percent = GetCurvePercent(frameIndex / 3 - 1, percent < 0 ? 0 : (percent > 1 ? 1 : percent));

            bone->SetScaleX(bone->GetScaleX() + (bone->GetData()->scale_x * (prevFrameX + (frames[frameIndex + TRANSLATE_FRAME_X] - prevFrameX) * percent)
                - bone->GetScaleX()) * alpha);
            bone->SetScaleY(bone->GetScaleY() + (bone->GetData()->scale_y * (prevFrameY + (frames[frameIndex + TRANSLATE_FRAME_Y] - prevFrameY) * percent)
                - bone->GetScaleY()) * alpha);
        }

        void ScaleTimeline::SetFrame(int frameIndex, float time, float x, float y)
        {
            frameIndex *= 3;

            frames[frameIndex] = time;
            frames[frameIndex + 1] = x;
            frames[frameIndex + 2] = y;
        }
    }
}
