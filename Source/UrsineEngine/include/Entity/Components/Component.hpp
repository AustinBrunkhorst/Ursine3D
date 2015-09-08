/* ---------------------------------------------------------------------------
** Team Bear King
** � 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Component.hpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

namespace ursine
{
    namespace ecs
    {
        Component::Component(ComponentTypeID typeID) 
            : m_typeID( typeID )
            , m_owner( nullptr )
            , m_typeMask( 1ull << typeID ) { }

        ComponentTypeID Component::GetTypeID(void) const
        {
            return m_typeID;
        }

        ComponentTypeMask Component::GetTypeMask(void) const
        {
            return m_typeMask;
        }

        Entity *Component::GetOwner(void) const
        {
            return m_owner;
        }

        template<class ComponentType>
        bool Component::Is(void) const
        {
            static_assert(std::is_base_of<Component, ComponentType>::value,
                "The specified type is not a component!");

            static const auto id = GetComponentID( ComponentType );

            return m_typeID == id;
        }
    }
}