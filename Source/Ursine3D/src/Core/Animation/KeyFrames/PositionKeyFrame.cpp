/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** PositionKeyFrame.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "PositionKeyFrame.h"

namespace ursine
{
    PositionKeyFrame::PositionKeyFrame(void)
        : KeyFrame( 0.0f ) { }

    PositionKeyFrame::PositionKeyFrame(float time, const SVec3 &value)
        : KeyFrame( time )
        , value( value ) { }
}
