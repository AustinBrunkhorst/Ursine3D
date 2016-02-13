#pragma once

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

                template<typename T>
                ResourceWriter &operator<<(const T &value);

            private:
                std::ofstream m_stream;
            };
        }
    }
}

#include "ResourceWriter.hpp"