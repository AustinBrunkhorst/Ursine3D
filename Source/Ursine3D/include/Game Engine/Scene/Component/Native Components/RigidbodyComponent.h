#pragma once

#include "Component.h"
#include "Rigidbody.h"

namespace ursine
{
    namespace ecs
    {
        enum class BodyType
        {
            Dynamic = physics::BODY_DYNAMIC,
            Static = physics::BODY_STATIC,
            kinematic = physics::BODY_KINEMATIC
        } Meta( Enable );

        class Rigidbody : public Component
        {
            NATIVE_COMPONENT;

            friend class PhysicsSystem;

        public:
            EditorField(
                BodyType bodyType,
                GetBodyType,
                SetBodyType
            );

            Rigidbody(void);
			~Rigidbody(void);

            Meta(Disable)
			void OnInitialize(void) override;

            BodyType GetBodyType(void) const;
            void SetBodyType(BodyType bodyType);

            void SetAwake(void);

        private:
            physics::Rigidbody m_rigidbody;

			void onTransformChange(EVENT_HANDLER(Entity));

        } Meta(Enable, DisplayName( "Rigidbody" ));
    }
}
