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

#include "Vec2.h"
#include "Vec3.h"
#include <sstream>

namespace Ursine
{
    Vec2::Vec2(const Vec3 &value)
        : x(value.x)
        , y(value.y) { }

    // Public Methods
    void Vec2::Clean(void)
    {
        x = Math::IsZero(x) ? 0.0f : x;
        y = Math::IsZero(y) ? 0.0f : y;
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