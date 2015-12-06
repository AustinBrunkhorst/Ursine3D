/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** BodyComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "Component.h"
#include "Body.h"

namespace ursine
{
    namespace ecs
    {
        class Body : public Component
        {
            NATIVE_COMPONENT;

            friend class PhysicsSystem;

        public:
            Body(void);
			~Body(void);

            Meta(Disable)
			void OnInitialize(void) override;

            void SetOffset(const SVec3 &offset);
            SVec3 GetOffset(void) const;

        private:
            physics::Body m_body;

			void onTransformChange(EVENT_HANDLER(Entity));

        } Meta(Enable, HiddenInInspector, DisableSerialization);
    }
}
