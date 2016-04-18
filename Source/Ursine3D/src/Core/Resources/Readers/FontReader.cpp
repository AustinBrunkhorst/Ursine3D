/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** FontReader.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "FontReader.h"
#include "FontData.h"

namespace ursine
{
    namespace resources
    {
        FontReader::FontReader(void) { }

        ResourceData::Handle FontReader::Read(ResourceReader &input)
        {
            BinaryData fntData;

            input.Read( fntData );

            FontData::TexturePageTable table;

            unsigned entryCount;

            input.Read( entryCount );

            for (unsigned i = 0; i < entryCount; ++i)
            {
                UAssertCatchable( input.IsOpen( ) && !input.IsEOF( ),
                    "Error reading font texture page entry '%i'.",
                    i
                );

                std::string path;

                input.Read( path );

                auto entryData = std::make_shared<BinaryData>( );

                input.Read( *entryData );

                auto result = table.emplace( path, entryData );

                UAssertCatchable( result.second,
                    "Duplicated font texture page entry.\npage: %s",
                    path.c_str( )
                );
            }

            return std::make_shared<FontData>( 
                std::move( fntData ), 
                std::move( table ) 
            );
        }
    }
}