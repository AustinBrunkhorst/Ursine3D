#include "TweenProperty.h"

namespace ursine
{
    template<class Type>
    TweenID &TweenID::Property(Type &input,
        Type target,
        const TimeSpan &duration,
        Ease::Function ease)
    {
        Type start = input;
        auto input_ptr = &input;

        doProperty(duration, [=](float percent) mutable 
        {
            *input_ptr = Math::Lerp(start, target, ease(percent));
        });

        return *this;
    }

    template<typename SetterFN, class Type>
    TweenID &TweenID::Setter(SetterFN setter,
        Type start,
        Type target,
        const TimeSpan &duration,
        Ease::Function ease)
    {
        doProperty(duration, [=](float percent) mutable 
        {
            setter(Math::Lerp(start, target, ease(percent)));
        });

        return *this;
    }

    template<class ClassType, typename SetterFN, class Type>
    TweenID &TweenID::Setter(ClassType *object,
        SetterFN setter,
        Type start,
        Type target,
        const TimeSpan &duration,
        Ease::Function ease)
    {
        auto functor = std::bind(setter, object, std::placeholders::_1);

        doProperty(duration, [=](float percent) mutable
        {
            functor(Math::Lerp(start, target, ease(percent)));
        });

        return *this;
    }

    template<class ClassType, typename CallbackFN, typename... Args>
    TweenID &TweenID::Call(ClassType *object, CallbackFN callback, Args &&... args)
    {
        return Call(std::bind(callback, object, std::forward<Args>(args)...));
    }
}