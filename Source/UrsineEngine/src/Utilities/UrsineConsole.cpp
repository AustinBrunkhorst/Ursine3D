/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** UrsineConsole.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "UrsineConsole.h"

namespace Ursine
{
    void SetConsoleColor(ConsoleColor color)
    {
    #ifdef PLATFORM_WINDOWS
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    #endif
    }
}