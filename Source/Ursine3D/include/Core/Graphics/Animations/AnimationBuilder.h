/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AnimationBuilder.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once
#include <vector>

#include "AnimationRig.h"
#include "Animation.h"
#include "AnimationState.h"

namespace ursine
{
    class AnimationBuilder
    {
    public:
        //given a Rig, State, and 2 vectors of matrices, generate matrix palette

        void GenerateAnimationData(const AnimationState &animState, const Animation *animation, std::vector<SMat4> &outputMatPal);

        //static stuff
        static void InitializeStaticData(void);
        static unsigned AddAnimation(void);
        static unsigned AddAnimationRig(void);
        
        static Animation *GetAnimation(const unsigned index);
        static AnimationRig *GetAnimationRig(const unsigned index);
    private:
        static std::vector<Animation> m_animationData;
        static std::vector<AnimationRig> m_animationRigData;
        
    };
}
