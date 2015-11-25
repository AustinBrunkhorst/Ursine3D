#include "Precompiled.h"

#include <Application.h>

#include "UISubProcess.h"

int main(int argc, char *argv[])
{
    auto type = typeof( unsigned );
    auto derp = type.GetName( );
    UI_MAIN( argc, argv );

    ursine::Application app( argc, argv );

    app.Run( ); 
    
    return 0; 
}