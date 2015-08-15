#pragma once

#include "Spine/Attachment.h"
#include "Spine/Atlas.h"
#include "Spine/Slot.h"

namespace Ursine
{
    class SkeletonRenderer;

    namespace Spine
    {
        class MeshAttachment : public Attachment
        {
            friend class SkeletonJson;
            friend class AtlasAttachmentLoader;
            friend SkeletonRenderer;

            std::string path;

            int vertex_count;
            float *vertices;
            int hull_length;

            float *region_uvs;
            float *uvs;

            int triangle_count;
            int region_height;

            int region_original_width;
            int region_original_height;

            float region_u;
            int *triangles;

            float r;
            float g;
            float b;
            float a;

            void *renderer_object;

            int region_offset_x;
            int region_offset_y;

            int region_width;
            float region_v;

            float region_u2;
            float region_v2;

            bool region_rotate;

            int edge_count;

            int *edges;

            float width;
            float height;

        public:
            MeshAttachment(const char *name);
            ~MeshAttachment(void);

            void UpdateUVs(void);
            void ComputeWorldVertices(Slot *slot, float *worldVertices);
        };

    }
}
