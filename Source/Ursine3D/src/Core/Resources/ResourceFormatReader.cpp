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
            auto &stream = m_reader.m_stream;

            auto fileName = resourceFile.string( );

            stream.open( fileName, std::ios::binary );

            URSINE_TODO( "Make recoverable." );
            UAssert( stream.is_open( ),
                "Unble to open resource build file.\nfile: %s",
                fileName.c_str( )
            );

            /// read header

            static const auto magicLength = sizeof( kResourceFormatMagic );

            char magic[ magicLength ];

            auto magicBytesRead = m_reader.ReadBytes( magic, magicLength );

            UAssert( 
                magicBytesRead == magicLength && 
                memcmp( kResourceFormatMagic, magic, magicLength ) == 0,
                "Invalid resource format magic.\nfile: %s",
                fileName.c_str( )
            );

            // skip over the header padding

            stream.seekg( kResourceFormatHeaderSize + 1 );

            /// read resource data

            unsigned readerTypeNameSize;
            std::string readerTypeName;

            m_reader >> readerTypeNameSize;

            readerTypeName.resize( readerTypeNameSize );

            m_reader.ReadBytes( &readerTypeName[ 0 ], readerTypeNameSize );

            auto readerType = meta::Type::GetFromName( readerTypeName );

            UAssert( readerType.IsValid( ),
                "Unknown resource reader '%s'.\nfile: %s",
                readerTypeName.c_str( ),
                fileName.c_str( )
            );

            auto readerTypeCtor = readerType.GetDynamicConstructor( );

            UAssert( readerTypeCtor.IsValid( ),
                "Resource reader '%s' is missing a default dynamic constructor.\nfile: %s",
                readerType.GetName( ).c_str( ),
                fileName.c_str( )
            );

            auto reader = ResourceTypeReader::Handle(
                readerTypeCtor.Invoke( ).GetValue<ResourceTypeReader*>( ) 
            );

            auto resource = reader->Read( m_reader );

            stream.close( );

            return resource;
        }
    }
}