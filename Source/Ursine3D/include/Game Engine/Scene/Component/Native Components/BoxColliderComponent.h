/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** BoxColliderComponent.h
**
** Author:
** - Jordan Ellis - contact@jordanellis.me
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "Component.h"
#include "BoxCollider.h"

namespace ursine
{
    namespace ecs
    {
        class BoxCollider : public Component
        {
            NATIVE_COMPONENT;

            friend class PhysicsSystem;

        public:
            EditorField(
                SVec3 dimensions,
                GetDimensions,
                SetDimensions
            );

            BoxCollider(void);

            SVec3 GetDimensions(void) const;
            void SetDimensions(const SVec3 &dimensions);

        private:
            physics::BoxCollider m_boxCollider;

        } Meta( Enable, WhiteListMethods, DisplayName( "BoxCollider" ) );
    }
}
