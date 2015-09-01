#include "UrsinePrecompiled.h"

#include "Spine/Timelines/TranslateTimeline.h"
#include "Spine/SpineAnimationConfig.h"

namespace ursine
{
    namespace spine
    {
        TranslateTimeline::TranslateTimeline(int frame_count)
            : BaseTimeline(SP_TIMELINE_SCALE, frame_count, 3) {}

        TranslateTimeline::~TranslateTimeline(void) {}

        void TranslateTimeline::Apply(Skeleton *skeleton, float lastTime, float time, SpineEvent **firedEvents, int *eventsCount, float alpha)
        {
            Bone *bone;
            int frameIndex;
            float prevFrameX, prevFrameY, frameTime, percent;

            if (time < frames[0])
                return; /* Time is before first frame. */

            bone = skeleton->GetBones()[bone_index];

            if (time >= frames[frame_count - 3])
            { /* Time is after last frame. */
                bone->SetX(bone->GetX() + (bone->GetData()->x + frames[frame_count - 2] - bone->GetX()) * alpha);
                bone->SetY(bone->GetY() + (bone->GetData()->y + frames[frame_count - 1] - bone->GetY()) * alpha);
                return;
            }

            /* Interpolate between the previous frame and the current frame. */
            frameIndex = BinarySearch(&(frames[0]), frame_count, time, 3);
            prevFrameX = frames[frameIndex - 2];
            prevFrameY = frames[frameIndex - 1];
            frameTime = frames[frameIndex];
            percent = 1 - (time - frameTime) / (frames[frameIndex + TRANSLATE_PREV_FRAME_TIME] - frameTime);
            percent = GetCurvePercent(frameIndex / 3 - 1, percent < 0 ? 0 : (percent > 1 ? 1 : percent));

            bone->SetX(bone->GetX() + (bone->GetData()->x + prevFrameX + (frames[frameIndex + TRANSLATE_FRAME_X] - prevFrameX) * percent - bone->GetX())
                * alpha);
            bone->SetY(bone->GetY() + (bone->GetData()->y + prevFrameY + (frames[frameIndex + TRANSLATE_FRAME_Y] - prevFrameY) * percent - bone->GetY())
                * alpha);
        }

        void TranslateTimeline::SetFrame(int frameIndex, float time, float x, float y)
        {
            frameIndex *= 3;

            frames[frameIndex] = time;
            frames[frameIndex + 1] = x;
            frames[frameIndex + 2] = y;
        }
    }
}
