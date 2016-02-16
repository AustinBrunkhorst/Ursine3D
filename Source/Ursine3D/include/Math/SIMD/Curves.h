/* ---------------------------------------------------------------------------
** Team Bear King
** DigiPen Institute of Technology 2015
**
** Curves.h
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "SIMD.h"

#include "SVec3.h"

namespace ursine
{
    class Curves
    {
    public:
        static SVec3 CatmullRomSpline(
            const SVec3 &p0, const SVec3 &p1, 
            const SVec3 &p2, const SVec3 &p3, float t
        );
    };
}

#include "Curves.hpp"
