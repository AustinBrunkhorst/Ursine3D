/* ---------------------------------------------------------------------------
** Team Bear King
** DigiPen Institute of Technology 2015
**
** EntityHierarchyNode.h
**
** Author:
** - Jordan Ellis - contact@jordanellis.me
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "HierarchyNode.h"

namespace ursine
{
    namespace ecs
    {
        // Forward Declaration
        class Hierarchy;

        /** @brief EntityNodes contain all children in
        *   their hierarchy. An EntityNode also has a
        *   parent entity.  If this Parent entity is == -1,
        *   this EntityNode is at the top of the scene.
        */
        class EntityHierarchyNode : public HierarchyNode
        {
        public:
            EntityHierarchyNode(EntityID id);

            void Deactivate(void);

            EntityID ID(void) const;

            EntityID Parent(void) const;
            void SetParent(EntityID parent);

            EntityID Root(void) const;
            void SetRoot(EntityID root);

        private:
            EntityID m_id;
            EntityID m_parent;
            EntityID m_root;
        };
    }
}
