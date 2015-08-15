#pragma once

#include "Spine/Attachment.h"
#include "Spine/Slot.h"

namespace Ursine
{
    class SkeletonRenderer;

    namespace Spine
    {
        class SkinnedMeshAttachment : public Attachment
        {
            friend class SkeletonJson;
            friend class AtlasAttachmentLoader;
            friend SkeletonRenderer;

            std::string path;

            int bone_count;
            int *bones;

            int weight_count;
            float *weights;

            int triangle_count;
            int *triangles;

            int uv_count;

            float *region_uvs;

            float *uvs;

            int hull_length;

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

            float region_u;
            float region_v;

            float region_u2;
            float region_v2;

            bool region_rotate;

            int edge_count;
            int *edges;

            float width;
            float height;

        public:
            SkinnedMeshAttachment(const char *name);
            ~SkinnedMeshAttachment(void);

            void UpdateUVs(void);

            void ComputeWorldVertices(Slot *slot, float *world_verticies);
        };
    }
}
