/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Hierarchy.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "Hierarchy.h"
#include "EntityEvent.h"
#include "Entity.h"
#include "HierarchyEvents.h"

namespace ursine
{
    namespace ecs
    {
        void Hierarchy::AddEntity(Entity *entity)
        {
            auto id = entity->m_id;

            // grow the array's size if needed
            if (id >= m_nodes.size( ))
                m_nodes.emplace_back( id );

            // Add it to the root
            m_root.AddChild( id );

            // subscribe to this entities events
            entity->Listener( this )
                .On( ENTITY_PARENT_CHANGED, &Hierarchy::parentChanged );
        }

        void Hierarchy::RemoveEntity(Entity *entity)
        {
            auto id = entity->m_id;
            auto &entityNode = m_nodes[ id ];

            // Remove this entity from the parent
            auto parentID = entityNode.Parent( );

            if (parentID == -1)
                m_root.RemoveChild( id );
            else
                m_nodes[ parentID ].RemoveChild( id );

            // Deactivate this entity
            entityNode.Deactivate( );

            // unsubscribe to this entities events
            entity->Listener( this )
                .Off( ENTITY_PARENT_CHANGED, &Hierarchy::parentChanged );
        }

        const std::vector<EntityID> *Hierarchy::GetChildren(EntityID entity) const
        {
            return m_nodes[ entity ].Children( );
        }

        EntityID Hierarchy::GetParent(EntityID entity) const
        {
            return m_nodes[ entity ].Parent( );
        }

        EntityID Hierarchy::GetRoot(EntityID entity) const
        {
            return m_nodes[ entity ].Root( );
        }

        const RootHierarchyNode &Hierarchy::GetRootNode(void) const
        {
            return m_root;
        }

        uint Hierarchy::GetSiblingIndex(EntityID entity) const
        {
            auto &children = *getSiblingArray( entity );

            int i = 0;

            for (auto &child : children)
            {
                if (child == entity)
                    return i;
                else
                    ++i;
            }

            UAssert( i == children.size( ), 
                "This shouldn't happen. Something is wrong with the scene" 
            );

            return 0;
        }

        void Hierarchy::SetAsFirstSibling(EntityID entity)
        {
            SetSiblingIndex( entity, 0 );
        }

        void Hierarchy::SetSiblingIndex(EntityID entity, uint index)
        {
            auto &children = *getSiblingArray( entity );

            UAssert( index < children.size( ), 
                "This is an invalid index." 
            );

            auto i = 0;

            for (auto &child : children)
            {
                if (child == entity)
                    break;
                else
                    ++i;
            }

            UAssert( i != children.size( ), 
                "This shouldn't happen. Something is wrong with the scene" 
            );

            // walk from the old place to the new place, making sure all things are moved
            auto dir = static_cast<int>( index ) > i ? 1 : -1;

            for (auto j = i; j != index; j += dir)
            {
                children[ j ] = children[ j + dir ];
                children[ j + dir ] = entity;
            }
        }

        void Hierarchy::parentChanged(EVENT_HANDLER(Entity))
        {
            EVENT_ATTRS(Entity, ParentChangedArgs);

            auto entityID = sender->m_id;
            auto &entityNode = m_nodes[ entityID ];

            auto oldID = args->oldParent ? args->oldParent->m_id : -1;
            auto newID = args->newParent ? args->newParent->m_id : -1;

            if (oldID == newID)
                return;
            
            // assign the new parent
            entityNode.SetParent( newID );

            // Add to the root node if it has no parent
            if (newID == -1)
            {
                m_root.AddChild( entityID );
                entityNode.SetRoot( entityID );
            }
            // Add to the new parent node
            else
            {
                auto &parentNode = m_nodes[ newID ];
                parentNode.AddChild( entityID );
                entityNode.SetRoot( parentNode.Root( ) );
            }

            // Remove it from the old parent (root or other entity)
            if (oldID == -1)
                m_root.RemoveChild( entityID );
            else
                m_nodes[ oldID ].RemoveChild( entityID );
        }

        const std::vector<EntityID> *Hierarchy::getSiblingArray(EntityID id) const
        {
            auto parentID = m_nodes[ id ].Parent( );
            const HierarchyNode *parentNode;

            if (parentID == -1)
                parentNode = &m_root;
            else
                parentNode = &m_nodes[ parentID ];

            return parentNode->Children( );
        }

        std::vector<EntityID> *Hierarchy::getSiblingArray(EntityID id)
        {
            auto parentID = m_nodes[ id ].Parent( );
            HierarchyNode *parentNode;

            if (parentID == -1)
                parentNode = &m_root;
            else
                parentNode = &m_nodes[ parentID ];

            return &(parentNode->m_children);
        }
    }
}
