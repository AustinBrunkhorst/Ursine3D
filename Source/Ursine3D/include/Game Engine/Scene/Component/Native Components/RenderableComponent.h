#pragma once

#include "Component.h"

#include "GFXAPIDefines.h"

namespace ursine
{
    namespace ecs
    {
        class Renderable : public Component
        {
            NATIVE_COMPONENT;

        public:
            Renderable(void);
            ~Renderable(void);

            Meta(Disable)
            graphics::GfxHND GetHandle(void) const;

            Meta(Disable)
            void SetHandle(graphics::GfxHND handle);

            void OnInitialize(void) override;

        private:
            friend class RenderSystem;

            graphics::GfxHND m_handle;

            // lets us know if we need to update the matrix in the renderer
            bool m_dirty;

            void onTransformChange(EVENT_HANDLER(Entity));

            // notify the components listening to the 
            // renderable component to update the renderer
            // Ex: Model updates the renderers matrix
            void updateRenderer(void);

        } Meta(Enable, HiddenInInspector);
    }
}