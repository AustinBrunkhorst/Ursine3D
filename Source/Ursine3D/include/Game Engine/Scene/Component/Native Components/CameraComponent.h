#pragma once

#include "Component.h"

#include "Camera.h"

#include "GFXAPIDefines.h"

namespace ursine
{
    namespace ecs
    {
        typedef ::Camera GraphicsCamera;

        class Camera : public Component
        {
            NATIVE_COMPONENT;

        public:
            Camera(void);
            ~Camera(void);

            GraphicsCamera &GetCamera(void) const;

        private:
            friend class RenderSystem;

            GFXHND m_handle;
        } Meta(Enable, DisplayName( "Camera" ));
    }
}