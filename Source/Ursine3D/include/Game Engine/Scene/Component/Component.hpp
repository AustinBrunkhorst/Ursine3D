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

namespace ursine
{
    namespace ecs
    {
        Component::Component(ComponentTypeID typeID)
            : m_typeID( typeID )
            , m_owner( nullptr ) 
        #if defined(URSINE_WITH_EDITOR)
            , m_baseInitialized( false )
        #endif
            { }

        ComponentTypeID Component::GetTypeID(void) const
        {
            return m_typeID;
        }

        ComponentTypeMask Component::GetTypeMask(void) const
        {
            return 1ull << m_typeID;
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

	    template<class ComponentType>
		Component::Handle<ComponentType>::Handle(void)
			: m_entity( nullptr )
		{
		}

		template<class ComponentType>
		Component::Handle<ComponentType>::Handle(const Handle<ComponentType> &other)
			: m_entity( other.m_entity )
		{
		}

		template<class ComponentType>
		Component::Handle<ComponentType>::Handle(const ComponentType *other)
		{
			if (other)
			{
				m_entity = other->GetOwner();

				UAssert(
					m_entity != nullptr, 
					"If the owner of the component is null, you're probably constructing"
					" this handle inside the component's constructor."
				);
			}
			else
				m_entity = nullptr;
		}

		template<class ComponentType>
		Component::Handle<ComponentType>::~Handle(void)
		{
			m_entity = nullptr;
		}

	    template<class ComponentType>
		const ComponentType* Component::Handle<ComponentType>::operator=(const ComponentType* rhs)
		{
			if (rhs)
			{
				m_entity = rhs->GetOwner( );

				return operator->( );
			}
			else
			{
				m_entity = nullptr;

				return nullptr;
			}
		}

		template<class ComponentType>
		const Component::Handle<ComponentType> &Component::Handle<ComponentType>::operator=(const Handle<ComponentType>& rhs)
		{
			m_entity = rhs.m_entity;

			return *this;
		}

	    template<class ComponentType>
		bool Component::Handle<ComponentType>::operator==(const ComponentType* rhs) const
		{
			if (m_entity)
				return operator->( ) == rhs;
			else
				return rhs == nullptr;
		}

		template<class ComponentType>
		bool Component::Handle<ComponentType>::operator==(const Handle<ComponentType> &rhs) const
		{
			return m_entity == rhs.m_entity;
		}

        template<class ComponentType>
        bool Component::Handle<ComponentType>::operator!=(const ComponentType* rhs) const
        {
            if ( m_entity )
                return operator->( ) != rhs;
            else
                return rhs != nullptr;
        }

        template<class ComponentType>
        bool Component::Handle<ComponentType>::operator!=(const Handle<ComponentType> &rhs) const
        {
            return m_entity != rhs.m_entity;
        }


		template<class ComponentType>
	    Component::Handle<ComponentType>::operator bool(void) const
	    {
			return m_entity && m_entity->HasComponent<ComponentType>( );
	    }

		template<class ComponentType>
		ComponentType& Component::Handle<ComponentType>::operator*(void)
		{
			return *operator->( );
		}

		template<class ComponentType>
		const ComponentType& Component::Handle<ComponentType>::operator*(void) const
		{
			return *operator->( );
		}

	    template<class ComponentType>
		ComponentType *Component::Handle<ComponentType>::operator->(void)
		{
			return m_entity->GetComponent<ComponentType>( );
		}

		template<class ComponentType>
		const ComponentType* Component::Handle<ComponentType>::operator->(void) const
		{
			return m_entity->GetComponent<ComponentType>( );
		}

		extern template Transform *Component::Handle<Transform>::operator->(void);
		extern template const Transform *Component::Handle<Transform>::operator->(void) const;
    }
}