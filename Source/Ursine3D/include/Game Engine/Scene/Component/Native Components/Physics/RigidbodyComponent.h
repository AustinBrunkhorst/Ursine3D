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

            EditorField(
                bool freezeXRotation,
                GetRotationFreezeX,
                SetRotationFreezeX
            );

            EditorField(
                bool freezeYRotation,
                GetRotationFreezeY,
                SetRotationFreezeY
            );

            EditorField(
                bool freezeZRotation,
                GetRotationFreezeZ,
                SetRotationFreezeZ
            );

            EditorField(
                bool sleepToggle,
                GetSleepToggle,
                SetSleepToggle
            );

			EditorField(
				SVec3 gravity,
				GetGravity,
				SetGravity
			);

            Rigidbody(void);
			~Rigidbody(void);

            Meta(Disable)
			void OnInitialize(void) override;

            BodyType GetBodyType(void) const;
            void SetBodyType(BodyType bodyType);

            float GetMass(void) const;
            void SetMass(float mass);

            Meta(Disable)
            void SetAwake(void);

            void SetOffset(const SVec3 &offset);
            SVec3 GetOffset(void) const;

            void SetRotationFreezeX(bool flag);
            bool GetRotationFreezeX(void) const;

            void SetRotationFreezeY(bool flag);
            bool GetRotationFreezeY(void) const;

            void SetRotationFreezeZ(bool flag);
            bool GetRotationFreezeZ(void) const;

            void SetSleepToggle(bool flag);
            bool GetSleepToggle(void) const;

            Meta(Disable)
            void UpdateInertiaTensor(void);

            void SetVelocity(const SVec3 &velocity);
            SVec3 GetVelocity(void) const;

            void SetAngularVelocity(const SVec3 &angularVelocity);
            SVec3 GetAngularVelocity(void) const;

			void SetGravity(const SVec3 &gravity);
			SVec3 GetGravity(void) const;

            URSINE_TODO("Add ForceMode");
            void AddForce(const SVec3 &force);

            // Relative to the transforms coordinate system
            void AddForceRelative(const SVec3 &force);

            void AddForceAtPosition(const SVec3 &force, const SVec3 &worldPosition);

            void AddTorque(const SVec3 &torque);

            // Relative to the transforms coordinate system
            void AddTorqueRelative(const SVec3 &torque);

        private:
            physics::Rigidbody m_rigidbody;

			void onTransformChange(EVENT_HANDLER(Entity));

        } Meta(Enable, DisplayName( "Rigidbody" ));
    }
}
