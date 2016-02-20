/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** LineRendererPass.h
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
        class LineRendererPass : public RenderPass
        {
        public:
            LineRendererPass(bool debugRender = false, std::string passName = "LineRendererPass");
            virtual ~LineRendererPass(void);

        protected:
            void executePass(Camera &currentCamera) override;

            bool m_debugRender;
        };
    }
}