/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Ease.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

namespace ursine
{
    namespace ease
    {
        typedef float(*Function)(float t);

        enum class Type
        {
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
            BounceInOut,
            EASE_COUNT
        };
        
        Function GetFunction(Type type);
        Type GetType(Function function);

        float Linear(float t);

        float QuadraticIn(float t);
        float QuadraticOut(float t);
        float QuadraticInOut(float t);

        float CubicIn(float t);
        float CubicOut(float t);
        float CubicInOut(float t);

        float QuarticIn(float t);
        float QuarticOut(float t);
        float QuarticInOut(float t);

        float QuinticIn(float t);
        float QuinticOut(float t);
        float QuinticInOut(float t);

        float SineIn(float t);
        float SineOut(float t);
        float SineInOut(float t);

        float CircularIn(float t);
        float CircularOut(float t);
        float CircularInOut(float t);

        float ExponentialIn(float t);
        float ExponentialOut(float t);
        float ExponentialInOut(float t);

        float ElasticIn(float t);
        float ElasticOut(float t);
        float ElasticInOut(float t);

        float BackIn(float t);
        float BackOut(float t);
        float BackInOut(float t);

        float BounceIn(float t);
        float BounceOut(float t);
        float BounceInOut(float t);
    }

    extern template
    Json JsonSerializer::Serialize(ease::Function &instance);

    extern template
    void JsonSerializer::Deserialize(const Json &data, ease::Function &out);
}