/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** U3DContentProcessorOptions.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "U3DContentProcessorOptions.h"

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            U3DContentProcessorOptions::U3DContentProcessorOptions(void)
                : processRig( false )
                , processModel( false )
                , processAnimations( false ) { }
        }
    }
}
