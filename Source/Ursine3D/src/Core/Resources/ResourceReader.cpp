#include "UrsinePrecompiled.h"

#include "ResourceReader.h"

namespace ursine 
{
    namespace resources
    {
        char ResourceReader::ReadByte(void)
        {
            char byte;

            m_stream >> byte;

            return byte;
        }

        size_t ResourceReader::ReadBytes(char *output, size_t count)
        {
            m_stream.read( output, count );

            return m_stream.gcount( );
        }

        ResourceReader &ResourceReader::Seek(size_t offset)
        {
            m_stream.seekg( offset );

            return *this;
        }
    }
}