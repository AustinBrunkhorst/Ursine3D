/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AABB.h
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "Vec2.h"
#include "RayCast.h"

namespace Ursine
{
    #define AABB_EXTENSION 0.1f
    #define AABB_MULTIPLIER 2.0f

    class AABB
    {
    public:
        AABB(void);
        AABB(const Vec2 &low_bound, const Vec2 &up_bound);
        AABB(const AABB &other);

        ~AABB(void);

        inline Vec2 GetSize(void) const;

        inline Vec2 GetCenter(void) const;

        inline float GetPerimeter(void) const;

        // Combine an AABB with this one
        inline void Combine(const AABB &other);
            
        // Combine both AABBs into this
        inline void Combine(const AABB &one, const AABB &two);

        // does this box contain the provided aabb
        inline bool Contains(const AABB &other) const;

        // does this box overlap with the provided aabb
        inline bool Overlap(const AABB &other) const;

        bool RayCast(RayCastOutput &output, const RayCastInput &input);

        // lower vertex
        Vec2 low_bound;
        // upper vertex
        Vec2 up_bound;
    };
}

#include "AABB.hpp"
