#pragma once

#include "Tween.h"

namespace ursine
{
    class LocalTweenManager
    {
        std::unordered_map<uint32, TweenID> _created;

        uint32 _default_group;

    public:
        LocalTweenManager(TweenGroupID default_group = Tween::GROUP_DEFAULT);
        ~LocalTweenManager(void);

        // Creates a timer with the default group defined during construction
        TweenID Create(void);

        // Creates a timer with the specified group
        TweenID Create(TweenGroupID group);

        // Clears all timers in this manager
        void ClearAll(void);

        // Pauses all timers in this manager
        void PauseAll(void);

        // Resumes all timers in this manager
        void ResumeAll(void);
    };
}