/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AABB.cpp
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

namespace Ursine
{
    AABB::AABB(void)
    {
            
    }

    AABB::AABB(const Vector2 &low_bound, const Vector2 &up_bound)
        : low_bound(low_bound)
        , up_bound(up_bound)
    {
        
    }

    AABB::~AABB(void)
    {
            
    }

    AABB::AABB(const AABB &other)
        : low_bound(other.low_bound)
        , up_bound(other.up_bound)
    {

    }

    // From Real-Time CollisionDetection, p179.
    bool AABB::RayCast(RayCastOutput &output, const RayCastInput &input)
    {
        Vector2 tl = { low_bound.x, up_bound.y };
        Vector2 br = { up_bound.x, low_bound.y };
        Vector2 norm;
        float t = Math::MAX_FLOAT;
        float temp;

        // top side
        if (Collision::RaySegmentIntersection(input, tl, up_bound, temp))
        {
            if (temp < t)
            {
                t = temp;
                norm = { 0.0f, 1.0f };
            }
        }

        // bottom side
        if (Collision::RaySegmentIntersection(input, br, low_bound, temp))
        {
            if (temp < t)
            {
                t = temp;
                norm = { 0.0f, -1.0f };
            }
        }

        // left side
        if (Collision::RaySegmentIntersection(input, tl, low_bound, temp))
        {
            if (temp < t)
            {
                t = temp;
                norm = { -1.0f, 0.0f };
            }
        }

        // right side
        if (Collision::RaySegmentIntersection(input, br, up_bound, temp))
        {
            if (temp < t)
            {
                t = temp;
                norm = { 1.0f, 0.0f };
            }
        }

        if (norm != Vector2())
        {
            output.normal = norm;
            output.fraction = t;
            return true;
        }
        else
            return false;
    }
}