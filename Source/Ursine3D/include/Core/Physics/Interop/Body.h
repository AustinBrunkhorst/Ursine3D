/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
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
        class Body : public BodyBase
        {
        public:
            Body(void);

            void SetUserID(int id);
            int GetUserID(void);

            void SetUserPointer(void *ptr);
            void *GetUserPointer(void);

            static Body *DownCast(BodyBase *body);
            static const Body *DownCast(const BodyBase *body);

            void SetTransform(ecs::Transform *transform);
            void GetTransform(ecs::Transform *transform);

            void SetCollider(ColliderBase *collider);
            ColliderBase *GetCollider(void);
             
            void SetOffset(const SVec3 &offset);
            SVec3 GetOffset(void) const;

            void SetDisableContactResponse(bool disable);
            bool GetDisableContactResponse(void) const;

            void SetAwake(void);

        private:
            bool m_disableContactResponse;
            
            SVec3 m_offset;
        };
    }
}
