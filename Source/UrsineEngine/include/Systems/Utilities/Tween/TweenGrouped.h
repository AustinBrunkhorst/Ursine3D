#pragma once

#include "TweenItem.h"

#include "Ease.h"

namespace ursine
{
    class TweenGrouped : public TweenItem
    {
        // can construct groups
        friend class TweenID;

        uint32 _owner;

        // items in the group
        std::vector<TweenItem *> _items;

        explicit TweenGrouped(uint32 owner);

        // disable copy and assign
        TweenGrouped(const TweenGrouped &rhs) = delete;
        TweenGrouped operator=(const TweenGrouped &rhs) = delete;

        bool Update(void) override;

        void doProperty(const TimeSpan &duration,
            const TweenPercentageCallback &property_fn);
    public:
        ~TweenGrouped(void);

        template<class Type>
        TweenGrouped &Property(Type &input,
            Type target,
            const TimeSpan &duration,
            ease::Function ease = ease::Linear);

        template<typename SetterFN, class Type>
        TweenGrouped &Setter(SetterFN setter,
            Type start,
            Type target,
            const TimeSpan &duration,
            ease::Function ease = ease::Linear);

        template<class ClassType, typename SetterFN, class Type>
        TweenGrouped &Setter(ClassType *object,
            SetterFN setter,
            Type start,
            Type target,
            const TimeSpan &duration,
            ease::Function ease = ease::Linear);

        // Creates a delay in the tween
        TweenGrouped &Delay(const TimeSpan &duration);

        // Invokes a callback in the tween
        TweenGrouped &Call(TweenCallback callback);

        // Invokes a member function callback in the tween with 
        // optional bound arguments
        template<class ClassType, typename CallbackFN, typename... Args>
        TweenGrouped &Call(ClassType *object, CallbackFN callback, Args &&...);

        // Ends this group
        TweenID EndGroup(void) const;
    };
}

#include "TweenGrouped.hpp"