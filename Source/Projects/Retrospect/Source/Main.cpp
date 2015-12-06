/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Main.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include <Application.h>

#include <UISubProcess.h>

int main(int argc, char *argv[])
{
    UI_MAIN( argc, argv );

    ursine::Application app( argc, argv );

    app.Run( );

    return 0;
}
