#pragma once

namespace Ursine
{
    namespace Spine
    {
        class AttachmentLoader;
        class Atlas;
        struct SkeletonData;
        class SpineAnimation;
        struct SpineJson;

        class SkeletonJson
        {
            bool owns_loader;

            float scale;

            AttachmentLoader *attachment_loader;

            SpineAnimation *readAnimation(SpineJson *root, SkeletonData *skeletonData);

        public:
            SkeletonJson(Atlas *atlas);
            SkeletonJson(AttachmentLoader *attachmentLoader);

            ~SkeletonJson(void);

            SkeletonData *ReadData(const char *json);
            SkeletonData *ReadFile(const char *path);

            float GetScale(void) const;
            void SetScale(float scale);
        };
    }
}
