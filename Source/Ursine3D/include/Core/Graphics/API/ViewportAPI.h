#pragma once


#include "GFXAPIDefines.h"

#include "ViewportRenderModes.h"
#include "Viewport.h"

namespace ursine
{
    namespace graphics
    {
        class ViewportAPI
        {
            friend class GfxAPI;
        private:
            struct privData;

        public:
            //create a viewport
            GfxHND CreateViewport(unsigned width, unsigned height);

            //destroy a viewport
            void DestroyViewport(GfxHND &handle);

            Viewport &GetViewport(GfxHND &handle);

        private:
            privData *m_privates;

        private:
            void SetPrivates(void *priv);
            void Initialize();
            void Uninitialize();
        };
    }
}
