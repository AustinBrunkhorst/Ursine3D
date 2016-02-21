#pragma once

#include <fstream>

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            class ResourceWriter
            {
            public:
                ResourceWriter &WriteBytes(const char *bytes, size_t count);

                ResourceWriter &Seek(size_t offset);

                template<typename T>
                ResourceWriter &operator<<(const T &value);

            private:
                friend class ResourceFormatWriter;

                std::ofstream m_stream;
            };
        }
    }
}

#include "ResourceWriter.hpp"