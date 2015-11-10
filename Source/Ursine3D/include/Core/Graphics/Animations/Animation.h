/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Animation.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once
#include <string>
#include <vector>

#include "AnimationKeyframe.h"
#include "SVec3.h"
#include "SQuat.h"

//this class holds the keyframes

namespace ursine
{
    class Animation
    {
    public:
        //create an animation with information from loader. What does an animation need?
            //keyframes
            //how many bones should it have?
            //animation length
            //name

        Animation(std::string &name);

        // add a keyframe to this animation
        void AddKeyframe(const SVec3 &trans, const SVec3 &scale, const SQuat &rot, const float runTime);

        // gettors and setters //////////////////////////////////////

        const std::string &GetName(void) const;
        void SetName(const std::string &name);

        const std::vector<AnimationKeyframe> &GetKeyframes(void) const;
        const AnimationKeyframe &GetKeyframe(unsigned index) const;
    private:
        std::string m_name;

        std::vector<AnimationKeyframe> m_keyframes;
    };
}