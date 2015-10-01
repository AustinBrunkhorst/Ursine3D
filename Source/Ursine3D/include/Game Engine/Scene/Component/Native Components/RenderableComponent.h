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

            GFXHND GetHandle(void) const;
            void SetHandle(GFXHND handle);

        private:
            friend class RenderSystem;

            GFXHND m_handle;
        } Meta(Enable, DisplayName( "Renderable" ));
    }
}