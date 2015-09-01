/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ComponentType.hpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "ComponentRegistrar.h"

namespace ursine
{
    namespace ecs
    {
        template<class Type>
        ComponentTypeID ComponentType<Type>::ID = 0;

        template<class Type>
        ComponentTypeMask ComponentType<Type>::Mask = 0;

        template<class Type>
        bool ComponentType<Type>::Registered = false;

        template<class Type>
        ComponentType<Type>::ComponentType(const char *name)
        {
            ComponentRegistrar::Register<Type>(name);
        }

        template<class Type>
        ComponentType<Type>::ComponentType(
            const char *member_name,
            ComponentDataAccessor accessor,
            JsonSerializer::Serializer serializer,
            JsonSerializer::Deserializer deserializer,
            const char *display_name = nullptr)
        {
            ComponentRegistrar::RegisterMember<Type>(display_name ? display_name : member_name, accessor,
                serializer, deserializer);
        }

        template<class Type>
        Component *ComponentType<Type>::Create(void)
        {
            // by default, use the default constructor
            return new Type();
        }

        template<class Type>
        Component *ComponentType<Type>::Copy(const Component *instance)
        {
            static_assert(std::is_copy_constructible<Type>::value,
                "This component type doesn't have a copy constructor!");

            // by default, use the copy constructor
            return new Type(*(static_cast<const Type*>(instance)));
        }
    }
}