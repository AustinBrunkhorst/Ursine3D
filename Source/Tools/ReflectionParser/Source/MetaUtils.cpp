#include "Precompiled.h"

#include "MetaUtils.h"

#include <Utils.h>

#include <iostream>

namespace utils
{
    void ToString(const CXString &str, std::string &output)
    {
        auto cstr = clang_getCString( str );

        output = cstr;

        clang_disposeString( str );
    }

    TemplateData::Type TemplateBool(bool value)
    {
        return value ? TemplateData::Type::True : TemplateData::Type::False;
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
        std::ifstream input( filename );

        if (!input)
        {
            std::stringstream error;

            error << "Unable to open file \"" << filename << "\" for reading." << std::endl;
            error << strerror( errno );

            throw std::exception( error.str( ).c_str( ) );
        }

        auto contents = std::make_shared<std::string>( );

        input.seekg( 0, std::ios::end );

        contents->reserve(
            static_cast<std::string::size_type>( input.tellg( ) )
        );

        input.seekg( 0, std::ios::beg );

        contents->assign(
            (std::istreambuf_iterator<char>( input )),
            std::istreambuf_iterator<char>( ) 
        );

        input.close( );

        return contents;
    }

    void WriteText(const std::string &filename, const std::string &text)
    {
        std::ofstream output( filename );

        if (!output)
        {
            std::stringstream error;

            error << "Unable to open file \"" << filename << "\" for writing." << std::endl;
            error << strerror( errno );

            throw std::exception( error.str( ).c_str( ) );
        }

        output << text;

        output.close( );
    }

    void FatalError(const std::string &error)
    {
        std::cerr << "Error: " << error << std::endl;

        exit( EXIT_FAILURE );
    }
}