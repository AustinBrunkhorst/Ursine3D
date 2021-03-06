/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
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
        void MakeLowerCase(std::string &input)
        {
            transform( input.begin( ), input.end( ), input.begin( ), tolower );
        }

        void MakeUpperCase(std::string &input)
        {
            transform( input.begin( ), input.end( ), input.begin( ), toupper );
        }

        void CopyToClipboard(const std::string &text)
        {
        #if defined(PLATFORM_WINDOWS)

            auto length = text.size( ) + 1;
            auto memory = GlobalAlloc( GMEM_MOVEABLE, text.size( ) + 1 );

            memcpy( GlobalLock( memory ), text.c_str( ), length );

            GlobalUnlock( memory );
            OpenClipboard( nullptr );
            EmptyClipboard( );
            SetClipboardData( CF_TEXT, memory );
            CloseClipboard( );

        #endif
        }

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