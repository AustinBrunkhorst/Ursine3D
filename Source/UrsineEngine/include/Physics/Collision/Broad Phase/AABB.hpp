/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AABB.hpp
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

namespace Ursine
{
    inline Vector2 AABB::GetSize(void) const
    {
        return up_bound - low_bound;
    }

    inline Vector2 AABB::GetCenter(void) const
    {
        return low_bound + (GetSize() * 0.5f);
    }

    inline float AABB::GetPerimeter() const
    {
        float x = up_bound.x - low_bound.x;
        float y = up_bound.y - low_bound.y;
        return 2.0f * (x + y);
    }

    inline void AABB::Combine(const AABB &other)
    {
        low_bound = Vector2(Math::Min(low_bound.x, other.low_bound.x)
                          , Math::Min(low_bound.y, other.low_bound.y));
        up_bound = Vector2(Math::Max(up_bound.x, other.up_bound.x)
                          , Math::Max(up_bound.y, other.up_bound.y));
    }

    inline void AABB::Combine(const AABB &one, const AABB &two)
    {
        low_bound = Vector2(Math::Min(one.low_bound.x, two.low_bound.x)
                          , Math::Min(one.low_bound.y, two.low_bound.y));
        up_bound = Vector2(Math::Max(one.up_bound.x, two.up_bound.x)
                          , Math::Max(one.up_bound.y, two.up_bound.y));
    }

    inline bool AABB::Contains(const AABB &other) const
    {
        bool result = true;
        result = result && low_bound.x <= other.low_bound.x;
        result = result && low_bound.y <= other.low_bound.y;
        result = result && other.up_bound.x <= up_bound.x;
        result = result && other.up_bound.y <= up_bound.y;
        return result;
    }

    inline bool AABB::Overlap(const AABB &other) const
    {
        Vector2 d1, d2;

        d1 = other.low_bound - up_bound;
        d2 = low_bound - other.up_bound;

        if (d1.x > 0.0f || d1.y > 0.0f)
            return false;

        if (d2.x > 0.0f || d2.y > 0.0f)
            return false;

        return true;
    }
}