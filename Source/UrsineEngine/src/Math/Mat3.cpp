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

<<<<<<< HEAD
#include "UrsinePrecompiled.h"
=======
#include "Mat3.h"
#include "Mat4.h"

#include <sstream>
>>>>>>> master

namespace Ursine
{
    // Constructors
<<<<<<< HEAD
	Mat3::Mat3(const SMat3 &mat)
	{
		memcpy(m, mat.m, sizeof(m));
	}

=======
>>>>>>> master
    Mat3::Mat3(const Mat4 &mat)
    {
        Set(
            mat.m[0][0], mat.m[0][1], mat.m[0][2],
            mat.m[1][0], mat.m[1][1], mat.m[1][2],
            mat.m[2][0], mat.m[2][1], mat.m[2][2]
        );
    }

<<<<<<< HEAD
	Mat3::Mat3(const SMat4 &mat)
	{
		Set(
			mat.m[0][0], mat.m[0][1], mat.m[0][2],
			mat.m[1][0], mat.m[1][1], mat.m[1][2],
			mat.m[2][0], mat.m[2][1], mat.m[2][2]
		);
	}
    
=======
>>>>>>> master
    // Public Methods
    std::string Mat3::ToString(void) const
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
