#include "UrsinePrecompiled.h"

#include "Spine/spine.h"

namespace Ursine
{
    namespace Spine
    {
        SpinePolygon::SpinePolygon(int capacity)
            : capacity(capacity)
            , vertices(new float [capacity])
        {

        }

        SpinePolygon::~SpinePolygon(void)
        {
            delete []vertices;
        }

        bool SpinePolygon::ContainsPoint(float x, float y)
        {
            int prev_index = count - 2;

            bool inside = 0;

            int i;

            for (i = 0; i < count; i += 2)
            {
                float vertex_y = vertices[i + 1];
                float prev_y = vertices[prev_index + 1];

                if ((vertex_y < y && prev_y >= y) || (prev_y < y && vertex_y >= y))
                {
                    float vertex_x = vertices[i];

                    if (vertex_x + (y - vertex_y) / (prev_y - vertex_y) * (vertices[prev_index] - vertex_x) < x)
                        inside = !inside;
                }

                prev_index = i;
            }

            return inside;
        }

        bool SpinePolygon::IntersectsSegment(float x1, float y1, float x2, float y2)
        {
            float width12 = x1 - x2, height12 = y1 - y2;

            float det1 = x1 * y2 - y1 * x2;

            float x3 = vertices[count - 2], y3 = vertices[count - 1];

            int i;

            for (i = 0; i < count; i += 2)
            {
                float x4 = vertices[i], y4 = vertices[i + 1];

                float det2 = x3 * y4 - y3 * x4;
                float width34 = x3 - x4, height34 = y3 - y4;
                float det3 = width12 * height34 - height12 * width34;
                float x = (det1 * width34 - width12 * det2) / det3;

                if (((x >= x3 && x <= x4) || (x >= x4 && x <= x3)) && ((x >= x1 && x <= x2) || (x >= x2 && x <= x1)))
                {
                    float y = (det1 * height34 - height12 * det2) / det3;

                    if (((y >= y3 && y <= y4) || (y >= y4 && y <= y3)) && ((y >= y1 && y <= y2) || (y >= y2 && y <= y1)))
                        return 1;
                }

                x3 = x4;
                y3 = y4;
            }

            return 0;
        }
    }
}