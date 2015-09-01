#include "Precompiled.h"

#include <Utils.h>

namespace utils
{
    void ToString(const CXString &str, std::string &output)
    {
        auto cstr = clang_getCString( str );

        output = cstr;

        clang_disposeString( str );
    }

    std::string GetQualifiedName(const std::string &displayName, const Namespace &currentNamespace)
    {
        std::string name;

        ursine::utils::Join( currentNamespace, "::", name );

        if (!currentNamespace.empty( ))
            name += "::";

        name += displayName;

        return name;
    }

    std::string GetQualifiedName(const Cursor &cursor, const Namespace &currentNamespace)
    {
        return GetQualifiedName( cursor.GetSpelling( ), currentNamespace );
    }

    std::shared_ptr<std::string> LoadText(const std::string &filename)
    {
        std::ifstream stream( filename, std::ios::in );

        if (stream)
        {
            auto contents = std::make_shared<std::string>( );

            stream.seekg( 0, std::ios::end );

            contents->reserve(
                static_cast<std::string::size_type>( stream.tellg( ) )
            );

            stream.seekg( 0, std::ios::beg) ;

            contents->assign(
                (std::istreambuf_iterator<char>( stream )),
                std::istreambuf_iterator<char>( ) 
            );

            stream.close( );

            return contents;
        }

        return nullptr;
    }
}