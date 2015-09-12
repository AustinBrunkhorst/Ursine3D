#include "TweenProperty.h"

namespace ursine
{
    template<class Type>
    TweenID &TweenID::Property(Type &input,
        Type target,
        const TimeSpan &duration,
        ease::Function ease)
    {
        Type start = input;
        auto *pInput = &input;

        doProperty( duration, [=](float percent) mutable
        {
            **pInput = math::Lerp( start, target, ease( percent ) );
        } );

        return *this;
    }

    template<typename SetterFN, class Type>
    TweenID &TweenID::Setter(SetterFN setter,
        Type start,
        Type target,
        const TimeSpan &duration,
        ease::Function ease)
    {
        doProperty( duration, [=](float percent) mutable
        {
            setter( math::Lerp( start, target, ease( percent ) ) );
        } );

        return *this;
    }

    template<class ClassType, typename SetterFN, class Type>
    TweenID &TweenID::Setter(ClassType *object,
        SetterFN setter,
        Type start,
        Type target,
        const TimeSpan &duration,
        ease::Function ease)
    {
        auto functor = std::bind( setter, object, std::placeholders::_1 );

        doProperty( duration, [=](float percent) mutable
        {
            functor( math::Lerp( start, target, ease( percent ) ) );
        } );

        return *this;
    }

    template<class ClassType, typename CallbackFN, typename... Args>
    TweenID &TweenID::Call(ClassType *object, CallbackFN callback, Args &&... args)
    {
        return Call( std::bind( callback, object, std::forward<Args>( args )... ) );
    }
}
