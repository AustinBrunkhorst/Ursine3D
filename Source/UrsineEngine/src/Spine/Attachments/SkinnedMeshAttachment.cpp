#include "UrsinePrecompiled.h"

#include "Spine/spine.h"

namespace Ursine
{
    namespace Spine
    {
        SkinnedMeshAttachment::SkinnedMeshAttachment(const char *name)
            : Attachment(name, SP_ATTACHMENT_SKINNED_MESH)
            , bone_count(0)
            , bones(nullptr)
            , weight_count(0)
            , weights(nullptr)
            , triangle_count(0)
            , triangles(nullptr)
            , uv_count(0)
            , region_uvs(nullptr)
            , uvs(nullptr)
            , hull_length(0)
            , edge_count(0)
            , edges(nullptr)
        {
            r = 1;
            g = 1;
            b = 1;
            a = 1;
        }

        SkinnedMeshAttachment::~SkinnedMeshAttachment(void)
        {
            delete []bones;
            delete []weights;
            delete []region_uvs;
            delete []uvs;
            delete []triangles;
            delete []edges;
        }

        void SkinnedMeshAttachment::UpdateUVs(void)
        {
            int i;

            float width = region_u2 - region_u;
            float height = region_v2 - region_v;

            delete []uvs;

            uvs = new float[uv_count];

            if (region_rotate)
            {
                for (i = 0; i < uv_count; i += 2)
                {
                    uvs[i] = region_u + region_uvs[i + 1] * width;
                    uvs[i + 1] = region_v + height - region_uvs[i] * height;
                }
            }
            else
            {
                for (i = 0; i < uv_count; i += 2)
                {
                    uvs[i] = region_u + region_uvs[i] * width;
                    uvs[i + 1] = region_v + region_uvs[i + 1] * height;
                }
            }
        }

        void SkinnedMeshAttachment::ComputeWorldVertices(Slot *slot, float *world_verticies)
        {
            int w = 0;
            int v = 0;
            int b = 0;
            int f = 0;

            float x = slot->bone->GetSkeleton()->GetX();
            float y = slot->bone->GetSkeleton()->GetY();

            Bone **skeletonBones = slot->bone->GetSkeleton()->GetBones();

            if (slot->attachment_verts_count == 0)
            {
                for (; v < bone_count; w += 2)
                {
                    float wx = 0;
                    float wy = 0;

                    const int nn = bones[v] + v;

                    v++;
                    for (; v <= nn; v++, b += 3)
                    {
                        const Bone *bone = skeletonBones[bones[v]];

                        const float vx = weights[b];
                        const float vy = weights[b + 1];

                        const float weight = weights[b + 2];

                        wx += (vx * bone->GetM00() + vy * bone->GetM01() + bone->GetWorldX()) * weight;
                        wy += (vx * bone->GetM10() + vy * bone->GetM11() + bone->GetWorldY()) * weight;
                    }

                    world_verticies[w] = wx + x;
                    world_verticies[w + 1] = wy + y;
                }
            }
            else
            {
                const float *ffd = slot->attachment_verts;

                for (; v < bone_count; w += 2)
                {
                    float wx = 0;
                    float wy = 0;

                    const int nn = bones[v] + v;

                    v++;

                    for (; v <= nn; v++, b += 3, f += 2)
                    {
                        const Bone *bone = skeletonBones[bones[v]];

                        const float vx = weights[b] + ffd[f];
                        const float vy = weights[b + 1] + ffd[f + 1];

                        const float weight = weights[b + 2];

                        wx += (vx * bone->GetM00() + vy * bone->GetM01() + bone->GetWorldX()) * weight;
                        wy += (vx * bone->GetM10() + vy * bone->GetM11() + bone->GetWorldY()) * weight;
                    }

                    world_verticies[w] = wx + x;
                    world_verticies[w + 1] = wy + y;
                }
            }
        }
    }
}
