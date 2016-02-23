/* ---------------------------------------------------------------------------
** Team Bear King
** DigiPen Institute of Technology 2014
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

#include <limits>

namespace ursine
{
    class SQuat;

    namespace math
    {
        const float Epsilon = 0.0001f;
        const float PI = 3.14159265f;
        const float PI_DIV_2 = PI / 2.0f;
        const float PI_2 = PI * 2;
        const float PI_SQR = PI * PI;
        const float SQRT_2 = 1.4142f;

        const float MAX_FLOAT = std::numeric_limits<float>::max();

        template<typename T>
        inline bool IsZero(T value);

        template<typename T1, typename T2>
        inline bool IsEqual(T1 a, T2 b, float epsion = math::Epsilon);

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

        inline float RadiansToDegrees(float radians);

        inline float DegreesToRadians(float degrees);

        void SinCos(float radians, float &sin, float &cos);

        extern template
        SQuat Lerp(const SQuat &a, const SQuat &b, float percent);
    }
}

#include "UrsineMath.hpp"