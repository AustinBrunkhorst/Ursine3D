#pragma once

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            typedef std::pair<meta::Type, meta::Type> TypePair;
            typedef std::unordered_map<std::string, TypePair> TypePairMap;

            ///////////////////////////////////////////////////////////////////
            // Extensions
            ///////////////////////////////////////////////////////////////////

            extern const char * const kResourceTypeWorldExtension;

            ///////////////////////////////////////////////////////////////////
            // Utility Functions
            ///////////////////////////////////////////////////////////////////

            const TypePairMap &GetBuiltInResourceHandlers(void);
        }
    }
}