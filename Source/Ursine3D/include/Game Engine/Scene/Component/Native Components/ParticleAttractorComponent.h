/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ParticleAttractorComponent.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "Component.h"
#include "Renderable.h"
#include "GfxAPI.h"
#include "RenderableComponentBase.h"
#include "Color.h"

namespace ursine
{
    namespace ecs
    {
        class ParticleAttractor
            : public Component
        {
            NATIVE_COMPONENT;

        public:
            ParticleAttractor(void);
            ~ParticleAttractor(void);

            Meta(Disable)
            void OnInitialize(void) override;

        private:
            void onParticleUpdate(EVENT_HANDLER(Entity));

        } Meta(
            Enable,
            DisplayName("ParticleAttractor"),
            RequiresComponents(typeof(ursine::ecs::ParticleSystem))
        );
    }
}