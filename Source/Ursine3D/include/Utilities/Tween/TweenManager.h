/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ActionManager.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "CoreSystem.h"

#include "Tween.h"

namespace ursine
{
    class TweenManager : public core::CoreSystem
    {
        CORE_SYSTEM

    public:
        Meta(Enable, DisableNonDynamic)
        TweenManager(void);
        ~TweenManager(void);

        // Suspends updating a specific group
        void Pause(TweenGroupID group);

        // Resumes updating a specific group
        void Resume(TweenGroupID group);

        // Removes all timers in a specific group
        void Clear(TweenGroupID group = Tween::GROUP_DEFAULT);

    private:
        friend class Tween;
        friend class TweenID;

        friend class LocalTweenManager;

        uint32 m_nextID;

		std::mutex m_mutex;

        std::unordered_map<uint32, Tween> m_tweens;

        // tweens in queue for deletion
        std::vector<uint32> m_deletionQueue;

        // groups and their pause state: vector<bool> is optimized for space (slow)
        // true is paused, false is not
        std::vector<uint8> m_groups;

        void onAppUpdate(EVENT_HANDLER(Application));

        TweenID create(TweenGroupID group);

        Tween *get(uint32 id);

        void cancel(uint32 id, bool invokeRemoved = true);
    } Meta(WhiteListMethods, Enable);
}