/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** BoxCollider.h
**
** Author:
** - Jordan Ellis - contact@jordanellis.me
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "PhysicsInteropConfig.h"
#include "SVec3.h"

namespace ursine
{
    namespace physics
    {
        class BoxCollider : public BoxColliderBase
        {
        public:
            BoxCollider(void);

            SVec3 GetDimensions(void) const;
            void SetDimensions(const SVec3 &dimensions);
        };
    }
}
