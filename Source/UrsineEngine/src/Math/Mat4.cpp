/* ---------------------------------------------------------------------------
** Team Bear King
** DigiPen Institute of Technology 2015
**
** Mat4.cpp
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
    // Constructors
	Mat4::Mat4(const SMat4 &other)
	{
		memcpy(m, other.m, sizeof(m));
	}

    Mat4::Mat4(const Mat3 &mat)
    {
        Set(
            mat.m[0][0], mat.m[0][1], mat.m[0][2], 0.0f,
            mat.m[1][0], mat.m[1][1], mat.m[1][2], 0.0f,
            mat.m[2][0], mat.m[2][1], mat.m[2][2], 0.0f,
            0.0f,    0.0f,    0.0f,    1.0f
        );
    }

	Mat4::Mat4(const SMat3 &mat)
	{
		Set(
			mat.m[0][0], mat.m[0][1], mat.m[0][2], 0.0f,
			mat.m[1][0], mat.m[1][1], mat.m[1][2], 0.0f,
			mat.m[2][0], mat.m[2][1], mat.m[2][2], 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
			);
	}

    // Public Methods
    std::string Mat4::ToString(void) const
    {
        std::ostringstream M00, M01, M02, M03,
                           M10, M11, M12, M13,
                           M20, M21, M22, M23,
                           M30, M31, M32, M33;

        M00 << m[0][0]; M01 << m[0][1]; M02 << m[0][2]; M03 << m[0][3];
        M10 << m[1][0]; M11 << m[1][1]; M12 << m[1][2]; M13 << m[1][3];
        M20 << m[2][0]; M21 << m[2][1]; M22 << m[2][2]; M23 << m[2][3];
        M30 << m[3][0]; M31 << m[3][1]; M32 << m[3][2]; M33 << m[3][3];

        return {
            "{" +
                M00.str() + ", " +
                M01.str() + ", " +
                M02.str() + ", " +
                M03.str() +
            "}\n" +
            "{" +
                M10.str() + ", " +
                M11.str() + ", " +
                M12.str() + ", " +
                M13.str() +
            "}\n" +
            "{" +
                M20.str() + ", " +
                M21.str() + ", " +
                M22.str() + ", " +
                M23.str() +
            "}\n" +
            "{" +
                M30.str() + ", " +
                M31.str() + ", " +
                M32.str() + ", " +
                M33.str() +
            "}\n"
        };
    }
}