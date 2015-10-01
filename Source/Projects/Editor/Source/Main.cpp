#include "Precompiled.h"

#include <Application.h>

#include "UISubProcess.h"

int main(int argc, char *argv[])
{
    UI_MAIN( argc, argv );

    ursine::Application app( argc, argv );

    app.Run( ); 

    return 0; 
}