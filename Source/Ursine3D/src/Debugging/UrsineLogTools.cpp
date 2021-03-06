/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** UrsineLogTools.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "UrsineLogTools.h"

#include "UrsineConsole.h"

#include <boost/filesystem.hpp>

#ifdef PLATFORM_WINDOWS
// windows function crap
#pragma comment(lib, "user32.lib")

// used for outputting the stack on Windows
#include <dbghelp.h>
#pragma comment(lib, "dbghelp.lib")
#endif

#ifdef CONFIG_DEBUG
    #ifdef PLATFORM_WINDOWS
        #define URSINE_DEBUG_BREAK() if (IsDebuggerPresent()) { DebugBreak(); }
    #else
        #include <signal.h>
        #define DEBUG_BREAK() signal(SIGTRAP)
    #endif
#else
    #define URSINE_DEBUG_BREAK()
#endif

namespace
{
    void displayCrashMessageBox(void)
    {
        if (ursine::logging::gExitCode == EXIT_SUCCESS)
            return;

        std::wstringstream message;

        message << "Please contact a developer with the debug trace.";
        message << "\n\n";
        message << boost::filesystem::current_path( ) / WIDEN( URSINE_ERROR_LOG_FILE );

    #ifdef PLATFORM_WINDOWS

        MessageBoxW( nullptr, message.str( ).c_str( ), L"Unexpected Crash", MB_OK | MB_ICONERROR );

    #else

        URSINE_TODO( "Add messageboxes for this platform" );

    #endif
    }
}

namespace ursine
{
    namespace logging
    {
        int gExitCode = EXIT_SUCCESS;

        std::thread::id gMainThreadID;

        Initializer gInitialzer;

        Initializer::Initializer(void)
        {
            // let's hope this is on the main thread
            gMainThreadID = std::this_thread::get_id( );

        #if (URSINE_DISPLAY_CRASH_MESSAGEBOX)

            atexit( displayCrashMessageBox );

        #endif
        }

        void UncaughtAssertion(const AssertionException &exception)
        {
            auto &file = exception.GetFile( );
            auto &function = exception.GetFunction( );
            auto line = exception.GetLine( );
            auto &message = exception.GetErrorMessage( );

            static const auto format = "%s";

        #if (URSINE_OUTPUT_CONSOLE)
            Log( stdout, URSINE_UNCAUGHT_ASSERTION_LOG_HEADER, CC_TEXT_BRIGHT_RED, file, function, line, format, message.c_str( ) );
        #endif

        #if (URSINE_OUTPUT_FILE)
            auto fileOut = fopen( URSINE_ERROR_LOG_FILE, "at" );

            Log( fileOut, URSINE_UNCAUGHT_ASSERTION_LOG_HEADER, CC_TEXT_BRIGHT_RED, file, function, line, format, message.c_str( ) );

            fclose( fileOut );
        #endif

            ExitError( );
        }

        void OutputInfo(FILE *handle, URSINE_FFL_ARGS)
        {
            SetConsoleColor( CC_TEXT_GREEN );

            fprintf( handle, "File: " );

            SetConsoleColor( CC_TEXT_WHITE );

            fprintf( handle, "%s\n", boost::filesystem::path( file ).filename( ).string( ).c_str( ) );

            SetConsoleColor( CC_TEXT_GREEN );

            fprintf( handle, "Function: " );

            SetConsoleColor( CC_TEXT_WHITE );

            fprintf( handle, "%s\n", function.c_str( ) );

            SetConsoleColor( CC_TEXT_GREEN );

            fprintf( handle, "Line Number: " );

            SetConsoleColor( CC_TEXT_WHITE );

            fprintf( handle, "%i\n\n", line );

            OutputStack( handle );
            OutputThreadInfo( handle );
            OutputWDir( handle );
            OutputTime( handle );

            SetConsoleColor( CC_TEXT_BRIGHT_WHITE );
        }

        void OutputStack(FILE *handle)
        {
        #ifdef PLATFORM_WINDOWS

            auto process = GetCurrentProcess( );

            SymInitialize( process, nullptr, true );

            void *stack[100];

            auto frames = CaptureStackBackTrace( 0, 100, stack, nullptr );

            SYMBOL_INFO *symbol = static_cast<SYMBOL_INFO*>(
                malloc( sizeof(SYMBOL_INFO) + 256 * sizeof(char) ) );

            // well, this is ironic.
            if (!symbol || frames < 5)
                return;

            symbol->MaxNameLen = 255;
            symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

            SetConsoleColor( CC_TEXT_YELLOW );

            fprintf( handle, "Call Stack\n----------\n\n" );

            // start from 4 to skip the debug functions
            for (int i = 4; i < frames; i++)
            {
                SymFromAddr( process, reinterpret_cast<DWORD64>( stack[ i ] ), nullptr, symbol );

                SetConsoleColor( CC_TEXT_WHITE );

                fprintf( handle, " - " );

                SetConsoleColor( CC_TEXT_BRIGHT_GREEN );

                fprintf( handle, "%s ", symbol->Name );

                SetConsoleColor( CC_TEXT_WHITE );

                fputc( '(', handle );

                SetConsoleColor( CC_TEXT_BRIGHT_WHITE );

                fprintf( handle, "0x%0llX", symbol->Address );

                SetConsoleColor( CC_TEXT_WHITE );

                fprintf( handle, ")\n" );
            }

            // add extra newline for cleanliness
            fputc( '\n', handle );

            free( symbol );

            SymCleanup( process );

        #endif // PLATFORM_WINDOWS
        }

        void OutputWDir(FILE *handle)
        {
            SetConsoleColor( CC_TEXT_YELLOW );

            fprintf( handle, "Working Directory\n-----------------\n\n" );

            SetConsoleColor( CC_TEXT_WHITE );

            fprintf( handle, "%s\n\n", boost::filesystem::current_path( ).string( ).c_str( ) );
        }

        void OutputThreadInfo(FILE *handle)
        {
            SetConsoleColor( CC_TEXT_YELLOW );

            fprintf( handle, "Thread\n------\n\n" );

            SetConsoleColor( CC_TEXT_WHITE );

            fprintf( handle, "ID: " );

            SetConsoleColor( CC_TEXT_BRIGHT_WHITE );

            auto tid = std::this_thread::get_id( );

            std::stringstream stream;

            stream << tid;

            fprintf( handle, "%s\n", stream.str( ).c_str( ) );

            SetConsoleColor( CC_TEXT_WHITE );

            fprintf( handle, "Main Thread: " );

            SetConsoleColor( CC_TEXT_BRIGHT_WHITE );

            fprintf( handle, "%s\n\n", tid == gMainThreadID ? "yes" : "no" );
        }

        void ExitError(void)
        {
            // focus on the console
        #ifdef PLATFORM_WINDOWS

            SetForegroundWindow( GetConsoleWindow( ) );

        #endif

            /*
            * If you're seeing this, it's because you're in debug mode
            * and an assertion failed or a fatal error was thrown.
            * Check out the console, or look at the call stack.
            **/
            URSINE_DEBUG_BREAK( );

            // make sure we show the message box
            exit( gExitCode = EXIT_FAILURE );
        }

        void OutputTime(FILE *handle)
        {
            char buffer[80];

            time_t current_time;

            time( &current_time );

            auto info = localtime( &current_time );

            strftime( buffer, 80, "%x - %I:%M%p", info );

            size_t padding_left = 48 - strlen( buffer );

            // align this puppy to the right
            for (int i = 0; i < padding_left; ++i)
                fputc( ' ', handle );

            SetConsoleColor( CC_TEXT_WHITE );

            fprintf( handle, "%s\n", buffer );
        }
    }
}
