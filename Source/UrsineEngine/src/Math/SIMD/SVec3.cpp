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

namespace ursine
{
    SVec3::SVec3(const Vec2 &value, float Z)
            : x(value.X())
            , y(value.Y())
            , z(Z)
    {
#ifdef USE_SSE
        w = 0.0f;
#endif
    }

	SVec3::SVec3(const Vec3 &value)
		: x(value.X())
		, y(value.Y())
		, z(value.Z())
	{
#ifdef USE_SSE
		w = 0.0f;
#endif
	}

    SVec3::SVec3(const SVec4 &value)
            : x(value.X())
            , y(value.Y())
            , z(value.Z())
    {
#ifdef USE_SSE
        w = 0.0f;
#endif
    }

	SVec3::SVec3(const Vec4 &value)
		: x(value.X())
		, y(value.Y())
		, z(value.Z())
	{
#ifdef USE_SSE
		w = 0.0f;
#endif
	}

    // Public Methods
    void SVec3::Clean(void)
    {
        x = math::IsZero(x) ? 0.0f : x;
        y = math::IsZero(y) ? 0.0f : y;
        z = math::IsZero(z) ? 0.0f : z;
    }

    std::string SVec3::ToString(void) const
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
