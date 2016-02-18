#pragma once

#include <fstream>

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
            friend class ResourceFormatReader;

            std::ifstream m_stream;
        };
    }
}

#include "ResourceReader.hpp"