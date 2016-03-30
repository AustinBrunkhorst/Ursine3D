#pragma once

#include "FileSystem.h"

#include "BinaryData.h"

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
                ResourceWriter(void);
                ResourceWriter(const fs::path &output);

                static void WriteBytes(std::ostream &stream, const void *bytes, size_t count);

                static void Write(std::ostream &stream, const std::string &output);
                static void Write(std::ostream &stream, const BinaryData &output);

                template<typename T>
                static void Write(std::ostream &stream, const T &value);

                ResourceWriter &Seek(size_t offset);

                ResourceWriter &WriteBytes(const void *bytes, size_t count);

                ResourceWriter &Write(const std::string &output);
                ResourceWriter &Write(const BinaryData &output);

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