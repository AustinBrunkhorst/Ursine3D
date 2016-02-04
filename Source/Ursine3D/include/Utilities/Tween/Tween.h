/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Action.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "TweenConfig.h"
#include "TweenItem.h"
#include "TweenID.h"

#include <deque>

namespace ursine
{
    class Tween
    {
    public:
        // Default tween group
        static const TweenGroupID GROUP_DEFAULT = 0;

        ~Tween(void);

        void Update(DeltaTime dt);

        static TweenID Create(TweenGroupID group = GROUP_DEFAULT);

    private:
        friend class TweenManager;
        friend class TweenID;

        // group id (defaults to zero)
        TweenGroupID m_group;

        // determines if the tween is being deleted from the TweenManager
        bool m_deleting;

        // determines if the timer will be updated
        bool m_paused;

        std::deque<TweenItem *> m_items;

        // called when this tween is removed (used internally)
        TweenCallback m_removed;

        Tween(TweenGroupID group);

        // adds an item to this tween
        void add(TweenItem *item);

        // removes all items from this tween
        void stop(void);
    };
}