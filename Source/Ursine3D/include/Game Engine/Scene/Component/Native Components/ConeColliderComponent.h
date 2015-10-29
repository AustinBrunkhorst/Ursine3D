#pragma once

#include "Component.h"
#include "ConeCollider.h"

namespace ursine
{
    namespace ecs
    {
        class ConeCollider : public Component
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

            ConeCollider(void);
            ~ConeCollider(void);

            void OnInitialize(void) override;

            float GetRadius(void) const;
            void SetRadius(float radius);

            float GetHeight(void) const;
            void SetHeight(float height);

        private:
            physics::ConeCollider m_coneCollider;

            float m_height;
            float m_radius;

            void onTransformChange(EVENT_HANDLER(Entity));

            void updateHeightAndRadius(void);

        } Meta( Enable, WhiteListMethods, DisplayName( "ConeCollider" ) );
    }
}