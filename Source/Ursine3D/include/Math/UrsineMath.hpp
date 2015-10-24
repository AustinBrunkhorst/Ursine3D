/* ---------------------------------------------------------------------------
** Team Bear King
** DigiPen Institute of Technology 2014
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

#include <stdlib.h>

namespace ursine
{
    namespace math
    {
        template<typename T>
        inline bool IsZero(T value)
        {
            return (value <= math::Epsilon) && (value >= -math::Epsilon);
        }

        template<typename T1, typename T2>
        inline bool IsEqual(T1 a, T2 b)
        {
            return ((a >= b) ? (a - b) : (b - a)) < math::Epsilon;
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
            {
                if (value < 0)
                    value = min;
                else
                    value = max;
            }

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

        inline float RadiansToDegrees(float radians)
        {
			static const float scalar = 180.0f / PI;
            return radians * scalar;
        }

        inline float DegreesToRadians(float degrees)
        {
			static const float scalar = PI / 180.0f;
            return degrees * scalar;
        }
    }
}