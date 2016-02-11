/* ----------------------------------------------------------------------------
** Team Bear King
** � 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** RigComponent.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "Component.h"

namespace ursine
{
    namespace ecs
    {
        class Rig
            : public Component
        {
            NATIVE_COMPONENT;

        public:
            Rig(void);
            ~Rig(void);

            Meta(Disable)
            void OnInitialize(void) override;

        private:
        } Meta(Enable, DisplayName("Rig"));
    }
}