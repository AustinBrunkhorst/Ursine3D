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
        bool LoadAllText(const std::string &filename, std::string &output)
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

        bool WriteAllText(const std::string &filename, const std::string &text)
        {
            std::ofstream output( filename );

            if (!output)
                return false;

            output << text;

            output.close( );

            return true;
        }
    }
}