#pragma once

#include "Component.h"
#include "CylinderCollider.h"

namespace ursine
{
    namespace ecs
    {
        class CylinderCollider : public Component
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

            CylinderCollider(void);
            ~CylinderCollider(void);

            Meta(Disable)
            void OnInitialize(void) override;

            float GetRadius(void) const;
            void SetRadius(float radius);

            float GetHeight(void) const;
            void SetHeight(float height);

        private:
            physics::CylinderCollider m_cylinderCollider;

            float m_height;
            float m_radius;

            void onTransformChange(EVENT_HANDLER(Entity));

            void updateHeightAndRadius(void);

        } Meta(Enable, DisplayName( "CylinderCollider" ));
    }
}
