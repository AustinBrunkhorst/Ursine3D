/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Utils.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#ifdef PLATFORM_WINDOWS
// working directory on windows
#include <direct.h>
#ifndef getcwd

#define getcwd _getcwd

#endif

#ifndef chdir

#define chdir _chdir

#endif

#else
#include <unistd.h>
#endif

namespace Ursine
{
    namespace Utils
    {
        void Join(const std::vector<std::string> &input, const std::string &joiner, std::string &output)
        {
            // nothing to join
            if (input.empty( ))
                return;

            // append element with joiner
            for (size_t i = 0; i < input.size( ) - 1; ++i)
                output += input[ i ] + joiner;

            // append last element without joiner
            output += input.back( );
        }

        std::string MediaPath(const std::string &file, MediaType type)
        {
            switch (type)
            {
                case TYPE_AUDIO: return "Assets/Audio/Desktop/" + file;
                case TYPE_GUIDs: return "Assets/Audio/" + file;
            }

            return "";
        }

        std::string NormalizePath(std::string path)
        {
            std::replace_if(path.begin(), path.end(),
                [](char c) { return c == '\\'; }, '/');

            return path;
        }

        std::string FileName(const std::string &path)
        {
            return path.substr(path.find_last_of("/\\") + 1);
        }

        std::string DirectoryName(const std::string &filename)
        {
            return filename.substr(0, filename.find_last_of("/\\"));
        }

        std::string GetWorkingDirectory(void)
        {
            char *cwd = getcwd(nullptr, 0);

            // empty
            if (!cwd)
                return std::string();

            std::string dir(cwd);

            free(cwd);

            return dir;
        }

        void SetWorkingDirectory(const std::string &directory)
        {
            UAssert(!chdir(directory.c_str()),
                "Failed to set working directory.\n- directory: %s\n- error: %s",
                directory.c_str(),
                strerror(errno));
        }
    }
}