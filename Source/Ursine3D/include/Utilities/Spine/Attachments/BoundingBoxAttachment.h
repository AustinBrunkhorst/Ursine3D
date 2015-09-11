#pragma once

#include "Spine/Attachment.h"
#include "Spine/Atlas.h"
#include "Spine/Slot.h"

namespace ursine
{
    namespace spine
    {
        class BoundingBoxAttachment : public Attachment
        {
            friend class SkeletonJson;
            friend class SkeletonBounds;

            int vertex_count;

            float *vertices;

        public:
            BoundingBoxAttachment(const char *name);
            ~BoundingBoxAttachment(void);

            void ComputeWorldVertices(Bone *bone, float *world_verticies);
        };
    }
}
