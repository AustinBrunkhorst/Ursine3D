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
                template<typename T>
                ResourceWriter &operator<<(const T &value);

            private:
                std::ofstream m_stream;
            };
        }
    }
}

#include "ResourceWriter.hpp"