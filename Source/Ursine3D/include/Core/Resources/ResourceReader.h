#pragma once

namespace ursine
{
    namespace resources
    {
        class ResourceReader
        {
        public:
            size_t ReadBytes(char *output, size_t count);

            template<typename T>
            ResourceReader &operator>>(T &output);

        private:
            std::ifstream m_stream;
        };
    }
}

#include "ResourceReader.hpp"