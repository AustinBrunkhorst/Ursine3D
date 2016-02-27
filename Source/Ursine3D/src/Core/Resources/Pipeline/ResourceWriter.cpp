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
                : m_stream( output.string( ) )
            {

            }

            ResourceWriter &ResourceWriter::WriteBytes(const char *bytes, size_t count)
            {
                m_stream.write( bytes, count );

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