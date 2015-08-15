#include "UrsinePrecompiled.h"

#include "Spine/spine.h"

#include <limits.h>

namespace Ursine
{
    namespace Spine
    {
        SkeletonBounds::SkeletonBounds(void) {}

        SkeletonBounds::~SkeletonBounds(void)
        {
            int i;

            for (i = 0; i < capacity; ++i)

                if (polygons[i])
                    delete polygons[i];

            delete []polygons;
            delete []bounding_boxes;
        }

        void SkeletonBounds::Update(Skeleton *skeleton, bool update_aabb)
        {
            int i;

            if (capacity < skeleton->GetSlotCount())
            {
                SpinePolygon * *newPolygons;

                delete []bounding_boxes;

                bounding_boxes = new BoundingBoxAttachment *[skeleton->GetSlotCount()];

                newPolygons = new SpinePolygon *[skeleton->GetSlotCount()]();

                memcpy(newPolygons, polygons, capacity);

                delete []polygons;

                polygons = newPolygons;

                capacity = skeleton->GetSlotCount();
            }

            min_x = (float)INT_MAX;
            min_y = (float)INT_MAX;
            max_x = (float)INT_MIN;
            max_y = (float)INT_MIN;

            count = 0;

            for (i = 0; i < skeleton->GetSlotCount(); ++i)
            {
                SpinePolygon *polygon;

                BoundingBoxAttachment *boundingBox;

                Slot *slot = skeleton->GetSlots()[i];

                Attachment *attachment = slot->attachment;

                if (!attachment || attachment->type != SP_ATTACHMENT_BOUNDING_BOX)
                    continue;

                boundingBox = (BoundingBoxAttachment*)attachment;

                bounding_boxes[count] = boundingBox;

                polygon = polygons[count];

                if (!polygon || polygon->capacity < boundingBox->vertex_count)
                {
                    if (polygon)
                        delete polygon;

                    polygons[count] = polygon = new SpinePolygon(boundingBox->vertex_count);
                }

                polygon->count = boundingBox->vertex_count;

                boundingBox->ComputeWorldVertices(slot->bone, polygon->vertices);

                if (update_aabb)
                {
                    int ii = 0;

                    for (; ii < polygon->count; ii += 2)
                    {
                        float x = polygon->vertices[ii];
                        float y = polygon->vertices[ii + 1];

                        if (x < min_x)
                            min_x = x;

                        if (y < min_y)
                            min_y = y;

                        if (x > max_x)
                            max_x = x;

                        if (y > max_y)
                            max_y = y;
                    }
                }

                count++;
            }
        }

        bool SkeletonBounds::AABBContainsPoint(float x, float y)
        {
            return x >= min_x && x <= max_x && y >= min_y && y <= max_y;
        }

        bool SkeletonBounds::AABBIntersectsSegment(float x1, float y1, float x2, float y2)
        {
            float m, x, y;
            if ((x1 <= min_x && x2 <= min_x) || (y1 <= min_y && y2 <= min_y) || (x1 >= max_x && x2 >= max_x)
                || (y1 >= max_y && y2 >= max_y))
                return 0;
            m = (y2 - y1) / (x2 - x1);
            y = m * (min_x - x1) + y1;
            if (y > min_y && y < max_y)
                return 1;
            y = m * (max_x - x1) + y1;
            if (y > min_y && y < max_y)
                return 1;
            x = (min_y - y1) / m + x1;
            if (x > min_x && x < max_x)
                return 1;
            x = (max_y - y1) / m + x1;
            if (x > min_x && x < max_x)
                return 1;
            return 0;
        }

        bool SkeletonBounds::AABBIntersectsSkeleton(SkeletonBounds *bounds)
        {
            return min_x < bounds->max_x && max_x > bounds->min_x && min_y < bounds->max_y && max_y > bounds->min_y;
        }

        BoundingBoxAttachment *SkeletonBounds::ContainsPoint(float x, float y)
        {
            int i;

            for (i = 0; i < count; ++i)
                if (polygons[i]->ContainsPoint(x, y))
                    return bounding_boxes[i];

            return nullptr;
        }

        BoundingBoxAttachment *SkeletonBounds::IntersectsSegment(float x1, float y1, float x2, float y2)
        {
            int i;

            for (i = 0; i < count; ++i)
                if (polygons[i]->IntersectsSegment(x1, y1, x2, y2))
                    return bounding_boxes[i];

            return nullptr;
        }

        SpinePolygon *SkeletonBounds::GetPolygon(BoundingBoxAttachment *boundingBox)
        {
            int i;

            for (i = 0; i < count; ++i)
                if (bounding_boxes[i] == boundingBox)
                    return polygons[i];

            return nullptr;
        }
    }
}
