/* ---------------------------------------------------------------------------
** Team Bear King
** DigiPen Institute of Technology 2015
**
** Vec3.cpp
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "Vec3.h"
#include "Vec2.h"
#include "Vec4.h"
#include "UrsineMath.h"
#include <sstream>

namespace Ursine
{
    Vec3::Vec3(const Vec2 &value, float Z)
            : x(value.x)
            , y(value.y)
            , z(Z)
    {
#ifdef USE_SSE
        w = 0.0f;
#endif
    }

    Vec3::Vec3(const Vec4 &value)
            : x(value.x)
            , y(value.y)
            , z(value.z)
    {
#ifdef USE_SSE
        w = 0.0f;
#endif
    }

    // Public Methods
    void Vec3::Clean(void)
    {
        x = Math::IsZero(x) ? 0.0f : x;
        y = Math::IsZero(y) ? 0.0f : y;
        z = Math::IsZero(z) ? 0.0f : z;
    }

    std::string Vec3::ToString(void) const
    {
        std::ostringstream x_cvt;
        x_cvt << x;

        std::ostringstream y_cvt;
        y_cvt << y;

        std::ostringstream z_cvt;
        z_cvt << z;

        return{ "{" +
                x_cvt.str() + ", " +
                y_cvt.str() + ", " +
                z_cvt.str() +
                "}" };
    }
}
