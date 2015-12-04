#pragma once

#include <boost/filesystem.hpp>

namespace ursine
{
    namespace fs
    {
        using namespace boost::filesystem;

        typedef std::vector<path> FileList;

        bool LoadText(const std::string &filename, std::string &output);
        bool WriteText(const std::string &filename, const std::string &text);
    }
}