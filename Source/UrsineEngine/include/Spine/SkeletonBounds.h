#pragma once

#include "Spine/Attachments/BoundingBoxAttachment.h"
#include "Spine/Skeleton.h"
#include "Spine/SpinePolygon.h"

namespace Ursine
{
    namespace Spine
    {
        class SkeletonBounds
        {
            int count;
            BoundingBoxAttachment **bounding_boxes;
            SpinePolygon **polygons;

            int capacity;

            float min_x;
            float min_y;
            float max_x;
            float max_y;

        public:
            SkeletonBounds(void);
            ~SkeletonBounds(void);

            void Update(Skeleton *skeleton, bool update_aabb);

            bool AABBContainsPoint(float x, float y);

            bool AABBIntersectsSegment(float x1, float y1, float x2, float y2);

            bool AABBIntersectsSkeleton(SkeletonBounds *bounds);

            BoundingBoxAttachment *ContainsPoint(float x, float y);

            BoundingBoxAttachment *IntersectsSegment(float x1, float y1, float x2, float y2);

            SpinePolygon *GetPolygon(BoundingBoxAttachment *boundingBox);
        };
    }
}
