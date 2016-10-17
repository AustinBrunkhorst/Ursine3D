/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** RotationKeyFrame.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "KeyFrame.h"
#include "SQuat.h"

namespace ursine
{
    class RotationKeyFrame : public KeyFrame
    {
    public:
        RotationKeyFrame(void);
        RotationKeyFrame(float time, const SQuat &value);

        SQuat value;
    };
}
