/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** RotationKeyFrame.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "RotationKeyFrame.h"

namespace ursine
{
    RotationKeyFrame::RotationKeyFrame(void)
        : KeyFrame( 0.0f ) { }

    RotationKeyFrame::RotationKeyFrame(float time, const SQuat &value)
        : KeyFrame( time )
        , value( value ) { }
}
