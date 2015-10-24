/* ---------------------------------------------------------------------------
** Team Bear King
** DigiPen Institute of Technology 2015
**
** RootHierarchyNode.h
**
** Author:
** - Jordan Ellis - contact@jordanellis.me
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include"HierarchyNode.h"

namespace ursine
{
    namespace ecs
    {
        /** @brief RootNodes contain only children.
        *   These children are Entities.  These entities
        *   are the top most entities in the scene.
        */
        class RootHierarchyNode : public HierarchyNode
        {

        };
    }
}
