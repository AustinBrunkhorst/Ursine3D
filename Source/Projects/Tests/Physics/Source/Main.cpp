#include "Precompiled.h"

#include <Application.h>
#include <UISubProcess.h>

int main(int argc, char *argv[])
{
    {
#if defined(PLATFORM_WINDOWS)

        CefMainArgs mainArgs( GetModuleHandle( nullptr ) ); 
#else
       
        CefMainArgs mainArgs( argc, argv ); 

#endif

        CefRefPtr<ursine::UISubProcess> app( new ursine::UISubProcess );

        auto result = CefExecuteProcess( mainArgs, app, nullptr );

        if (result >= 0)
            return result;
    }

    ursine::logging::Initialize( );

    ursine::Application app( argc, argv );

    app.Run( ); 

    return 0; 
}