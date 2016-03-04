#include "UrsinePrecompiled.h"

#include "BuiltInResourceConfig.h"

#include "JsonImporter.h"

#include "WorldProcessor.h"
#include "ArchetypeProcessor.h"

namespace ursine
{
    const char * const rp::kResourceTypeWorldExtension = "uworld";
    const char * const rp::kResourceTypeArchetypeExtension = "uatype";

    const rp::TypePairMap &rp::GetBuiltInResourceHandlers(void)
    {
        static TypePairMap handlers {
            { kResourceTypeWorldExtension,     { typeof( JsonImporter ), typeof( WorldProcessor ) } },
            { kResourceTypeArchetypeExtension, { typeof( JsonImporter ), typeof( ArchetypeProcessor ) } }
        };

        return handlers;
    }
}