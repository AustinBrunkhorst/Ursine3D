#include "UrsinePrecompiled.h"

#include "ResourceReader.h"

namespace ursine 
{
    namespace resources
    {
        size_t ResourceReader::ReadBytes(char *output, size_t count)
        {
            m_stream.read( output, count );

            return m_stream.gcount( );
        }
    }
}