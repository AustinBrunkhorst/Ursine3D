/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ReflectionModule.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "ReflectionModule.h"

namespace ursine
{
    namespace meta
    {
        ReflectionModule::ReflectionModule(ReflectionDatabase &db)
            : db( db ) { }
    }
}