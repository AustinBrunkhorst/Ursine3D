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
            : WorldManager( world )
            , EventDispatcher( this ) { }

        NameManager::~NameManager(void) { }

        void NameManager::Add(const std::string &group, Entity *entity)
        {
            auto &groups = m_idMap[ entity->m_uniqueID ];

            // already have this group
            if (find( groups.begin( ), groups.end( ), group ) != groups.end( ))
                return;

            groups.push_back( group );

            m_names[ group ].push_back( entity );
        }

        void NameManager::Remove(const std::string &group, Entity *entity)
        {
            auto &entities = m_names[ group ];

            auto search = find( entities.begin( ), entities.end( ), entity );

            // doesn't have this group
            if (search == entities.end( ))
                return;

            entities.erase( search );

            auto &groups = m_idMap[ entity->m_uniqueID ];

            groups.erase( find( groups.begin( ), groups.end( ), group ) );
        }

        void NameManager::Clear(Entity *entity)
        {
            auto &groups = m_idMap[ entity->m_uniqueID ];

            while (!groups.empty( ))
                Remove( groups.front( ), entity );
        }

        Entity *NameManager::GetEntity(const std::string &name)
        {
            auto &entities = m_names[ name ];

            if (entities.empty( ))
                return nullptr;

            return entities.front( );
        }

        const EntityVector &NameManager::GetEntities(const std::string &group)
        {
            return m_names[ group ];
        }

        const std::string &NameManager::GetName(const Entity *entity) const
        {
            URSINE_TODO( "..." );
            return m_names.begin( )->first;
        }
    }
}
