#include "UrsinePrecompiled.h"

#include "ResourceFormatWriter.h"
#include "ResourceFormatConfig.h"

namespace ursine
{
    rp::ResourceFormatWriter::ResourceFormatWriter(ResourceItem::Handle resourceItem)
        : m_resourceItem( resourceItem )
    {

    }

    void rp::ResourceFormatWriter::Write(resources::ResourceData::Handle resource)
    {
        auto &stream = m_writer.m_stream;

        auto fileName = m_resourceItem->GetBuildFileName( ).string( );

        stream.open( fileName, std::ios::binary );

        auto streamBeginning = stream.tellp( );

        URSINE_TODO( "Make recoverable." );
        UAssert( stream.is_open( ),
            "Unble to open resource build file.\nfile: %s",
            fileName.c_str( )
        );

        auto readerType = resource->GetReaderType( );

        UAssert( readerType.IsValid( ),
            "Unknown reader type for resource data type '%s'.\nguid: %s",
            resource->GetType( ).GetName( ).c_str( ),
            to_string( m_resourceItem->GetGUID( ) ).c_str( )
        );

        auto readerTypeName = readerType.GetName( );
        auto readerTypeLength = static_cast<unsigned>( readerTypeName.size( ) );

        /// write the header

        // magic
        m_writer.WriteBytes( kResourceFormatMagic, sizeof( kResourceFormatMagic ) );

        auto bytesWritten = stream.tellp( ) - streamBeginning;

        auto padding = kResourceFormatHeaderSize - bytesWritten;

        // write empty bytes for the rest of the unused header
        for (size_t i = 0; i < padding; ++i)
            m_writer << '\0';

        /// write the resource's data

        // reader type
        m_writer << readerTypeLength;
        m_writer << readerTypeName;

        resource->Write( m_writer );

        stream.close( );
    }
}