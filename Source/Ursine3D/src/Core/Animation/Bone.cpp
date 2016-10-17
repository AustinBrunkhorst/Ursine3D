/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** Bone.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "Bone.h"

namespace ursine
{
    Bone::Bone(void)
        : name( "N/A" )
        , parent( -1 )
        , numChildren( 0 ) { }
}
