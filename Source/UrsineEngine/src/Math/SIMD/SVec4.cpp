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

#include "UrsinePrecompiled.h"

namespace ursine
{
    SVec4::SVec4(const SVec3 &value, float W)
        : x(value.X())
        , y(value.Y())
        , z(value.Z())
        , w(W) { }

	SVec4::SVec4(const Vec3 &value, float W)
		: x(value.X())
		, y(value.Y())
		, z(value.Z())
		, w(W) { }

	SVec4::SVec4(const Vec4 &value)
		: x(value.X())
		, y(value.Y())
		, z(value.Z())
		, w(value.w) { }

    // Public Methods
    void SVec4::Clean(void)
    {
        x = math::IsZero(x) ? 0.0f : x;
        y = math::IsZero(y) ? 0.0f : y;
        z = math::IsZero(z) ? 0.0f : z;
        w = math::IsZero(w) ? 0.0f : w;
    }

    std::string SVec4::ToString(void) const
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
