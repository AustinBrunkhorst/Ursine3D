/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** UrsinePlatform.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#ifdef _WIN32
#define PLATFORM_WINDOWS
#elif __unix__
#define PLATFORM_UNIX
#elif __linux__
#define PLATFORM_LINUX
#elif __APPLE__
#define PLATFORM_OSX
#endif