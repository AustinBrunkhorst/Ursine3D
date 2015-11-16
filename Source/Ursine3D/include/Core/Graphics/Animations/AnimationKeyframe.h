/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AnimationKeyframe.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "SVec3.h"
#include "SQuat.h"

namespace ursine
{
    //this is data that won't change, it's a keyframe
    struct AnimationKeyframe
    {
        float length;
        SVec3 translation;
        SVec3 scale;
        SQuat rotation;
    };
}