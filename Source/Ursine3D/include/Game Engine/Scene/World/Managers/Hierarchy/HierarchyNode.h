/* ---------------------------------------------------------------------------
** Team Bear King
** DigiPen Institute of Technology 2015
**
** HierarchyNode.h
**
** Author:
** - Jordan Ellis - contact@jordanellis.me
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "EntityConfig.h"

namespace ursine
{
    namespace ecs
    {
        class HierarchyNode
        {
            friend class Hierarchy;

        public:
            virtual const std::vector<EntityID> *Children(void) const;
            virtual void AddChild(EntityID child);
            virtual void RemoveChild(EntityID child);

        protected:
            std::vector<EntityID> m_children;
        };
    }
}
