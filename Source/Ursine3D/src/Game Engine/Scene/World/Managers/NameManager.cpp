/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
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

        void NameManager::Remove(Entity *entity)
        {
            auto search = m_names.find( entity->m_uniqueID );

            if (search == m_names.end( ))
                return;

            removeFromGroup( search->second, entity );

            m_names.erase( search );
        }

        Entity *NameManager::GetEntity(const std::string &name)
        {
            auto search = m_grouped.find( name );

            if (search == m_grouped.end( ) || search->second.empty( ))
                return nullptr;

            return search->second.front( );
        }

        const EntityVector &NameManager::GetEntities(const std::string &name)
        {
            return m_grouped[ name ];
        }

        const std::string &NameManager::GetName(const Entity *entity)
        {
            return m_names[ entity->m_uniqueID ];
        }

        void NameManager::SetName(Entity *entity, const std::string &name)
        {
            setName( entity, name );

            EditorEntityNameChangedArgs e( 
                WORLD_EDITOR_ENTITY_NAME_CHANGED, 
                entity, 
                name 
            );

            m_world->Dispatch( WORLD_EDITOR_ENTITY_NAME_CHANGED, &e );
        }

        void NameManager::setName(Entity *entity, const std::string &name)
        {
            auto nameEntry = m_names.find( entity->m_uniqueID );

            // first time setting the name
            if (nameEntry == m_names.end( ))
            {
                m_names[ entity->m_uniqueID ] = name;
            }
            else
            {
                removeFromGroup( nameEntry->second, entity );

                // update name
                nameEntry->second = name;
            }

            // add new grouped name
            m_grouped[ name ].emplace_back( entity );
        }

        void NameManager::removeFromGroup(const std::string &name, Entity *entity)
        {
            auto &group = m_grouped[ name ];

            group.erase( find( group.begin( ), group.end( ), entity ) );
        }
    }
}
