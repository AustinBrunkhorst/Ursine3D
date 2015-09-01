#include "Precompiled.h"

#include "ReflectionOptions.h"
#include "ReflectionParser.h"

#include <iostream>

int fatalError(const std::string &error);

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        std::stringstream error;

        error << "Expecting at least 4 arguments." << std::endl;
        error << "[target name], ";
        error << "[source file], ";
        error << "[output header file], ";
        error << "[output source file] ";
        error << "[, compiler flags ...]" << std::endl;

        return fatalError( error.str( ) );
    }

    // misc initialization
    {
        ursine::logging::Initialize( );

        // path to the executable
        auto exeDir = fs::path( argv[ 0 ] );

        // set the working directory to the executable directory
        fs::current_path( exeDir.parent_path( ) );
    }

    ReflectionOptions options;

    options.targetName = argv[ 1 ];
    options.sourceFile = argv[ 2 ];
    options.outputHeaderFile = argv[ 3 ];
    options.outputSourceFile = argv[ 4 ];

    // default arguments
    options.arguments =
    { {
        "-x",
        "c++",
        "-std=c++11"
    } };

    ReflectionParser parser( options );

    parser.Parse( );

    // header template
    {
        auto headerTemplateText = utils::LoadText( "Templates/Header.mustache" );

        if (!headerTemplateText)
            return fatalError( "Unable to load header template." );

        TemplateData headerTemplateData { TemplateData::Type::Object };

        headerTemplateData[ "targetName" ] = options.targetName;
        headerTemplateData[ "sourceFile" ] = options.sourceFile;

        MustacheTemplate headerTemplate { *headerTemplateText };

        if (!headerTemplate.isValid( ))
        {
            std::stringstream error;

            error << "Error compiling header template." << std::endl;
            error << headerTemplate.errorMessage( );

            return fatalError( error.str( ) );
        }

        auto output = headerTemplate.render( headerTemplateData );
    }

    // source template
    {
        auto sourceTemplateText = utils::LoadText( "Templates/Source.mustache" );

        if (!sourceTemplateText)
            return fatalError( "Unable to load source template." );

        TemplateData sourceTemplateData { TemplateData::Type::Object };

        MustacheTemplate sourceTemplate { *sourceTemplateText };

        if (!sourceTemplate.isValid( ))
        {
            std::stringstream error;

            error << "Error compiling source template." << std::endl;
            error << sourceTemplate.errorMessage( );

            return fatalError( error.str( ) );
        }

        auto output = sourceTemplate.render( sourceTemplateData );
    }

    return 0;
}

int fatalError(const std::string &error)
{
    std::cerr << error << std::endl;

    return EXIT_FAILURE;
}