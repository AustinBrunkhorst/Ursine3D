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