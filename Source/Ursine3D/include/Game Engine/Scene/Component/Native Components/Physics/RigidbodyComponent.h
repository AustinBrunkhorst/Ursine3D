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
            Kinematic = physics::BODY_KINEMATIC
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

            EditorField(
                float mass,
                GetMass,
                SetMass
            );

            Rigidbody(void);
			~Rigidbody(void);

            Meta(Disable)
			void OnInitialize(void) override;

            BodyType GetBodyType(void) const;
            void SetBodyType(BodyType bodyType);

            float GetMass(void) const;
            void SetMass(float mass);

            void SetAwake(void);

            void SetOffset(const SVec3 &offset);
            SVec3 GetOffset(void) const;

            void LockXRotation(bool flag);
            void LockYRotation(bool flag);
            void LockZRotation(bool flag);

            void UpdateInertiaTensor(void);

        private:
            physics::Rigidbody m_rigidbody;

			void onTransformChange(EVENT_HANDLER(Entity));

        } Meta(Enable, DisplayName( "Rigidbody" ));
    }
}
