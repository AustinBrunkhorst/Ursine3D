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

            float GetRadius(void) const;
            void SetRadius(float radius);

        private:
            URSINE_TODO("Have this be a static object so we can share data across instances");
            physics::SphereCollider m_sphereCollider;

        } Meta( Enable, WhiteListMethods, DisplayName( "SphereCollider" ) );
    }
}