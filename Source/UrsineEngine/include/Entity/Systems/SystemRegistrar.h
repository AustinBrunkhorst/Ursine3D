/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SystemRegistrar.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "SystemConfig.h"

#include <vector>

namespace ursine
{
    namespace ecs
    {
        // forward declaration
        class EntitySystem;

        // type ID -> auto add, factory
        typedef std::unordered_map<SystemTypeID, std::pair<bool, SystemFactory>> SystemFactoryMap;
        
        // name -> type ID
        typedef std::unordered_map<std::string, SystemTypeID> SystemNameMap;

        class SystemRegistrar final
        {
            static SystemTypeID _next_id;
            static SystemTypeMask _next_mask;

            static SystemNameMap &getNameMap(void);

        public:
            template<class Type>
            static void Register(const char *name, SystemFactory factory, bool auto_add);

            // Gets the unique ID for this system type
            template<class Type>
            static inline SystemTypeID GetID(void);

            // Gets the unique type mask for this system type
            template<class Type>
            static inline SystemTypeMask GetMask(void);

            template<class Type>
            static inline bool IsRegistered(void);

            static SystemFactoryMap &GetFactories(void);

            static SystemTypeID GetID(const std::string &name);
        };
    }
}

#include "SystemRegistrar.hpp"