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

            EditorField(
                bool enableDebugDraw,
                GetEnableDebugDraw,
                SetEnableDebugDraw
            );

            PhysicsSettings(void);
            ~PhysicsSettings(void);

            void OnInitialize(void) override;

            SVec3 GetGravity(void) const;
            void SetGravity(const SVec3 &gravity);

            bool GetEnableDebugDraw(void) const;
            void SetEnableDebugDraw(bool enable);

        private:
            PhysicsSystem *m_physicsSystem;

            SVec3 m_gravity;

        } Meta(Enable, DisplayName("PhysicsSettings"));
    }
}