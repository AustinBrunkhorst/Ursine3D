#include "UrsinePrecompiled.h"

#include "UIResourceReader.h"
#include "UIResourceData.h"

namespace ursine
{
    namespace resources
    {
        UIResourceReader::UIResourceReader(void) { }

        ResourceData::Handle UIResourceReader::Read(ResourceReader &input)
        {
            UIResourceData::ResourceTable table;

            unsigned entryCount;

            input.Read( entryCount );

            for (unsigned i = 0; i < entryCount; ++i)
            {
                UAssertCatchable( input.IsOpen( ) && !input.IsEOF( ),
                    "Error reading ui resource entry '%i'.",
                    i
                );

                std::string path;

                input.Read( path );

                auto entryData = std::make_shared<BinaryData>( );

                input.Read( *entryData );

                auto result = table.emplace( path, entryData );

                UAssertCatchable( result.second,
                    "Duplicated ui resource entry.\nresource: %s",
                    path.c_str( )
                );
            }

            return std::make_shared<UIResourceData>( std::move( table ) );
        }
    }
}