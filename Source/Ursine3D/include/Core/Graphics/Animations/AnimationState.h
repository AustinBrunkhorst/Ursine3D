/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AnimationState.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once
#include "Animation.h"
//this is per-instance data that keeps track of the current state of our animation

namespace ursine
{
    class AnimationState
    {
    public:
        AnimationState(void);

        float GetTimePosition(void) const;
        void SetTimePosition(const float);

        void IncrementTimePosition(const float dt);

        const Animation *GetAnimation(void) const;
        void SetAnimation(const Animation *animation);

    private:
        //current runtime
        float m_timePos;

        //current animation
        const Animation *m_animation;
    };
}