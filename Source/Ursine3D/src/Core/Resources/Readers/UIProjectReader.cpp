#include "UrsinePrecompiled.h"

#include "UIProjectReader.h"
#include "UIProjectData.h"

namespace ursine
{
    namespace resources
    {
        UIProjectReader::UIProjectReader(void) { }

        ResourceData::Handle UIProjectReader::Read(ResourceReader &input)
        {
            UIProjectData::ResourceTable table;

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

            return std::make_shared<UIProjectData>( std::move( table ) );
        }
    }
}