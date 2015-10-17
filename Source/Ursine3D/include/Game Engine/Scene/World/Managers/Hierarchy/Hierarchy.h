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
            const std::vector<EntityID> *GetChildren(const Entity *entity) const;
      
            EntityID GetParent(const Entity *entity) const;
      
            EntityID GetRoot(const Entity *entity) const;
      
            void AddEntity(Entity *entity);
      
            void RemoveEntity(Entity *entity);
    
        private:
            void ParentChanged(EVENT_HANDLER( ParentChangedArgs ));
      
            RootHierarchyNode m_root;
      
            // Array of EntityNodes.  Each index is associated to the entity's id.
            std::vector<EntityHierarchyNode> m_nodes;
      
            // TODO: Maybe contains the BVH for all children
        };
    }
}
