/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
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

#include "System.h"

#include "Tween.h"

namespace Ursine
{
    class TweenManager : public System
    {
        friend class Tween;
        friend class TweenID;

        friend class LocalTweenManager;

        uint32 _next_id;

        std::unordered_map<uint32, Tween> _tweens;

        // tweens in queue for deletion
        std::vector<uint32> _deletion_queue;

        // groups and their pause state: vector<bool> is optimized for space (slow)
        // true is paused, false is not
        std::vector<uint8> _groups;

        TweenID create(TweenGroupID group);

        Tween *get(uint32 id);

        void cancel(uint32 id, bool invoke_removed = true);

    public:
        TweenManager(void);
        ~TweenManager(void);

        void Update(void) override;

        // Suspends updating a specific group
        void Pause(TweenGroupID group);

        // Resumes updating a specific group
        void Resume(TweenGroupID group);

        // Removes all timers in a specific group
        void Clear(TweenGroupID group = Tween::GROUP_DEFAULT);
    };

    extern TweenManager *gTweenManager;
}