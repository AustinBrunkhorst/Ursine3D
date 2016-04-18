/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ResourceFormatReader.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "ResourceFormatReader.h"
#include "ResourceTypeReader.h"
#include "ResourceFormatConfig.h"

namespace ursine
{
    namespace resources
    {
        ResourceFormatReader::ResourceFormatReader(void) { }

        ResourceData::Handle ResourceFormatReader::Read(const fs::path &resourceFile)
        {
            ResourceReader reader( resourceFile );

            UAssert( reader.IsOpen( ),
                "Unable to open resource for reading.\nfile: %s",
                resourceFile.string( ).c_str( )
            );

            auto resource = read( reader );

            return resource;
        }

        ResourceData::Handle ResourceFormatReader::read(ResourceReader &reader)
        {
            /// read header

            static const auto magicLength = sizeof( kResourceFormatMagic );

            char magic[ magicLength ];

            auto magicBytesRead = reader.ReadBytes( magic, magicLength );

            UAssertCatchable( 
                magicBytesRead == magicLength && 
                memcmp( kResourceFormatMagic, magic, magicLength ) == 0,
                "Invalid resource format magic."
            );

            // skip over the header padding
            reader.Seek( kResourceFormatHeaderSize );

            /// read resource data

            std::string readerTypeName;

            reader.Read( readerTypeName );

            auto readerType = meta::Type::GetFromName( readerTypeName );

            UAssertCatchable( readerType.IsValid( ),
                "Unknown resource reader '%s'.",
                readerTypeName.c_str( )
            );

            auto readerTypeCtor = readerType.GetDynamicConstructor( );

            UAssertCatchable( readerTypeCtor.IsValid( ),
                "Resource reader '%s' is missing a default dynamic constructor.",
                readerType.GetName( ).c_str( )
            );

            auto typeReader = ResourceTypeReader::Handle(
                readerTypeCtor.Invoke( ).GetValue<ResourceTypeReader*>( ) 
            );

            auto resource = typeReader->Read( reader );

            return resource;
        }
    }
}