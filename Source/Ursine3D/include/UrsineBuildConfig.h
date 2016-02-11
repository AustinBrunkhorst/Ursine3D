/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** UrsineBuildConfig.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#if defined(_DEBUG)
    #if !defined(CONFIG_DEBUG)
        #define CONFIG_DEBUG
    #endif
#else
    #if !defined(CONFIG_RELEASE)
        #define CONFIG_RELEASE
    #endif
#endif