/* ---------------------------------------------------------------------------
** Team Bear King
** DigiPen Institute of Technology 2015
**
** Mat3.cpp
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
    // Constructors
	SMat3::SMat3(const Mat3 &other)
	{
		memcpy(m, other.m, sizeof(m));
	}

    SMat3::SMat3(const SMat4 &mat)
    {
        Set(
            mat.m[0][0], mat.m[0][1], mat.m[0][2],
            mat.m[1][0], mat.m[1][1], mat.m[1][2],
            mat.m[2][0], mat.m[2][1], mat.m[2][2]
        );
    }

	SMat3::SMat3(const Mat4 &mat)
	{
		Set(
			mat.m[0][0], mat.m[0][1], mat.m[0][2],
			mat.m[1][0], mat.m[1][1], mat.m[1][2],
			mat.m[2][0], mat.m[2][1], mat.m[2][2]
			);
	}

	SMat3::SMat3(const SQuat& q)
	{
		// TODO: assume(q.IsNormalized())
		const float x = q.X(); const float y = q.Y(); const float z = q.Z(); const float w = q.W();
		m[0][0] = 1 - 2 * (y*y + z*z); m[0][1] = 2 * (x*y - z*w); m[0][2] = 2 * (x*z + y*w);
		m[1][0] = 2 * (x*y + z*w); m[1][1] = 1 - 2 * (x*x + z*z); m[1][2] = 2 * (y*z - x*w);
		m[2][0] = 2 * (x*z - y*w); m[2][1] = 2 * (y*z + x*w); m[2][2] = 1 - 2 * (x*x + y*y);
	}

	// Public Methods
    std::string SMat3::ToString(void) const
    {
        std::ostringstream M00, M01, M02,
                           M10, M11, M12,
                           M20, M21, M22;

        M00 << m[0][0]; M01 << m[0][1]; M02 << m[0][2];
        M10 << m[1][0]; M11 << m[1][1]; M12 << m[1][2];
        M20 << m[2][0]; M21 << m[2][1]; M22 << m[2][2];

        return { 
            "{" + 
                M00.str() + ", " +
                M01.str() + ", " +
                M02.str() +
            "}\n" +
            "{" +
                M10.str() + ", " +
                M11.str() + ", " +
                M12.str() +
            "}\n" +
            "{" +
                M20.str() + ", " +
                M21.str() + ", " +
                M22.str() +
            "}\n" 
        };
    }
}
