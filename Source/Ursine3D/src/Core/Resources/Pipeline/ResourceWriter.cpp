#include "UrsinePrecompiled.h"

#include "ResourceWriter.h"

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            ResourceWriter::ResourceWriter(void)
                : m_stream( )
            {
            }

            ResourceWriter::ResourceWriter(const fs::path &output)
                : m_stream( output.string( ).c_str( ), std::ios::out | std::ios::binary )
            {

            }

            void ResourceWriter::WriteBytes(std::ostream &stream, const void *bytes, size_t count)
            {
                stream.write( reinterpret_cast<const char*>( bytes ), count );
            }

            void ResourceWriter::Write(std::ostream &stream, const std::string &output)
            {
                Write( stream, static_cast<unsigned>( output.size( ) ) );

                stream.write( output.c_str( ), output.size( ) );
            }

            void ResourceWriter::Write(std::ostream &stream, const BinaryData &output)
            {
                uint64 size = output.GetSize( );

                Write( stream, size );
                WriteBytes( stream, output.GetData( ), size );
            }

            ResourceWriter &ResourceWriter::WriteBytes(const void *bytes, size_t count)
            {
                WriteBytes( m_stream, bytes, count );

                return *this;
            }

            ResourceWriter &ResourceWriter::Write(const std::string &output)
            {
                Write( m_stream, output );

                return *this;
            }

            ResourceWriter &ResourceWriter::Write(const BinaryData &value) 
            {
                Write( m_stream, value );

                return *this;
            }

            ResourceWriter &ResourceWriter::Seek(size_t offset)
            {
                m_stream.seekp( offset );

                return *this;
            }

            bool ResourceWriter::IsOpen(void) const
            {
                return m_stream.is_open( );
            }
        }
    }
}