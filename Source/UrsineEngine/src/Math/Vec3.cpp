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

#include "UrsinePrecompiled.h"

namespace Ursine
{
	Vec3::Vec3(const SVec3 &value)
		: x(value.X())
		, y(value.Y())
		, z(value.Z())
	{ }

    Vec3::Vec3(const Vec2 &value, float Z)
            : x(value.X())
            , y(value.Y())
            , z(Z)
    { }

    Vec3::Vec3(const Vec4 &value)
            : x(value.X())
            , y(value.Y())
            , z(value.Z())
    { }

	Vec3::Vec3(const SVec4 &value)
			: x(value.X())
			, y(value.Y())
			, z(value.Z())
	{ }

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
