/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ViewportAPI.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

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
