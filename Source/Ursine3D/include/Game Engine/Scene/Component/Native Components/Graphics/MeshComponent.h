/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** MeshComponent.h
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
        class Mesh
            : public Component
        {
            NATIVE_COMPONENT;

        public:
            Mesh(void);
            ~Mesh(void);

            Meta(Disable)
            void OnInitialize(void) override;

        private:
        } Meta(Enable, DisplayName("Mesh"));
    }
}