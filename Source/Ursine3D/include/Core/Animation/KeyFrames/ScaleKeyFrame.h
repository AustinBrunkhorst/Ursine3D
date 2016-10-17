/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ScaleKeyFrame.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "KeyFrame.h"
#include "SVec3.h"

namespace ursine
{
    class ScaleKeyFrame : public KeyFrame
    {
    public:
        ScaleKeyFrame(void);
        ScaleKeyFrame(float time, const SVec3 &value);

        SVec3 value;
    };
}
