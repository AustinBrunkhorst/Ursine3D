/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** GhostComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "Component.h"
#include "Ghost.h"

namespace ursine
{
    namespace ecs
    {
        class Ghost : public Component
        {
            NATIVE_COMPONENT;

            friend class PhysicsSystem;

        public:
            EditorField(
                SVec3 offset,
                GetOffset,
                SetOffset
            );

            EditorField(
                bool disableContactResponse,
                GetDisableContactResponse,
                SetDisableContactResponse
            );

            EditorField(
                bool enableContactCallback,
                GetEnableContactCallback,
                SetEnableContactCallback
            );

            EditorField(
                bool enableRaycast,
                GetEnableRaycast,
                SetEnableRaycast
                );

            Ghost(void);
            ~Ghost(void);

            Meta(Disable)
            void OnInitialize(void) override;

            void SetOffset(const SVec3 &offset);
            SVec3 GetOffset(void) const;

            void SetDisableContactResponse(bool disable);
            bool GetDisableContactResponse(void) const;

            void SetEnableContactCallback(bool enable);
            bool GetEnableContactCallback(void) const;

            void SetEnableRaycast(bool enable);
            bool GetEnableRaycast(void) const;

            void SetAwake(void);

            Meta(Disable)
            void GetOverlappingPairs(std::vector<physics::GhostOverlappingItem> &pairs);

        private:
            physics::Ghost m_ghost;

            bool m_enableContactCallback;

            bool m_enableRaycast;

            void onTransformChange(EVENT_HANDLER(Entity));

            void onCollision(EVENT_HANDLER(Entity));

        } Meta( Enable, DisplayName( "Ghost" ) );
    }
}
