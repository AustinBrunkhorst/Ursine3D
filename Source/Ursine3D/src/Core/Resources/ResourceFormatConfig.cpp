/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ResourceFormatConfig.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "ResourceFormatConfig.h"

namespace ursine
{
    namespace resources
    {
        const char kResourceFormatMagic[3] { 'U', 'R', 'B' };
        const char * const kResourceFormatExtension = "urb";
        const size_t kResourceFormatHeaderSize = 32;
    }
}