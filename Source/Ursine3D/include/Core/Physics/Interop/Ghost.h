/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Ghost.h
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
        class Rigidbody;
        class Ghost;
        class Body;
    }

    namespace physics
    {
        struct GhostOverlappingItem
        {
            GhostOverlappingItem(BodyType type, void *ptr)
                : type( type )
                , body( reinterpret_cast<ecs::Body*>( ptr ) ) { }

            // Type of they overlapping object
            BodyType type;

            // The pointer to the component
            union
            {
                ecs::Component *component;
                ecs::Rigidbody *rigidbody;
                ecs::Body *body;
                ecs::Ghost *ghost;
            };
        };

        class Ghost : public GhostBase
        {
        public:
            Ghost(void);

            void SetUserID(int id);
            int GetUserID(void);

            void SetUserPointer(void *ptr);
            void *GetUserPointer(void);

            static Ghost *DownCast(BodyBase *body);
            static const Ghost *DownCast(const BodyBase *body);

            void SetTransform(ecs::Transform *transform);
            void GetTransform(ecs::Transform *transform);

            void SetCollider(ColliderBase *collider);
            ColliderBase *GetCollider(void);

            void SetOffset(const SVec3 &offset);
            SVec3 GetOffset(void) const;

            void SetDisableContactResponse(bool disable);
            bool GetDisableContactResponse(void) const;

            void SetAwake(void);

            void GetOverlappingPairs(std::vector<GhostOverlappingItem> &pairs);

        private:
            bool m_disableContactResponse;

            SVec3 m_offset;
        };
    }
}
