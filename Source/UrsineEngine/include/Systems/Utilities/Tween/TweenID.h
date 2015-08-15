#pragma once

#include "TweenConfig.h"
#include "TweenGrouped.h"

#include "TimeSpan.h"

#include "Ease.h"

namespace Ursine
{
    class TweenID
    {
        friend class TweenManager;
        friend class LocalTweenManager;

        friend class TweenGrouped;

        // id of the tween in the manager
        uint32 _id;

        TweenID(uint32 id);

        TweenID &Removed(TweenCallback callback);

        void doProperty(const TimeSpan &duration, 
            const TweenPercentageCallback &property_fn);
    public:
        TweenID(void);

        // Determines if the tween representing this ID is active (in the TweenManager)
        bool IsActive(void);

        // Sets a tween as paused
        void Pause(void);

        // Sets a tween as not paused
        void Resume(void);

        // Cancels the tween (removes it from the TweenManager).
        void Cancel(void) const;

        // Determines if the tween is paused
        bool IsPaused(void) const;

        // Removes all tween items currently running in this tween
        TweenID &Stop(void);

        template<class Type>
        TweenID &Property(Type &input,
            Type target, 
            const TimeSpan &duration, 
            Ease::Function ease = Ease::Linear);

        template<typename SetterFN, class Type>
        TweenID &Setter(SetterFN setter,
            Type start,
            Type target,
            const TimeSpan &duration,
            Ease::Function ease = Ease::Linear);

        template<class ClassType, typename SetterFN, class Type>
        TweenID &Setter(ClassType *object,
            SetterFN setter,
            Type start,
            Type target,
            const TimeSpan &duration,
            Ease::Function ease = Ease::Linear);

        // Creates a delay in the tween
        TweenID &Delay(const TimeSpan &duration);

        // Invokes a callback in the tween
        TweenID &Call(TweenCallback callback);

        // Invokes a member function callback in the tween with 
        // optional bound arguments
        template<class ClassType, typename CallbackFN, typename... Args>
        TweenID &Call(ClassType *object, CallbackFN callback, Args &&...);

        TweenGrouped &BeginGroup(void) const;

        TweenItem *GetCurrentItem(void) const;
    };
}

#include "TweenID.hpp"