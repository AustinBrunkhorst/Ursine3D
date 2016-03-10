/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** UrsineLogTools.hpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsineConsole.h"

namespace ursine
{
    namespace logging
    {
        template<typename... Args>
        void Assert(URSINE_FFL_ARGS, URSINE_LOG_FORMATTED)
        {
        #if (URSINE_OUTPUT_CONSOLE)
            Log( stdout, URSINE_ASSERTION_LOG_HEADER, CC_TEXT_YELLOW, file, function, line, format, args... );
        #endif

        #if (URSINE_OUTPUT_FILE)
            auto fileOut = fopen( URSINE_ERROR_LOG_FILE, "at" );

            Log( fileOut, URSINE_ASSERTION_LOG_HEADER, CC_TEXT_YELLOW, file, function, line, format, args... );

            fclose( fileOut );
        #endif

            ExitError( );
        }

        template<typename... Args>
        void Warning(URSINE_FFL_ARGS, URSINE_LOG_FORMATTED)
        {
        #if (URSINE_OUTPUT_CONSOLE)
            Log( stdout, URSINE_WARNING_LOG_HEADER, CC_TEXT_YELLOW, file, function, line, format, args... );
        #endif

        #if (URSINE_OUTPUT_FILE)
            auto fileOut = fopen( URSINE_ERROR_LOG_FILE, "at" );

            Log( fileOut, URSINE_WARNING_LOG_HEADER, CC_TEXT_YELLOW, file, function, line, format, args... );

            fclose( fileOut );
        #endif
        }

        template<typename... Args>
        void Error(URSINE_FFL_ARGS, URSINE_LOG_FORMATTED)
        {
        #if (URSINE_OUTPUT_CONSOLE)
            Log( stdout, URSINE_ERROR_LOG_HEADER, CC_TEXT_BRIGHT_RED, file, function, line, format, args... );
        #endif

        #if (URSINE_OUTPUT_FILE)
            auto fileOut = fopen( URSINE_ERROR_LOG_FILE, "at" );

            Log( fileOut, URSINE_ERROR_LOG_HEADER, CC_TEXT_BRIGHT_RED, file, function, line, format, args... );

            fclose( fileOut );
        #endif

            ExitError( );
        }

        template<typename... Args>
        void Log(
            FILE *handle, 
            const std::string &title,
            ConsoleColor titleColor, 
            URSINE_FFL_ARGS, 
            URSINE_LOG_FORMATTED
        )
        {
            std::string dashes( (46 - title.length( )) / 2, '-' );

            SetConsoleColor( CC_TEXT_BRIGHT_WHITE );

            fprintf( handle, "================================================\n" );

            SetConsoleColor( CC_TEXT_WHITE );

            fprintf( handle, (dashes + " ").c_str( ) );

            SetConsoleColor( titleColor );

            fprintf( handle, title.c_str( ) );

            SetConsoleColor( CC_TEXT_WHITE );

            if ((title.length( ) & 1) == 1)
                dashes += '-';

            fprintf( handle, (" " + dashes + "\n\n").c_str( ) );

            SetConsoleColor( CC_TEXT_BRIGHT_WHITE );

            fprintf( handle, format.c_str( ), args... );
            fprintf( handle, "\n\n" );

            OutputInfo( handle, file, function, line );

            SetConsoleColor( CC_TEXT_BRIGHT_WHITE );

            fprintf( handle, "================================================\n\n" );

            SetConsoleColor( CC_TEXT_WHITE );
        }
    }
}
