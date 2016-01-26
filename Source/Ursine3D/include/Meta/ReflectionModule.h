/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ReflectionModule.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "ReflectionDatabase.h"

namespace ursine
{
    namespace meta
    {
        class ReflectionModule
        {
        public:
            ReflectionModule(ReflectionDatabase &db);

        protected:
            ReflectionDatabase &db;
        };
    }
}