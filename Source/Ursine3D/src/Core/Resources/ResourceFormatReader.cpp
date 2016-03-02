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
            auto fileName = resourceFile.string( );

            auto stream = std::make_shared<std::ifstream>( );

            stream->open( fileName, std::ios::binary );

            UAssertCatchable( stream->is_open( ),
                "Unble to open resource build file.\nfile: %s",
                fileName.c_str( )
            );

            ResourceReader reader;

            reader.m_stream = stream;

            auto resource = read( reader );

            stream->close( );

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

            reader.ReadString( readerTypeName );

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