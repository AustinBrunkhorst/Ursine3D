/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** RayCast.h
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "Vec2.h"
#include "Utils.h"

namespace Ursine
{
    namespace ECS
    {
        class Entity;
    }

    // RayCastInput data.  Extends from p1 to p1 + max_fraction * (p2 - p1).
    class RayCastInput
    {
    public:
        Vec2 p1, p2;
        float max_fraction;
    };

    // RayCastOutput data.  The ray hits at p1 + fraction * (p2 - p1).
    // Normal will be the surface normal of intersection.
    class RayCastOutput
    {
    public:
        RayCastOutput(void)
            : fraction(0.0f)
            , entity(nullptr) { }

        Vec2 normal;
        float fraction;
        ECS::Entity *entity;
    };
}