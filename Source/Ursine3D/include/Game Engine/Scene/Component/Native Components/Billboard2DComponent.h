/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Billboard2DComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "Component.h"
#include "Renderable.h"
#include "GfxAPI.h"
#include "RenderableComponentBase.h"

namespace ursine
{
    namespace ecs
    {
        class Billboard2D 
            : public Component
        {
            NATIVE_COMPONENT;

			friend class RenderSystem;

        public:
            Billboard2D(void);
            ~Billboard2D(void);

            Meta(Disable)
            void OnInitialize(void) override;

            //get/set model
            ursine::graphics::Billboard2D *GetBillboard(void);

        private:

            // This model component's model in the renderer
            graphics::Billboard2D *m_billboard;

            // The graphics core API
            graphics::GfxAPI *m_graphics;

            void updateRenderer(void);

			RenderableComponentBase *m_base;

        } Meta(Enable, DisplayName("Billboard2D")); 
    }
}