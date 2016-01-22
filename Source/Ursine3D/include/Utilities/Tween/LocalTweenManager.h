/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** LocalTweenManager.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "Tween.h"

namespace ursine
{
    class LocalTweenManager
    {
    public:
        LocalTweenManager(void) = default;

        LocalTweenManager(TweenManager *manager, TweenGroupID defaultGroup = Tween::GROUP_DEFAULT);
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

    private:
        uint32 m_defaultGroup;

        TweenManager *m_manager;

        std::unordered_map<uint32, TweenID> m_created;
    };
}