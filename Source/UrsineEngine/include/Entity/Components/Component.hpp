/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Component.hpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

namespace Ursine
{
    namespace ECS
    {
        Component::Component(ComponentTypeID type_id) 
            : _type_id(type_id)
            , _owner(nullptr)
            , _type_mask(1ull << type_id) { }

        ComponentTypeID Component::GetTypeID(void) const
        {
            return _type_id;
        }

        ComponentTypeMask Component::GetTypeMask(void) const
        {
            return _type_mask;
        }

        Entity *Component::GetOwner(void) const
        {
            return _owner;
        }

        const std::string &Component::GetName(void) const
        {
            return ComponentRegistrar::Reflect(_type_id).name;
        }

        template<class ComponentType>
        bool Component::Is(void) const
        {
            static_assert(std::is_base_of<Component, ComponentType>::value,
                "The specified type is not a component!");

            static const auto id = GetComponentID(ComponentType);

            return _type_id == id;
        }
    }
}