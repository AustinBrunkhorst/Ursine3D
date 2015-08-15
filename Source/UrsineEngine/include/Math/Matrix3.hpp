/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Matrix3.hpp
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

namespace Ursine
{
    inline void Matrix3::SetWorldToCamera(float width, float height,
                                          float rotation, const Vector2 &translation)
    {
        float c, s;
        Math::SinCos(-rotation, s, c);

        float c_2 = c * c, s_2 = s * s
            , A = c / (c_2 + s_2)
            , B = s / (c_2 + s_2);

        SetColumns(
            Vector3(A / width, -B / height, 0),
            Vector3(B / width, A / height, 0),
            Vector3(((-A * translation.x) - B * translation.y) / width,
                    (B * translation.x - A * translation.y) / height, 1));
    }
}