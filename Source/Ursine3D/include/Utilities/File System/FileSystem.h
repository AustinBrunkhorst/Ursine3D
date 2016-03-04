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

#include <boost/filesystem.hpp>
#include <FileWatcher.h>

namespace ursine
{
    namespace fs
    {
        using namespace boost::filesystem;
        using namespace efsw;
        
        typedef std::vector<path> FileList;
        typedef boost::hash<path> PathHasher;

        typedef directory_iterator DirectoryIterator;
        typedef recursive_directory_iterator RecursiveDirectoryIterator;

        bool LoadAllText(const std::string &fileName, std::string &output);
        bool WriteAllText(const std::string &fileName, const std::string &text);

        std::string SafeFileName(const std::string &fileName, char replacement);

        path MakeRelativePath(const path &from, const path &to);
        bool PathContainsFile(path directory, path file);
    }
}