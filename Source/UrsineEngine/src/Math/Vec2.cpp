/* ---------------------------------------------------------------------------
** Team Bear King
** DigiPen Institute of Technology 2015
**
** Vec2.cpp
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

namespace ursine
{
	Vec2::Vec2(const Vec3 &value)
		: x(value.x)
		, y(value.y) { }

    Vec2::Vec2(const SVec3 &value)
        : x(value.x)
        , y(value.y) { }

    // Public Methods
    void Vec2::Clean(void)
    {
        x = math::IsZero(x) ? 0.0f : x;
        y = math::IsZero(y) ? 0.0f : y;
    }

    std::string Vec2::ToString(void) const
    {
        std::ostringstream x_cvt;
        x_cvt << x;

        std::ostringstream y_cvt;
        y_cvt << y;

        return{ "{" +
                x_cvt.str() + ", " +
                y_cvt.str() +
                "}" };
    }
}