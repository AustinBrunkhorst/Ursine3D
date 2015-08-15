/* ---------------------------------------------------------------------------
** Team Bear King
** � 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** UrsineMath.hpp
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "UrsineLogTools.h"

#include <math.h>
#include <exception>

namespace Ursine
{
    namespace Math
    {
        template<typename T>
        inline bool IsZero(T value)
        {
            return (value < Math::Epsilon) && (value > -Math::Epsilon);
        }

        template<typename T1, typename T2>
        inline bool IsEqual(T1 a, T2 b)
        {
            return ((a >= b) ? (a - b) : (b - a)) < Math::Epsilon;
        }

        // for use on doubles and floats
        // http://www.johndcook.com/blog/ieee_exceptions_in_cpp/
        template<typename T>
        inline bool IsFiniteNumber(T x)
        {
            return (x <= std::numeric_limits<T>::max() &&
                x >= -std::numeric_limits<T>::max());
        }

        template<typename T>
        inline T Clamp(T value, T min, T max)
        {
            if (!IsFiniteNumber(value))
                if (value < 0)
                    value = min;
                else
                    value = max;

            return (value > max) ? max : ((value < min) ? min : value);
        }

        template<typename T>
        inline T Max(T a, T b)
        {
            return (a > b) ? a : b;
        }

        template<typename T>
        inline T Min(T a, T b)
        {
            return (a < b) ? a : b;
        }

        inline float Wrap(float in_val, float min, float max)
        {
            float range = max - min;
            float val = fmod(in_val, range);

            // fmod doesn't function like normal % on negative numbers
            // this forces the same behavior.  Talk to Jordan for further
            // explanation.
            if (val < 0)
                val += range;

            val += min;

            return val;
        }

        template<typename T>
        T Lerp(const T &a, const T &b, float percent)
        {
            return a + (b - a) * percent;
        }

        template<typename T>
        T Rand(T min, T max)
        {
            return min + rand() / static_cast<T>(RAND_MAX / (max - min));
        }

        inline float fastSqrt(float val)
        {
            UAssert(val >= 0.0f, "sqrt of negative number.");

            if (val == 0.0f)
                return 0.0f;

            union
            {
                float f;
                int i;
            } workval;

            workval.f = val;
            workval.i -= 0x3f800000; // subtract 127 from biased exponent
            workval.i >>= 1; // requires signed shift to preserve sign
            workval.i += 0x3f800000; // rebias the new exponent
            workval.i &= 0x7FFFFFFF; // remove sign bit

            return workval.f;
        }

        inline float RadiansToDegrees(float radians)
        {
            return radians * 180 / PI;
        }

        inline float DegreesToRadians(float degrees)
        {
            return degrees * PI / 180;
        }
    }
}