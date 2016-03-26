/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** FileSystem.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "BinaryData.h"

#include <boost/filesystem.hpp>
#include <FileWatcher.h>

#include <unordered_set>

namespace ursine
{
    namespace fs
    {
        using namespace boost::filesystem;
        using namespace efsw;
        
        typedef boost::hash<path> PathHasher;
        typedef std::vector<path> PathList;
        typedef std::unordered_set<path, PathHasher> PathSet;

        template<typename T>
        using PathMap = std::unordered_map<path, T, PathHasher>;

        typedef directory_iterator DirectoryIterator;
        typedef recursive_directory_iterator RecursiveDirectoryIterator;

        bool LoadAllText(const std::string &fileName, std::string &output);
        bool WriteAllText(const std::string &fileName, const std::string &text);

        bool LoadAllBinary(const std::string &fileName, BinaryData &output);

        std::string SafeFileName(const std::string &fileName, char replacement);

        path MakeRelativePath(const path &from, const path &to);
        bool PathContainsFile(path directory, path file);
    }
}