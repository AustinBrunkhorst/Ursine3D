#include "UrsinePrecompiled.h"

#include "EntityHandle.h"

#include "EntityManager.h"

namespace ursine
{
    namespace ecs
    {
        EntityHandle::EntityHandle(void)
            : m_id( 0 )
            , m_version( 0 )
            , m_manager( nullptr ) { }

        EntityHandle::EntityHandle(const EntityManager *manager, Entity *entity)
            : m_id( entity->m_id )
            , m_version( entity->m_version )
            , m_manager( manager ) { }

        EntityHandle::operator bool(void) const 
        {
            return IsValid( );
        }

        bool EntityHandle::operator==(const EntityHandle &rhs) const
        {
            return m_version == rhs.m_version &&
                   m_id == rhs.m_id &&
                   m_manager == rhs.m_manager;
        }

        bool EntityHandle::operator!=(const EntityHandle &rhs) const
        {
            return !(m_version == rhs.m_version &&
                     m_id == rhs.m_id &&
                     m_manager == rhs.m_manager);
        }

        Entity *EntityHandle::operator->(void) const 
        {
            return Get( );
        }

        const EntityHandle &EntityHandle::Invalid(void)
        {
            static EntityHandle invalid;

            return invalid;
        }

        bool EntityHandle::IsValid(void) const
        {
            return Get( ) != nullptr;
        }

        Entity *EntityHandle::Get(void) const
        {
            if (!m_manager)
                return nullptr;

            return m_manager->GetEntity( *this );
        }
    }
}