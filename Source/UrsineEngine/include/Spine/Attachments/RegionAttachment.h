#pragma once

#include "Spine/Attachment.h"
#include "Spine/Atlas.h"
#include "Spine/Slot.h"

namespace Ursine
{
    class SkeletonRenderer;

    namespace Spine
    {
        enum VertexIndex
        {
            SP_VERTEX_X1 = 0,
            SP_VERTEX_Y1,
            SP_VERTEX_X2,
            SP_VERTEX_Y2,
            SP_VERTEX_X3,
            SP_VERTEX_Y3,
            SP_VERTEX_X4,
            SP_VERTEX_Y4
        };

        class RegionAttachment : public Attachment
        {
            friend class SkeletonJson;
            friend class AtlasAttachmentLoader;
            friend SkeletonRenderer;

            std::string path;

            float x;
            float y;

            float scale_x;
            float scale_y;

            float rotation;

            float width;
            float height;

            float r;
            float g;
            float b;
            float a;

            void *renderer_object;

            int region_offset_x;
            int region_offset_y;

            int region_width;
            int region_height;

            int region_original_width;
            int region_original_height;

            float offset[8];
            float uvs[8];

        public:
            RegionAttachment(const char *name);
            ~RegionAttachment(void);

            void SetUVs(float u, float v, float u2, float v2, bool rotate);

            void UpdateOffset(void);

            void ComputeWorldVertices(Bone *bone, float *vertices);
        };
    }
}
