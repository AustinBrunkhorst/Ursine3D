#include "UrsinePrecompiled.h"

#include "ResourceWriter.h"

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            ResourceWriter &ResourceWriter::WriteBytes(const char *bytes, size_t count)
            {
                m_stream.write( bytes, count );

                return *this;
            }
        }
    }
}