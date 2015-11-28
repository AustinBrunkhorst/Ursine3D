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
            , public RenderableComponentBase
        {
            NATIVE_COMPONENT;

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

            void updateRenderer(void) override;

        } Meta(Enable, DisplayName("Billboard2D")); 
    }
}