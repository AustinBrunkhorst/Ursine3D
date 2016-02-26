/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** PointRendererPass.h
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
        class PointRendererPass : public RenderPass
        {
        public:
            PointRendererPass(bool debugRender = false, std::string passName = "PointRendererPass");
            virtual ~PointRendererPass(void);

        protected:
            void executePass(Camera &currentCamera) override;

            bool m_debugRender;
        };
    }
}