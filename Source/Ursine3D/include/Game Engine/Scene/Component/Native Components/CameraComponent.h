#pragma once

#include "Component.h"

#include "Camera.h"

#include "GFXAPIDefines.h"

namespace ursine
{
    namespace ecs
    {
        class Camera : public Component
        {
            NATIVE_COMPONENT;

        public:
            Camera(void);
            ~Camera(void);

            GFXHND GetHandle(void) const;
            GFXCamera &GetCamera(void) const;

        private:
            friend class RenderSystem;

            GFXHND m_handle;
        } Meta(Enable, DisplayName( "Camera" ));
    }
}