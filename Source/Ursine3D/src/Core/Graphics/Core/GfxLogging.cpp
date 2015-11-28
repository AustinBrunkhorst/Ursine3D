/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** GfxLogging.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "GfxLogging.h"

namespace ursine
{
    namespace graphics
    {

        const std::string GetDateTime()
        {
            //time_t     now = time( 0 );
            //struct tm  tstruct;
            //char       buf[ 128 ];
            //char       extra[ 128 ];
            //tstruct = *localtime( &now );

            //strftime( buf, sizeof( buf ), "%Y-%m-%d_", &tstruct );
            //sprintf( extra, "%i-%i-%i", tstruct.tm_hour % 12, tstruct.tm_min, tstruct.tm_sec );
            //strcat( buf, extra );
            //return buf;

            return "";
        }

        void LogMessage(char *message, int tabCount, ...)
        {
#ifndef _DEBUG
            return;
#endif

            //std::ofstream output;
            //using namespace std::chrono;
            ////if we haven't made it yet, make it
            //if (filename.size( ) == 0)
            //{
            //  CreateDirectory( L"../LOGS/", nullptr );
            //  filename = "../LOGS/";
            //  std::string Final = GetDateTime( );
            //  filename += Final;
            //  filename += ".LOG";

            //  output.open( filename.c_str( ) );
            //  output << "--------------------------------------------GFX Log File--------------------------------------------" << std::endl;
            //  output << "                                    Date/Time:";
            //  output << Final << std::endl;
            //  output << "     Timestamp     |                                    Message                                     " << std::endl;
            //  output << "-------------------|--------------------------------------------------------------------------------" << std::endl;
            //  output.close( );

            //  tp = std::chrono::high_resolution_clock::now( );
            //}

            //char FinalMessage[ 1024 ];
            //memset( FinalMessage, 0, 1024 );
            //va_list ARGS;

            //va_start( ARGS, tabCount );
            //vsprintf( FinalMessage, message, ARGS );
            //va_end( ARGS );

            ////time
            //high_resolution_clock::time_point t2 = high_resolution_clock::now( );
            //duration<double> time_span = duration_cast<duration<double>>(t2 - tp);

            ////append output
            //output.open( filename.c_str( ), std::ofstream::app );
            //std::cout.width( 20 );
            //output << std::setfill( '0' ) << std::setw( 15 ) << std::setiosflags( std::ios::fixed ) << std::setprecision( 5 ) << time_span.count( );
            //output << "    | ";
            //for (int x = 0; x < tabCount * 2; ++x)
            //  output << " ";
            //output << FinalMessage << std::endl;
            //output.close( );

            return;
        }
    }
}