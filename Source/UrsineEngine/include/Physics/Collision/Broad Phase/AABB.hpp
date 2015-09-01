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

namespace ursine
{
    inline Vec2 AABB::GetSize(void) const
    {
        return up_bound - low_bound;
    }

    inline Vec2 AABB::GetCenter(void) const
    {
        return low_bound + (GetSize() * 0.5f);
    }

    inline float AABB::GetPerimeter() const
    {
        float x = up_bound.X() - low_bound.X();
        float y = up_bound.Y() - low_bound.Y();
        return 2.0f * (x + y);
    }

    inline void AABB::Combine(const AABB &other)
    {
        low_bound = Vec2(math::Min(low_bound.X(), other.low_bound.X())
                          , math::Min(low_bound.Y(), other.low_bound.Y()));
        up_bound = Vec2(math::Max(up_bound.X(), other.up_bound.X())
                          , math::Max(up_bound.Y(), other.up_bound.Y()));
    }

    inline void AABB::Combine(const AABB &one, const AABB &two)
    {
        low_bound = Vec2(math::Min(one.low_bound.X(), two.low_bound.X())
                          , math::Min(one.low_bound.Y(), two.low_bound.Y()));
        up_bound = Vec2(math::Max(one.up_bound.X(), two.up_bound.X())
                          , math::Max(one.up_bound.Y(), two.up_bound.Y()));
    }

    inline bool AABB::Contains(const AABB &other) const
    {
        bool result = true;
        result = result && low_bound.X() <= other.low_bound.X();
        result = result && low_bound.Y() <= other.low_bound.Y();
        result = result && other.up_bound.X() <= up_bound.X();
        result = result && other.up_bound.Y() <= up_bound.Y();
        return result;
    }

    inline bool AABB::Overlap(const AABB &other) const
    {
        Vec2 d1, d2;

        d1 = other.low_bound - up_bound;
        d2 = low_bound - other.up_bound;

        if (d1.X() > 0.0f || d1.Y() > 0.0f)
            return false;

        if (d2.X() > 0.0f || d2.Y() > 0.0f)
            return false;

        return true;
    }
}