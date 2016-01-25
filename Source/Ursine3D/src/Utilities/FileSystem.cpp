/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** FileSystem.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

namespace ursine
{
    namespace fs
    {
        bool LoadText(const std::string &filename, std::string &output)
        {
            std::ifstream input( filename );

            if (!input)
                return false;

            input.seekg( 0, std::ios::end );

            output.reserve(
                static_cast<std::string::size_type>( input.tellg( ) )
            );

            input.seekg( 0, std::ios::beg );

            output.assign(
                // disambiguate function declaration
                (std::istreambuf_iterator<char>( input )),
                std::istreambuf_iterator<char>( ) 
            );

            input.close( );

            return true;
        }

        bool WriteText(const std::string &filename, const std::string &text)
        {
            std::ofstream output( filename );

            if (!output)
                return false;

            output << text;

            output.close( );

            return true;
        }

        path MakeRelativePath(const path &from, const path &to)
        {
            // Start at the root path and while they are the same then do nothing then when they first
            // diverge take the remainder of the two path and replace the entire from path with ".."
            // segments.
            auto itFrom = from.begin( );
            auto itTo = to.begin( );

            // Loop through both
            while (itFrom != from.end( ) && itTo != to.end( ) && (*itTo) == (*itFrom))
            {
                ++itTo;
                ++itFrom;
            }

            path finalPath;

            while (itFrom != from.end( ))
            {
                finalPath /= "..";

                ++itFrom;
            }

            while (itTo != to.end( ))
            {
                finalPath /= *itTo;

                ++itTo;
            }

            return finalPath;
        }
    }
}