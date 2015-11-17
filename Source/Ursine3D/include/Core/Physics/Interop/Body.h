/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Body.h
**
** Author:
** - Jordan Ellis - contact@jordanellis.me
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "PhysicsInteropConfig.h"

namespace ursine
{
    namespace ecs
    {
        class Transform;
    }

    namespace physics
    {
        class Body : public virtual BodyBase
        {
        public:
            Body(void);

            void SetTransform(ecs::Transform *transform);

            void GetTransform(ecs::Transform *transform);

            void SetCollider(ColliderBase *collider);

            void RemoveCollider(void);
             
            void SetOffset(const SVec3 &offset);

            SVec3 GetOffset(void) const;

        private:
            SVec3 m_offset;

        };
    }
}
