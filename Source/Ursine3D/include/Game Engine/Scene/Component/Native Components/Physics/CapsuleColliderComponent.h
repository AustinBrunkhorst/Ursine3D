/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** CapsuleColliderComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "Component.h"
#include "CapsuleCollider.h"

namespace ursine
{
    namespace ecs
    {
        class CapsuleCollider : public Component
        {
            NATIVE_COMPONENT;

            friend class PhysicsSystem;

        public:
            EditorField(
                float radius,
                GetRadius,
                SetRadius
            );

            EditorField(
                float height,
                GetHeight,
                SetHeight
            );

            EditorField(
                SVec3 offset,
                GetOffset,
                SetOffset
            );

            CapsuleCollider(void);
            ~CapsuleCollider(void);

            Meta(Disable)
            void OnInitialize(void) override;

            float GetRadius(void) const;
            void SetRadius(float radius);

            float GetHeight(void) const;
            void SetHeight(float height);

            SVec3 GetOffset(void) const;
            void SetOffset(const SVec3 &offset);

        private:
            physics::CapsuleCollider m_capsuleCollider;

            float m_height;
            float m_radius;

            void onTransformChange(EVENT_HANDLER(Entity));

            void updateHeightAndRadius(void);
            
        } Meta(Enable, DisplayName( "CapsuleCollider" ));
    }
}