#pragma once


#include "GFXAPIDefines.h"

#include "ViewportRenderModes.h"
#include "Viewport.h"

namespace ursine
{
    class ViewportAPI
    {
        friend class GfxAPI;
    private:
        struct privData;

    public:
        //create a viewport
        GFXHND CreateViewport(unsigned width, unsigned height);

        //destroy a viewport
        void DestroyViewport(GFXHND &handle);

        Viewport &GetViewport(GFXHND &handle);

    private:
        privData *m_privates;

    private:
        void SetPrivates(void *priv);
        void Initialize();
        void Uninitialize();
    };
}
