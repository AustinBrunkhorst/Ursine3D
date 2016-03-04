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
        bool LoadAllText(const std::string &fileName, std::string &output)
        {
            std::ifstream input( fileName );

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

        bool WriteAllText(const std::string &fileName, const std::string &text)
        {
            std::ofstream output( fileName );

            if (!output)
                return false;

            output << text;

            output.close( );

            return true;
        }

        std::string SafeFileName(const std::string &fileName, char replacement)
        {
            auto sanitized = fileName;

            std::string illegalChars = "\\/:?\"<>|";

            for (auto &character : sanitized) 
            {
                if (illegalChars.find( character ) != std::string::npos)
                    character = replacement;
            }

            return sanitized;
        }

        path MakeRelativePath(const path &from, const path &to)
        {
            // start at the root path and while they are the same then do nothing then when they first
            // diverge take the remainder of the two path and replace the entire from path with ".."
            // segments.
            auto itFrom = from.begin( );
            auto itTo = to.begin( );

            // loop through both
            while (itFrom != from.end( ) && itTo != to.end( ) && *itTo == *itFrom)
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

        bool PathContainsFile(path directory, path file)
        {
            // if directory ends with "/" and isn't the root directory, then the final
            // component returned by iterators will include "." and will interfere
            // with the std::equal check below, so we strip it before proceeding
            if (directory.filename( ) == ".")
                directory.remove_filename( );

            // we're also not interested in the file's name.
            if (file.has_filename( ))
                file.remove_filename( );

            // if directory has more components than file, then file can't possibly
            // reside in the directory
            auto directoryLength = std::distance( directory.begin( ), directory.end( ) );
            auto fileLength = std::distance( file.begin( ), file.end( ) );

            if (directoryLength > fileLength)
                return false;

            // this stops checking when it reaches directory.end(), so it's OK if file
            // has more directory components afterward. They won't be checked.
            return std::equal( directory.begin( ), directory.end( ), file.begin( ) );
        }
    }
}