/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SystemRegistrar.hpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/


#include "SystemType.h"

namespace ursine
{
    namespace ecs
    {
        template<class Type>
        void SystemRegistrar::Register(const char *name, SystemFactory factory, bool auto_add)
        {
            static_assert(std::is_base_of<EntitySystem, Type>::value,
                "The specified type is not an entity system!");

            // we've reached the max number of bits we can fit in the component type mask.
            // if this becomes a limitation, we can simply change
            // ComponentTypeMask to std::bitset<N> where is N is a larger number
            // of bits than the current typedef. Obviously bitwise operations
            // on integral types are much faster than through the bitset class.
            UAssert(_next_id < MAX_ENTITY_SYSTEM_COUNT, 
                "Maximum number of entity system types exceeded.");

            // this system type has already been registered
            if (SystemType<Type>::Registered)
                return;

            SystemType<Type>::ID = _next_id;
            SystemType<Type>::Mask = _next_mask;
            SystemType<Type>::Registered = true;
            SystemType<Type>::Factory = factory;

            GetFactories()[_next_id] = { auto_add, factory };

            getNameMap()[name] = _next_id;

            ++_next_id;

            _next_mask <<= 1;
        }

        template<class Type>
        inline SystemTypeID SystemRegistrar::GetID(void)
        {
            static_assert(std::is_base_of<EntitySystem, Type>::value,
                "The specified type is not an entity system!");

            return SystemType<Type>::ID;
        }

        template<class Type>
        inline SystemTypeMask SystemRegistrar::GetMask(void)
        {
            static_assert(std::is_base_of<EntitySystem, Type>::value,
                "The specified type is not an entity system!");

            return SystemType<Type>::Mask;
        }

        template<class Type>
        inline bool SystemRegistrar::IsRegistered(void)
        {
            static_assert(std::is_base_of<EntitySystem, Type>::value,
                "The specified type is not an entity system!");

            return SystemType<Type>::Registered;
        }
    }
}