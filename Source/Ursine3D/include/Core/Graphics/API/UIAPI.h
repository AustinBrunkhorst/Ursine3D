#pragma once


#include <cef_render_handler.h>

#include "GfxAPIDefines.h"
#include "UIInstance.h"

namespace ursine
{
    namespace graphics
    {
        class UIAPI
        {
            friend class GfxAPI;
        private:
            struct privData;

        public:
            //create a viewport
            GfxHND CreateUI();

            //get ui
            UIInstance &GetUI(GfxHND UI);

            //destroy a viewport
            void DestroyUI(GfxHND UI);

        private:
            privData *m_privates;

        private:
            void SetPrivates(void *priv);
            void Initialize();
            void Uninitialize();
        };
    }
}