#pragma once

#include <fstream>
#include "FileSystem.h"

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            class ResourceWriter
            {
            public:
                ResourceWriter(void);
                ResourceWriter(const fs::path &output);

                ResourceWriter &WriteBytes(const void *bytes, size_t count);
                ResourceWriter &WriteString(const std::string &output);

                ResourceWriter &Seek(size_t offset);

                template<typename T>
                ResourceWriter &Write(const T &value);

                bool IsOpen(void) const;

            private:
                friend class ResourceFormatWriter;

                std::ofstream m_stream;
            };
        }
    }
}

#include "ResourceWriter.hpp"