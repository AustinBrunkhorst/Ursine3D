/* ---------------------------------------------------------------------------
** Team Bear King
** � 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** WorldManager.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "WorldManager.h"
#include "World.h"

namespace ursine
{
    namespace ecs
    {
        WorldManager::WorldManager(World *world)
            : m_world( world ) { }
    }
}