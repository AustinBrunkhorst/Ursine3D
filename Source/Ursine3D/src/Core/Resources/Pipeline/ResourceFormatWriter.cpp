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

        UAssertCatchable( stream.is_open( ),
            "Unble to open resource build file.\nfile: %s",
            fileName.c_str( )
        );

        auto readerType = resource->GetReaderType( );

        UAssertCatchable( readerType.IsValid( ),
            "Unknown reader type for resource data type '%s'.\nguid: %s",
            resource->GetType( ).GetName( ).c_str( ),
            to_string( m_resourceItem->GetGUID( ) ).c_str( )
        );

        /// write the header

        // magic
        m_writer.WriteBytes( kResourceFormatMagic, sizeof( kResourceFormatMagic ) );

        auto bytesWritten = stream.tellp( ) - streamBeginning;

        auto padding = kResourceFormatHeaderSize - bytesWritten;

        // write empty bytes for the rest of the unused header
        for (size_t i = 0; i < padding; ++i)
            m_writer.Write( '\0' );

        /// write the resource's data

        // reader type
        m_writer.Write( readerType.GetName( ) );

        resource->Write( m_writer );

        stream.close( );
    }
}