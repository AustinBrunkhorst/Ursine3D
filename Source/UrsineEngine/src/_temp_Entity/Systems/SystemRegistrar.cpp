/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SystemRegistrar.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "SystemRegistrar.h"

namespace ursine
{
    namespace ecs
    {
        SystemTypeID SystemRegistrar::_next_id = 0;
        SystemTypeMask SystemRegistrar::_next_mask = 1;
        
        SystemNameMap &SystemRegistrar::getNameMap(void)
        {
            static SystemNameMap names;

            return names;
        }

        SystemFactoryMap &SystemRegistrar::GetFactories(void)
        {
            static SystemFactoryMap factories;

            return factories;
        }

        SystemTypeID SystemRegistrar::GetID(const std::string &name)
        {
            auto &names = getNameMap();

            auto search = names.find(name);

            UAssert(search != names.end(), 
                "Unknown system type '%s'.",
                name.c_str());

            return search->second;
        }
    }
}