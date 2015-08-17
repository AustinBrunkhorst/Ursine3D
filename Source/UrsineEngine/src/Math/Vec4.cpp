/* ---------------------------------------------------------------------------
** Team Bear King
** DigiPen Institute of Technology 2015
**
** Vec4.cpp
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "Vec3.h"
#include "Vec4.h"
#include <sstream>

namespace Ursine
{
    Vec4::Vec4(const Vec3 &value, float W)
        : x(value.x)
        , y(value.y)
        , z(value.z)
        , w(W) { }

    // Public Methods
    void Vec4::Clean(void)
    {
        x = Math::IsZero(x) ? 0.0f : x;
        y = Math::IsZero(y) ? 0.0f : y;
        z = Math::IsZero(z) ? 0.0f : z;
        w = Math::IsZero(w) ? 0.0f : w;
    }

    std::string Vec4::ToString(void) const
    {
        std::ostringstream x_cvt;
        x_cvt << x;

        std::ostringstream y_cvt;
        y_cvt << y;

        std::ostringstream z_cvt;
        z_cvt << z;

        std::ostringstream w_cvt;
        w_cvt << w;

        return{ "{" +
                x_cvt.str() + ", " +
                y_cvt.str() + ", " +
                z_cvt.str() + ", " +
                w_cvt.str() +
				"}" };
    }
}
