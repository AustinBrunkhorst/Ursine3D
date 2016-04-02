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

        EntityHandle::EntityHandle(const EntityHandle &rhs)
            : m_id( rhs.m_id )
            , m_version( rhs.m_version )
            , m_manager( rhs.m_manager )
        {
        }

        EntityHandle::EntityHandle(const EntityHandle &&rhs)
            : m_id( std::move( rhs.m_id ) )
            , m_version( std::move( rhs.m_version ) )
            , m_manager( std::move( rhs.m_manager ) )
        {

        }

        EntityHandle &EntityHandle::operator=(const EntityHandle &rhs)
        {
            m_id = rhs.m_id;
            m_version = rhs.m_version;
            m_manager = rhs.m_manager;

            return *this;
        }

        EntityHandle &EntityHandle::operator=(const EntityHandle &&rhs)
        {
            m_id = std::move( rhs.m_id );
            m_version = std::move( rhs.m_version );
            m_manager = std::move( rhs.m_manager );

            return *this;
        }

        EntityHandle::EntityHandle(Entity *entity)
            : m_id( entity ? entity->m_id : 0 )
            , m_version( entity ? entity->m_version : 0 )
            , m_manager( entity ? entity->m_world->m_entityManager : nullptr ) { }

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

        bool EntityHandle::operator==(const Entity *rhs) const
        {
            return Get( ) == rhs;
        }

        bool EntityHandle::operator!=(const EntityHandle &rhs) const
        {
            return !(m_version == rhs.m_version &&
                     m_id == rhs.m_id &&
                     m_manager == rhs.m_manager);
        }

        bool EntityHandle::operator!=(const Entity *rhs) const
        {
            return Get( ) != rhs;
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

            auto *entity = m_manager->GetEntity( *this );

            if (entity && entity->IsActive( ))
                return entity;

            return nullptr;
        }
    }
}