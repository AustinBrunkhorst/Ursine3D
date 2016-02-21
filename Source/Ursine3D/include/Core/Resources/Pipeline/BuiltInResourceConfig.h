#pragma once

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            typedef std::unordered_map<std::string, meta::Type> TypeMap;

            ///////////////////////////////////////////////////////////////////
            // Extensions
            ///////////////////////////////////////////////////////////////////

            extern const char * const kResourceWorldExtension;

            ///////////////////////////////////////////////////////////////////
            // Utility Functions
            ///////////////////////////////////////////////////////////////////

            const TypeMap &GetBuiltInResourceImporters(void);
            const TypeMap &GetBuiltInResourceProcessors(void);
        }
    }
}