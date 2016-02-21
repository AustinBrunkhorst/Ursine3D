#include "UrsinePrecompiled.h"

#include "BuiltInResourceConfig.h"

#include "JsonImporter.h"
#include "WorldProcessor.h"

namespace ursine
{
    const char * const rp::kResourceTypeWorldExtension = "uworld";

    const rp::TypePairMap &rp::GetBuiltInResourceHandlers(void)
    {
        static TypePairMap handlers {
            { kResourceTypeWorldExtension, { typeof( JsonImporter ), typeof( WorldProcessor ) } }
        };

        return handlers;
    }
}