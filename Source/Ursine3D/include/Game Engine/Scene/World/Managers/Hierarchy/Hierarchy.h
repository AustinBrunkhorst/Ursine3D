/* ---------------------------------------------------------------------------
** Team Bear King
** DigiPen Institute of Technology 2015
**
** Hierarchy.h
**
** Author:
** - Jordan Ellis - contact@jordanellis.me
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "RootHierarchyNode.h"
#include "EntityHierarchyNode.h"
#include "HierarchyEvents.h"

namespace ursine
{
    namespace ecs
    {
        class Hierarchy
        {
        public:
            void AddEntity(Entity *entity);
            void RemoveEntity(Entity *entity);

            const std::vector<EntityID> *GetChildren(EntityID entity) const;
      
            EntityID GetParent(EntityID entity) const;
      
            EntityID GetRoot(EntityID entity) const;

            const RootHierarchyNode &GetRootNode(void) const;

            uint GetSiblingIndex(EntityID entity) const;

            void SetAsFirstSibling(EntityID entity);

            void SetSiblingIndex(EntityID entity, uint index);
    
        private:
            RootHierarchyNode m_root;
      
            // Array of EntityNodes.  Each index is associated to the entity's id.
            std::vector<EntityHierarchyNode> m_nodes;

            void parentChanged(EVENT_HANDLER(ParentChangedArgs));

            const std::vector<EntityID> *getSiblingArray(EntityID id) const;
            std::vector<EntityID> *getSiblingArray(EntityID id);
        };
    }
}
