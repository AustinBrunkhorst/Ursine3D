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
            ~BoxCollider(void);

            Meta(Disable);
            void OnInitialize(void) override;

            SVec3 GetDimensions(void) const;
            void SetDimensions(const SVec3 &dimensions);

        private:
            physics::BoxCollider m_boxCollider;

            SVec3 m_dimensions;

            void onTransformChange(EVENT_HANDLER(Entity));

            void updateDimensions(void);

        } Meta(Enable, DisplayName( "BoxCollider" ));
    }
}
