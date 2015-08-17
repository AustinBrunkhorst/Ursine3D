/* ---------------------------------------------------------------------------
** Team Bear King
** DigiPen Institute of Technology 2014
**
** UrsineMath.cpp
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsineMath.h"

namespace Ursine
{
    namespace Math
    {
        void SinCos(float radians, float &sin_value, float &cos_value)
        {
            sin_value = sin(radians);
            cos_value = cos(radians);
        }
    }
}