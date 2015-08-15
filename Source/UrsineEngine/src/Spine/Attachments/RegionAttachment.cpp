#include "UrsinePrecompiled.h"

#include "Spine/spine.h"

namespace Ursine
{
    namespace Spine
    {
        RegionAttachment::RegionAttachment(const char *name)
            : Attachment(name, SP_ATTACHMENT_REGION)
        {
            scale_x = 1;
            scale_y = 1;
            r = 1;
            g = 1;
            b = 1;
            a = 1;
        }

        RegionAttachment::~RegionAttachment(void)
        {

        }

        void RegionAttachment::SetUVs(float u, float v, float u2, float v2, bool rotate)
        {
            if (rotate)
            {
                uvs[SP_VERTEX_X2] = u;
                uvs[SP_VERTEX_Y2] = v2;
                uvs[SP_VERTEX_X3] = u;
                uvs[SP_VERTEX_Y3] = v;
                uvs[SP_VERTEX_X4] = u2;
                uvs[SP_VERTEX_Y4] = v;
                uvs[SP_VERTEX_X1] = u2;
                uvs[SP_VERTEX_Y1] = v2;
            }
            else
            {
                uvs[SP_VERTEX_X1] = u;
                uvs[SP_VERTEX_Y1] = v2;
                uvs[SP_VERTEX_X2] = u;
                uvs[SP_VERTEX_Y2] = v;
                uvs[SP_VERTEX_X3] = u2;
                uvs[SP_VERTEX_Y3] = v;
                uvs[SP_VERTEX_X4] = u2;
                uvs[SP_VERTEX_Y4] = v2;
            }
        }

        void RegionAttachment::UpdateOffset(void)
        {
            float regionScaleX = width / region_original_width * scale_x;
            float regionScaleY = height / region_original_height * scale_y;

            float localX = -width / 2 * scale_x + region_offset_x * regionScaleX;
            float localY = -height / 2 * scale_y + region_offset_y * regionScaleY;

            float localX2 = localX + region_width * regionScaleX;
            float localY2 = localY + region_height * regionScaleY;

            float radians = Math::DegreesToRadians(rotation);
            float cosine = cos(radians), sine = sin(radians);

            float localXCos = localX * cosine + x;
            float localXSin = localX * sine;
            float localYCos = localY * cosine + y;
            float localYSin = localY * sine;

            float localX2Cos = localX2 * cosine + x;
            float localX2Sin = localX2 * sine;
            float localY2Cos = localY2 * cosine + y;
            float localY2Sin = localY2 * sine;

            offset[SP_VERTEX_X1] = localXCos - localYSin;
            offset[SP_VERTEX_Y1] = localYCos + localXSin;
            offset[SP_VERTEX_X2] = localXCos - localY2Sin;
            offset[SP_VERTEX_Y2] = localY2Cos + localXSin;
            offset[SP_VERTEX_X3] = localX2Cos - localY2Sin;
            offset[SP_VERTEX_Y3] = localY2Cos + localX2Sin;
            offset[SP_VERTEX_X4] = localX2Cos - localYSin;
            offset[SP_VERTEX_Y4] = localYCos + localX2Sin;
        }

        void RegionAttachment::ComputeWorldVertices(Bone *bone, float *vertices)
        {
            const float *offset = this->offset;

            float x = bone->GetSkeleton()->GetX() + bone->GetWorldX(), y = bone->GetSkeleton()->GetY() + bone->GetWorldY();

            vertices[SP_VERTEX_X1] = offset[SP_VERTEX_X1] * bone->GetM00() + offset[SP_VERTEX_Y1] * bone->GetM01() + x;
            vertices[SP_VERTEX_Y1] = offset[SP_VERTEX_X1] * bone->GetM10() + offset[SP_VERTEX_Y1] * bone->GetM11() + y;
            vertices[SP_VERTEX_X2] = offset[SP_VERTEX_X2] * bone->GetM00() + offset[SP_VERTEX_Y2] * bone->GetM01() + x;
            vertices[SP_VERTEX_Y2] = offset[SP_VERTEX_X2] * bone->GetM10() + offset[SP_VERTEX_Y2] * bone->GetM11() + y;
            vertices[SP_VERTEX_X3] = offset[SP_VERTEX_X3] * bone->GetM00() + offset[SP_VERTEX_Y3] * bone->GetM01() + x;
            vertices[SP_VERTEX_Y3] = offset[SP_VERTEX_X3] * bone->GetM10() + offset[SP_VERTEX_Y3] * bone->GetM11() + y;
            vertices[SP_VERTEX_X4] = offset[SP_VERTEX_X4] * bone->GetM00() + offset[SP_VERTEX_Y4] * bone->GetM01() + x;
            vertices[SP_VERTEX_Y4] = offset[SP_VERTEX_X4] * bone->GetM10() + offset[SP_VERTEX_Y4] * bone->GetM11() + y;
        }
    }
}
