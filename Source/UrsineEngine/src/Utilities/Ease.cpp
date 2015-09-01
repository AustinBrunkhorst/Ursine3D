/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Ease.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "Ease.h"

namespace ursine
{
    namespace ease
    {
        Function GetFunction(Type type)
        {
            static const Function functions[ static_cast<int>( Type::EASE_COUNT ) ] = {
                Linear,
                QuadraticIn,
                QuadraticOut,
                QuadraticInOut,
                CubicIn,
                CubicOut,
                CubicInOut,
                QuarticIn,
                QuarticOut,
                QuarticInOut,
                QuinticIn,
                QuinticOut,
                QuinticInOut,
                SineIn,
                SineOut,
                SineInOut,
                CircularIn,
                CircularOut,
                CircularInOut,
                ExponentialIn,
                ExponentialOut,
                ExponentialInOut,
                ElasticIn,
                ElasticOut,
                ElasticInOut,
                BackIn,
                BackOut,
                BackInOut,
                BounceIn,
                BounceOut,
                BounceInOut
            };

            return functions[ static_cast<int>( type ) ];
        }

        Type GetType(Function function)
        {
            #define map_type(type) if (function == type) return Type::type

            map_type(Linear);
            map_type(QuadraticIn);
            map_type(QuadraticOut);
            map_type(QuadraticInOut);
            map_type(CubicIn);
            map_type(CubicOut);
            map_type(CubicInOut);
            map_type(QuarticIn);
            map_type(QuarticOut);
            map_type(QuarticInOut);
            map_type(QuinticIn);
            map_type(QuinticOut);
            map_type(QuinticInOut);
            map_type(SineIn);
            map_type(SineOut);
            map_type(SineInOut);
            map_type(CircularIn);
            map_type(CircularOut);
            map_type(CircularInOut);
            map_type(ExponentialIn);
            map_type(ExponentialOut);
            map_type(ExponentialInOut);
            map_type(ElasticIn);
            map_type(ElasticOut);
            map_type(ElasticInOut);
            map_type(BackIn);
            map_type(BackOut);
            map_type(BackInOut);
            map_type(BounceIn);
            map_type(BounceOut);
            map_type(BounceInOut);

            #undef map_type

            return Type::EASE_COUNT;
        }

        float Linear(float t)
        {
            return t;
        }

        float QuadraticIn(float t)
        {
            return t * t;
        }

        float QuadraticOut(float t)
        {
            return -(t * (t - 2.0f));
        }

        float QuadraticInOut(float t)
        {
            if (t < 0.5f)
                return 2.0f * t * t;

            return (-2.0f * t * t) + (4.0f * t) - 1.0f;
        }

        float CubicIn(float t)
        {
            return t * t * t;
        }

        float CubicOut(float t)
        {
            float f = (t - 1.0f);

            return f * f * f + 1.0f;
        }

        float CubicInOut(float t)
        {
            if (t < 0.5f)
                return 4 * t * t * t;

            float f = ((2.0f * t) - 2.0f);

            return 0.5f * f * f * f + 1.0f;
        }

        float QuarticIn(float t)
        {
            return t * t * t * t;
        }

        float QuarticOut(float t)
        {
            float f = (t - 1.0f);

            return f * f * f * (1.0f - t) + 1.0f;
        }

        float QuarticInOut(float t)
        {
            if (t < 0.5f)
                return 8.0f * t * t * t * t;

            float f = (t - 1.0f);

            return -8.0f * f * f * f * f + 1.0f;
        }

        float QuinticIn(float t)
        {
            return t * t * t * t * t;
        }

        float QuinticOut(float t)
        {
            float f = (t - 1.0f);

            return f * f * f * f * f + 1.0f;
        }

        float QuinticInOut(float t)
        {
            if (t < 0.5f)
                return 16.0f * t * t * t * t * t;

            float f = ((2.0f * t) - 2.0f);

            return 0.5f * f * f * f * f * f + 1.0f;
        }

        float SineIn(float t)
        {
            return sinf((t - 1.0f) * math::PI_DIV_2) + 1.0f;
        }

        float SineOut(float t)
        {
            return sinf(t * math::PI_DIV_2);
        }

        float SineInOut(float t)
        {
            return 0.5f * (1.0f - cosf(t * math::PI));
        }

        float CircularIn(float t)
        {
            return 1.0f - sqrtf(1.0f - (t * t));
        }

        float CircularOut(float t)
        {
            return sqrtf((2.0f - t) * t);
        }

        float CircularInOut(float t)
        {
            if (t < 0.5f)
                return 0.5f * (1.0f - sqrtf(1.0f - 4.0f * (t * t)));

            return 0.5f * (sqrtf(-((2.0f * t) - 3.0f) * ((2.0f * t) - 1.0f)) + 1.0f);
        }

        float ExponentialIn(float t)
        {
            return (t == 0.0f) ? t : powf(2.0f, 10.0f * (t - 1.0f));
        }

        float ExponentialOut(float t)
        {
            return (t == 1.0) ? t : 1.0f - powf(2.0f, -10.0f * t);
        }

        float ExponentialInOut(float t)
        {
            if (t == 0.0f || t == 1.0)
                return t;

            if (t < 0.5f)
                return 0.5f * powf(2.0f, (20.0f * t) - 10.0f);

            return -0.5f * powf(2.0f, (-20.0f * t) + 10.0f) + 1.0f;
        }

        float ElasticIn(float t)
        {
            return sinf(13.0f * math::PI_DIV_2 * t) * powf(2.0f, 10.0f * (t - 1.0f));
        }

        float ElasticOut(float t)
        {
            return sinf(-13.0f * math::PI_DIV_2 * (t + 1.0f)) * powf(2.0f, -10.0f * t) + 1.0f;
        }

        float ElasticInOut(float t)
        {
            if (t < 0.5f)
                return 0.5f * sinf(13.0f * math::PI_DIV_2 * (2.0f * t)) * powf(2.0f, 10.0f * ((2.0f * t) - 1.0f));

            return 0.5f * (sinf(-13.0f * math::PI_DIV_2 * ((2.0f * t - 1.0f) + 1.0f)) * powf(2.0f, -10.0f * (2.0f * t - 1.0f)) + 2.0f);
        }

        float BackIn(float t)
        {
            return t * t * t - t * sinf(t * math::PI);
        }

        float BackOut(float t)
        {
            float f = (1.0f - t);

            return 1.0f - (f * f * f - f * sinf(f * math::PI));
        }

        float BackInOut(float t)
        {
            if (t < 0.5f)
            {
                float f = 2.0f * t;

                return 0.5f * (f * f * f - f * sinf(f * math::PI));
            }

            float f = (1.0f - (2.0f * t - 1.0f));

            return 0.5f * (1.0f - (f * f * f - f * sinf(f * math::PI))) + 0.5f;
        }

        float BounceIn(float t)
        {
            return 1.0f - BounceOut(1.0f - t);
        }

        float BounceOut(float t)
        {
            if (t < 4.0f / 11.0f)
                return (121.0f * t * t) / 16.0f;

            if (t < 8.0f / 11.0f)
                return (363.0f / 40.0f * t * t) - (99.0f / 10.0f * t) + 17.0f / 5.0f;

            if (t < 9.0f / 10.0f)
                return (4356.0f / 361.0f * t * t) - (35442.0f / 1805.0f * t) + 16061.0f / 1805.0f;

            return (54.0f / 5.0f * t * t) - (513.0f / 25.0f * t) + 268.0f / 25.0f;
        }

        float BounceInOut(float t)
        {
            if (t < 0.5f)
                return 0.5f * BounceIn(t * 2.0f);

            return 0.5f * BounceIn(t * 2.0f - 1.0f) + 0.5f;
        }
    }

    template<>
    Json JsonSerializer::Serialize(ease::Function &instance)
    {
        return Json( static_cast<int>( ease::GetType( instance ) ) );
    }

    template<>
    void JsonSerializer::Deserialize(const Json &data, ease::Function &out)
    {
        out = ease::GetFunction( static_cast<ease::Type>( data.int_value( ) ) );
    }
}