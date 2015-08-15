/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Quaternion.h
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

namespace Ursine
{
    class Vector3;

    class Quaternion
    {
    public:
        inline Quaternion(void);
        inline Quaternion(float W, float X, float Y, float Z);
        inline Quaternion(const Quaternion &other);
        inline Quaternion(const Vector3 &axis, float angle);
        inline Quaternion(const Vector3 &from, const Vector3 &to);

        const Quaternion &operator=(const Quaternion &rhs);

        void Set(float W, float X, float Y, float Z);

    protected:
        float w, x, y, z;
    };
}

#include "Quaternion.hpp"
