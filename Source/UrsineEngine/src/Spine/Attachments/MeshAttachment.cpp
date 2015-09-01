#include "UrsinePrecompiled.h"

#include "Spine/Spine.h"

namespace ursine
{
    namespace spine
    {
        MeshAttachment::MeshAttachment(const char *name)
            : Attachment(name, SP_ATTACHMENT_MESH)
            , vertex_count(0)
            , vertices(nullptr)
            , hull_length(0)
            , region_uvs(nullptr)
            , uvs(nullptr)
            , triangle_count(0)
            , triangles(nullptr)
            , edge_count(0)
            , edges(nullptr)
        {
            r = 1;
            g = 1;
            b = 1;
            a = 1;
        }

        MeshAttachment::~MeshAttachment(void)
        {
            delete []vertices;
            delete []region_uvs;
            delete []uvs;
            delete []triangles;
            delete []edges;
        }

        void MeshAttachment::UpdateUVs(void)
        {
            int i;

            float width = region_u2 - region_u;
            float height = region_v2 - region_v;

            delete []uvs;

            uvs = new float[vertex_count];

            if (region_rotate)
            {
                for (i = 0; i < vertex_count; i += 2)
                {
                    uvs[i] = region_u + region_uvs[i + 1] * width;
                    uvs[i + 1] = region_v + height - region_uvs[i] * height;
                }
            }
            else
            {
                for (i = 0; i < vertex_count; i += 2)
                {
                    uvs[i] = region_u + region_uvs[i] * width;
                    uvs[i + 1] = region_v + region_uvs[i + 1] * height;
                }
            }
        }

        void MeshAttachment::ComputeWorldVertices(Slot *slot, float *worldVertices)
        {
            int i;

            float *vertices = this->vertices;

            const Bone *bone = slot->bone;

            float x = bone->GetSkeleton()->GetX() + bone->GetWorldX();
            float y = bone->GetSkeleton()->GetY() + bone->GetWorldY();

            if (slot->attachment_verts_count == vertex_count)
                vertices = slot->attachment_verts;

            for (i = 0; i < vertex_count; i += 2)
            {
                const float vx = vertices[i], vy = vertices[i + 1];

                worldVertices[i] = vx * bone->GetM00() + vy * bone->GetM01() + x;
                worldVertices[i + 1] = vx * bone->GetM10() + vy * bone->GetM11() + y;
            }
        }
    }
}
