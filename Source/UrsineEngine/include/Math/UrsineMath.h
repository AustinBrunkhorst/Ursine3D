/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** UrsineMath.h
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
** -------------------------------------------------------------------------*/

#pragma once

namespace Ursine
{
    namespace Math
    {
        const double Epsilon = 0.0001;
        const float PI = 3.14159265f;
        const float PI_DIV_2 = PI / 2.0f;
        const float PI_2 = PI * 2;
        const float PI_SQR = PI * PI;
        const float SQRT_2 = 1.4142f;

        const float MAX_FLOAT = std::numeric_limits<float>::max();

        template<typename T>
        inline bool IsZero(T value);

        template<typename T1, typename T2>
        inline bool IsEqual(T1 a, T2 b);

        template<typename T>
        inline bool IsFiniteNumber(T x);

        template<typename T>
        inline T Clamp(T value, T min, T max);

        template<typename T>
        inline T Max(T a, T b);

        template<typename T>
        inline T Min(T a, T b);

        inline float Wrap(float in_val, float min, float max);

        // Linear interpolates between a and b with the given percentage
        template<typename T>
        inline T Lerp(const T &a, const T &b, float percent);

        template<typename T>
        T Rand(T min, T max);

        // This is not very percise. With large floating point values it is generally 1% off.
        inline float fastSqrt(float val);

        inline float RadiansToDegrees(float radians);

        inline float DegreesToRadians(float degrees);

        void SinCos(float radians, float &sin, float &cos);
    }
}

#include "UrsineMath.hpp"