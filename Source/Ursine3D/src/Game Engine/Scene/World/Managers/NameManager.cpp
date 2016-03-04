/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** GroupManager.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "NameManager.h"
#include "Entity.h"

namespace ursine
{
    namespace ecs
    {
        NameManager::NameManager(World *world)
            : WorldManager( world ) { }

        NameManager::~NameManager(void) { }

        void NameManager::Remove(EntityID id)
        {
            auto search = m_names.find( id );

            if (search == m_names.end( ))
                return;

            removeFromGroup( search->second, id );

            // reset the name
            search->second = "";
        }

        EntityHandle NameManager::GetEntity(const std::string &name) const
        {
            auto search = m_grouped.find( name );

            if (search == m_grouped.end( ) || search->second.empty( ))
                return EntityHandle::Invalid( );

            return m_world->GetEntityManager( )->GetEntityByID( search->second.front( ) );
        }

        EntityHandleVector NameManager::GetEntities(const std::string &name) const
        {
            auto *entityManager = m_world->GetEntityManager( );

            EntityHandleVector entities;

            auto search = m_grouped.find( name );

            if (search == m_grouped.end( ))
                return entities;

            for (auto id : search->second)
                entities.emplace_back( entityManager->GetEntityByID( id ) );

            return entities;
        }

        const std::string &NameManager::GetName(EntityID id)
        {
            return m_names[ id ];
        }

        void NameManager::SetName(EntityID id, const std::string &name)
        {
            setName( id, name );

        #if defined(URSINE_WITH_EDITOR)

            EditorEntityNameChangedArgs e( 
                WORLD_EDITOR_ENTITY_NAME_CHANGED, 
                m_world->GetEntityManager( )->GetEntityByID( id ),
                name 
            );

            m_world->Dispatch( WORLD_EDITOR_ENTITY_NAME_CHANGED, &e );

        #endif
        }

        void NameManager::setName(EntityID id, const std::string &name)
        {
            auto nameEntry = m_names.find( id );

            // first time setting the name
            if (nameEntry == m_names.end( ))
            {
                m_names[ id ] = name;
            }
            else
            {
                removeFromGroup( nameEntry->second, id );

                // update name
                nameEntry->second = name;
            }

            // add new grouped name
            m_grouped[ name ].emplace_back( id );
        }

        void NameManager::removeFromGroup(const std::string &name, EntityID id)
        {
            auto &group = m_grouped[ name ];

			auto search = find( group.begin( ), group.end( ), id );

			if (search != group.end( ))
				group.erase( search );
        }
    }
}
