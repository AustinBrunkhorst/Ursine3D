#include "UrsinePrecompiled.h"

#include "BuiltInResourceConfig.h"

namespace ursine
{
    const char * const rp::kResourceWorldExtension = "uworld";

    const rp::TypeMap &rp::GetBuiltInResourceImporters(void)
    {
        static TypeMap importers {
            { kResourceWorldExtension, typeof( JsonImporter ) }
        };

        return importers;
    }

    const rp::TypeMap &rp::GetBuiltInResourceProcessors(void)
    {
        static TypeMap processors {
            { kResourceWorldExtension, typeof( WorldProcessor ) }
        };

        return processors;
    }
}