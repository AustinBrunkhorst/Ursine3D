/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** TweenID.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "TweenConfig.h"
#include "TweenGrouped.h"

#include "TimeSpan.h"

#include "Ease.h"

namespace ursine
{
    class TweenID
    {
    public:
        TweenID(void);

        // Determines if this represents a valid tween
        bool IsValid(void) const;

        // Determines if the tween representing this ID is active (in the TweenManager)
        bool IsActive(void) const;

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
            ease::Function ease = ease::Linear);

        template<typename SetterFN, class Type>
        TweenID &Setter(SetterFN setter,
            Type start,
            Type target,
            const TimeSpan &duration,
            ease::Function ease = ease::Linear);

        template<class ClassType, typename SetterFN, class Type>
        TweenID &Setter(ClassType *object,
            SetterFN setter,
            Type start,
            Type target,
            const TimeSpan &duration,
            ease::Function ease = ease::Linear);

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

    private:
        friend class TweenManager;
        friend class LocalTweenManager;

        friend class TweenGrouped;

        // id of the tween in the manager
        uint32 m_id;

        TweenManager *m_manager;

        TweenID(TweenManager *manager, uint32 id);

        TweenID &Removed(TweenCallback callback);

        void doProperty(const TimeSpan &duration,
            const TweenPercentageCallback &propertyFN);
    };
}

#include "TweenID.hpp"