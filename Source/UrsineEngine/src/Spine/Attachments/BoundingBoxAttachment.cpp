#include "UrsinePrecompiled.h"

#include "Spine/Spine.h"

namespace ursine
{
    namespace spine
    {
        BoundingBoxAttachment::BoundingBoxAttachment(const char *name)
            : Attachment(name, SP_ATTACHMENT_BOUNDING_BOX)
            , vertex_count(0)
            , vertices(nullptr)
        {

        }

        BoundingBoxAttachment::~BoundingBoxAttachment(void)
        {
            delete []vertices;
        }

        void BoundingBoxAttachment::ComputeWorldVertices(Bone *bone, float *world_verticies)
        {
            int i;
            float px, py;
            float *vertices = this->vertices;
            float x = bone->GetSkeleton()->GetX() + bone->GetWorldX(), y = bone->GetSkeleton()->GetY() + bone->GetWorldY();
            for (i = 0; i < this->vertex_count; i += 2)
            {
                px = vertices[i];
                py = vertices[i + 1];
                world_verticies[i] = px * bone->GetM00() + py * bone->GetM01() + x;
                world_verticies[i + 1] = px * bone->GetM10() + py * bone->GetM11() + y;
            }
        }
    }
}
