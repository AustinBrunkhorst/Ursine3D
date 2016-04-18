/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ResourceFormatConfig.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

namespace ursine
{
    namespace resources
    {
        // Magic for built resource file headers
        extern const char kResourceFormatMagic[3];

        // Extension (without a dot) for built resource files
        extern const char * const kResourceFormatExtension;

        // Size in bytes of the resource format header
        extern const size_t kResourceFormatHeaderSize;
    }
}