#pragma once

#include <fstream>
#include "FileSystem.h"

namespace ursine
{
    namespace resources
    {
        class ResourceReader
        {
        public:
            ResourceReader(void);
            ResourceReader(const fs::path &input);

            char ReadByte(void);
            size_t ReadBytes(char *input, size_t count);

            ResourceReader &Seek(size_t offset);

            template<typename T>
            ResourceReader &operator>>(T &input);

            bool IsOpen(void) const;

        private:
            friend class ResourceFormatReader;

            std::ifstream m_stream;
        };
    }
}

#include "ResourceReader.hpp"