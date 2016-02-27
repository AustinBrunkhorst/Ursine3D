#include "UrsinePrecompiled.h"

#include "ResourceReader.h"

namespace ursine 
{
    namespace resources
    {
        ResourceReader::ResourceReader(void)
            : m_stream( )
        {

        }

        ResourceReader::ResourceReader(const fs::path &input)
            : m_stream( input.string( ) )
        {

        }

        char ResourceReader::ReadByte(void)
        {
            char byte;

            m_stream >> byte;

            return byte;
        }

        size_t ResourceReader::ReadBytes(char *input, size_t count)
        {
            m_stream.read( input, count );

            return m_stream.gcount( );
        }

        ResourceReader &ResourceReader::Seek(size_t offset)
        {
            m_stream.seekg( offset );

            return *this;
        }

        bool ResourceReader::IsOpen(void) const
        {
            return m_stream.is_open( );
        }
    }
}