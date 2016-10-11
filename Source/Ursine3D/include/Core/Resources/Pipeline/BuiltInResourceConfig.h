/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** BuiltInResourceConfig.h
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
        namespace pipeline
        {
            typedef std::pair<meta::Type, meta::Type> TypePair;
            typedef std::unordered_map<std::string, TypePair> TypePairMap;

            ///////////////////////////////////////////////////////////////////
            // Extensions
            ///////////////////////////////////////////////////////////////////

            extern const char * const kResourceTypeWorldExtension;
            extern const char * const kResourceTypeArchetypeExtension;
            extern const char * const kResourceTypeUIScreenExtension;
            extern const char * const kResourceTypeAudioEventExtension;
            extern const char * const kResourceTypeModelExtension;
            extern const char * const kResourceTypeRigExtension;
            extern const char * const kResourceTypeAnimationExtension;

            ///////////////////////////////////////////////////////////////////
            // Utility Functions
            ///////////////////////////////////////////////////////////////////

            const TypePairMap &GetBuiltInResourceHandlers(void);
        }
    }
}