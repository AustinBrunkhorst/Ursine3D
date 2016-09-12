/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** BuiltInResourceConfig.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "BuiltInResourceConfig.h"

#include "JsonImporter.h"

#include "WorldProcessor.h"
#include "ArchetypeProcessor.h"
#include "UIScreenProcessor.h"
#include "AudioItemProcessor.h"

namespace ursine
{
    const char * const rp::kResourceTypeWorldExtension = "uworld";
    const char * const rp::kResourceTypeArchetypeExtension = "uatype";
    const char * const rp::kResourceTypeUIScreenExtension = "ui";
    const char * const rp::kResourceTypeAudioEventExtension = "uaevent";

    const rp::TypePairMap &rp::GetBuiltInResourceHandlers(void)
    {
        static TypePairMap handlers {
            { kResourceTypeWorldExtension,      { typeof( JsonImporter ), typeof( WorldProcessor ) } },
            { kResourceTypeArchetypeExtension,  { typeof( JsonImporter ), typeof( ArchetypeProcessor ) } },
            { kResourceTypeUIScreenExtension,   { typeof( JsonImporter ), typeof( UIScreenProcessor ) } },
            { kResourceTypeAudioEventExtension, { typeof( JsonImporter ), typeof( AudioItemProcessor ) } }
        };

        return handlers;
    }
}