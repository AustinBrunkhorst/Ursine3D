/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Main.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include <Application.h>

#include "UISubProcess.h"

#include <ReflectionDatabase.h>
#include <UrsineReflectionModule.h>
#include <SharedReflectionModule.h>
#include "EditorReflectionModule.h"

int main(int argc, char *argv[])
{
    try
    {
        MetaInitialize( META_MODULE_INITIALIZER );

        UI_MAIN( argc, argv );

        ursine::Application app( argc, argv );

        app.Run( );
    }
    catch (ursine::AssertionException &e)
    {
        ursine::logging::UncaughtAssertion( e );
    }

    return 0; 
}
