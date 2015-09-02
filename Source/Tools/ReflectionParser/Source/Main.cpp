#include "Precompiled.h"

#include "ReflectionOptions.h"
#include "ReflectionParser.h"

#include <iostream>

void fatalError(const std::string &error);

void parse(const po::variables_map &cmdLine);

int main(int argc, char *argv[])
{
    // misc initialization
    {
        // path to the executable
        auto exeDir = fs::path( argv[ 0 ] );

        // set the working directory to the executable directory
        fs::current_path( exeDir.parent_path( ) );
    }

    // parse command line
    try 
    {
        po::options_description program( "Ursine3D Reflection Parser" );

        program.add_options( )
            ( "help,h", "Displays help information." )
            ( "target-name,t", po::value<std::string>( )->required( ), "Input target project name." )
            ( "source-in,i", po::value<std::string>( )->required( ), "Source file (header) to compile reflection data from." )
            ( "header-out,a", po::value<std::string>( )->required( ), "Output generated C++ header file." )
            ( "source-out,b", po::value<std::string>( )->required( ), "Output generated C++ source file." )
            ( "flag,f", po::value<std::vector<std::string>>( )->multitoken( ), "Optional list of flags to pass to the compiler." );

        po::variables_map cmdLine;

        po::store( po::parse_command_line( argc, argv, program ), cmdLine );

        if (cmdLine.count( "help" )) 
        {
            std::cout << program << std::endl;

            return 0;
        }

        po::notify( cmdLine );

        parse( cmdLine );
    }
    catch (std::exception &e) 
    {
        std::cerr << "error: " << e.what( ) << std::endl;

        return EXIT_FAILURE;
    }
    catch (...) 
    {
        std::cerr << "Unhandled exception occurred!" << std::endl;
    }

    return 0;
}

void fatalError(const std::string &error)
{
    std::cerr << error << std::endl;

    exit( EXIT_FAILURE );
}

void parse(const po::variables_map &cmdLine)
{
    ReflectionOptions options;

    options.targetName = cmdLine.at( "target-name" ).as<std::string>( );
    options.sourceFile = cmdLine.at( "source-in" ).as<std::string>( );
    options.outputHeaderFile = cmdLine.at( "header-out" ).as<std::string>( );
    options.outputSourceFile = cmdLine.at( "source-out" ).as<std::string>( );

    // default arguments
    options.arguments =
    { {
        "-x",
        "c++",
        "-std=c++11"
    } };

    if (cmdLine.count( "flag" ))
    {
        auto flags = cmdLine.at( "flag" ).as<std::vector<std::string>>( );

        for (auto flag : flags)
            options.arguments.emplace_back( flag.c_str( ) );
    }
    
    ReflectionParser parser( options );

    parser.Parse( );

    // header template
    {
        auto headerTemplateText = utils::LoadText( "Templates/Header.mustache" );

        if (!headerTemplateText)
            fatalError( "Unable to load header template." );

        TemplateData headerTemplateData { TemplateData::Type::Object };

        headerTemplateData[ "targetName" ] = options.targetName;
        headerTemplateData[ "sourceFile" ] = options.sourceFile;

        MustacheTemplate headerTemplate{ *headerTemplateText };

        if (!headerTemplate.isValid( ))
        {
            std::stringstream error;

            error << "Error compiling header template." << std::endl;
            error << headerTemplate.errorMessage( );

            fatalError( error.str( ) );
        }

        auto output = headerTemplate.render( headerTemplateData );
    }

    // source template
    {
        auto sourceTemplateText = utils::LoadText( "Templates/Source.mustache" );

        if (!sourceTemplateText)
            fatalError( "Unable to load source template." );

        TemplateData sourceTemplateData { TemplateData::Type::Object };

        MustacheTemplate sourceTemplate { *sourceTemplateText };

        if (!sourceTemplate.isValid( ))
        {
            std::stringstream error;

            error << "Error compiling source template." << std::endl;
            error << sourceTemplate.errorMessage( );

            fatalError( error.str( ) );
        }

        auto output = sourceTemplate.render( sourceTemplateData );
    }
}