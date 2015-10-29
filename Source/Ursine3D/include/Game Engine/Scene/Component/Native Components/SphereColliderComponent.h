/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SphereColliderComponent.h
**
** Author:
** - Jordan Ellis - contact@jordanellis.me
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "Component.h"
#include "SphereCollider.h"

namespace ursine
{
    namespace ecs
    {
        class SphereCollider : public Component
        {
            NATIVE_COMPONENT;

            friend class PhysicsSystem;

        public:
            EditorField(
                float radius,
                GetRadius,
                SetRadius
            );

            SphereCollider(void);
            ~SphereCollider(void);

            void OnInitialize(void) override;

            float GetRadius(void) const;
            void SetRadius(float radius);

        private:
            physics::SphereCollider m_sphereCollider;

            float m_radius;

            void onTransformChange(EVENT_HANDLER(Entity));

            void updateRadius(void);

        } Meta( Enable, WhiteListMethods, DisplayName( "SphereCollider" ) );
    }
}
