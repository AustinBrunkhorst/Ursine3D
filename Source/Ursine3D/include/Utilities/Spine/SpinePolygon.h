#pragma once

namespace ursine
{
    namespace spine
    {
        class SpinePolygon
        {
            friend class SkeletonBounds;

            float * const vertices;
            int count;
            int capacity;

        public:
            SpinePolygon(int capacity);
            ~SpinePolygon(void);

            bool ContainsPoint(float x, float y);
            bool IntersectsSegment(float x1, float y1, float x2, float y2);
        };
    }
}