#pragma once

#include <Component.h>
#include "PhysicsSystem.h"

namespace ursine
{
    namespace ecs
    {
        class PhysicsSettings : public Component
        {
            NATIVE_COMPONENT;

        public:
            EditorField(
                SVec3 gravity,
                GetGravity,
                SetGravity
            );

            PhysicsSettings(void);
            ~PhysicsSettings(void);

            void OnInitialize(void) override;

            SVec3 GetGravity(void) const;
            void SetGravity(const SVec3 &gravity);

        private:
            PhysicsSystem *m_physicsSystem;

        } Meta(Enable, DisplayName("PhysicsSettings"));
    }
}