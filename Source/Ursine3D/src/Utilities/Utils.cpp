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

namespace ursine
{
    namespace utils
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

        void OpenPath(const std::string &path)
        {
        #ifdef PLATFORM_WINDOWS

            ShellExecute(
                nullptr,
                "open",
                path.c_str( ),
                nullptr,
                nullptr,
                SW_SHOWNORMAL
            );

        #endif
        }
    }
}