/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** PhysicsSettingsComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

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

			EditorField(
				bool playmodeDebugDraw,
				GetPlaymodeDebugDraw,
				SetPlaymodeDebugDraw
			);

            PhysicsSettings(void);
            ~PhysicsSettings(void);

            void OnInitialize(void) override;

            SVec3 GetGravity(void) const;
            void SetGravity(const SVec3 &gravity);

            bool GetEnableDebugDraw(void) const;
            void SetEnableDebugDraw(bool enable);

			bool GetPlaymodeDebugDraw(void) const;
			void SetPlaymodeDebugDraw(bool enable);

        private:
            PhysicsSystem *m_physicsSystem;

            SVec3 m_gravity;

        } Meta(Enable, DisplayName( "PhysicsSettings" ));
    }
}