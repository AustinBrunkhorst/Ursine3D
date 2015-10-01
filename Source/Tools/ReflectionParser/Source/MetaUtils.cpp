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

    std::string GetQualifiedName(const CursorType &type)
    {
        if (type.GetKind() != CXType_Typedef)
            return type.GetDisplayName( );

        auto declaration = type.GetDeclaration( );

        auto parent = declaration.GetLexicalParent( );

        Namespace parentNamespace;

        // walk up to the root namespace
        while (parent.GetKind( ) == CXCursor_Namespace)
        {
            parentNamespace.emplace( parentNamespace.begin( ), parent.GetDisplayName( ) );

            parent = parent.GetLexicalParent( );
        }

        // add the display name as the end of the namespace
        parentNamespace.emplace_back(
            type.GetDisplayName( )
        );

        std::string qualifiedName;

        ursine::utils::Join( parentNamespace, "::", qualifiedName );

        return qualifiedName;
    }

    std::string GetQualifiedName(
        const std::string &displayName, 
        const Namespace &currentNamespace
    )
    {
        std::string name;

        ursine::utils::Join( currentNamespace, "::", name );

        if (!currentNamespace.empty( ))
            name += "::";

        name += displayName;

        return name;
    }

    std::string GetQualifiedName(
        const Cursor &cursor, 
        const Namespace &currentNamespace
    )
    {
        return GetQualifiedName( cursor.GetSpelling( ), currentNamespace );
    }

    void LoadText(const std::string &filename, std::string &output)
    {
        std::ifstream input( filename );

        if (!input)
        {
            std::stringstream error;

            error << "Unable to open file \"" 
                  << filename 
                  << "\" for reading." << std::endl;
            error << strerror( errno );

            throw std::exception( error.str( ).c_str( ) );
        }

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
    }

    void WriteText(const std::string &filename, const std::string &text)
    {
        std::ofstream output( filename );

        if (!output)
        {
            std::stringstream error;

            error << "Unable to open file \"" 
                  << filename << "\" for writing." 
                  << std::endl;
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