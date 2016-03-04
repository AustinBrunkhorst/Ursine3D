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

            ResourceWriter &ResourceWriter::WriteBytes(const void *bytes, size_t count)
            {
                m_stream.write( reinterpret_cast<const char*>( bytes ), count );

                return *this;
            }

            ResourceWriter &ResourceWriter::WriteString(const std::string &output)
            {
                Write( static_cast<unsigned>( output.size( ) ) );

                m_stream.write( output.c_str( ), output.size( ) );

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