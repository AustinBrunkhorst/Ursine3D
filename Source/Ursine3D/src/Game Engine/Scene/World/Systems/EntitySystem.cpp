/* ---------------------------------------------------------------------------
** Team Bear King
** � 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EntitySystem.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/


#include "UrsinePrecompiled.h"

#include "EntitySystem.h"
#include "World.h"

#include "TimerManager.h"
#include "TweenManager.h"

namespace ursine
{
    namespace ecs
    {
        EntitySystem::EntitySystem(World *world) 
            : m_world( world )
            , m_timers( GetCoreSystem( TimerManager ) )
            , m_tweens( GetCoreSystem( TweenManager ) )
            , m_typeID( 0 )
        { }
    }
}