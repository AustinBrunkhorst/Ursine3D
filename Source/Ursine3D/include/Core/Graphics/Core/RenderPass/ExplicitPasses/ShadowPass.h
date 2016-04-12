/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ShadowPass.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "RenderPass.h"

namespace ursine
{
    namespace graphics
    {
        class Light;

        class ShadowPass : public RenderPass
        {
        public:
            ShadowPass(const std::string &passName = "ShadowPass");
            virtual ~ShadowPass(void);

        protected:
            void executePass(Camera &currentCamera, int index) override;

            void setDepthTarget(Light *light);
        };
    }
}