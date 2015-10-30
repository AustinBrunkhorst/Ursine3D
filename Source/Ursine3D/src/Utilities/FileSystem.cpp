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
    }
}