#pragma once

#include "FileSystem.h"

#include <istream>

namespace ursine
{
    namespace resources
    {
        class ResourceReader
        {
        public:
            ResourceReader(void);

            // Reads from memory
            ResourceReader(const void *buffer, size_t size);

            // Reads from a file
            ResourceReader(const fs::path &input);

            char ReadByte(void);
            size_t ReadBytes(void *input, size_t count);

            ResourceReader &ReadString(std::string &input);

            template<typename T>
            ResourceReader &Read(T &input);

            ResourceReader &Seek(size_t offset);

            bool IsOpen(void) const;

        private:
            friend class ResourceFormatReader;

            std::shared_ptr<std::istream> m_stream;
        };
    }
}

#include "ResourceReader.hpp"