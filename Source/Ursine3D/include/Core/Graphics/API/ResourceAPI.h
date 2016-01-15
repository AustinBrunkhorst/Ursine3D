/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ResourceAPI.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "GFXAPIDefines.h"
#include "Model.h"

namespace ursine
{
    namespace graphics
    {
        class ResourceAPI
        {
            friend class GfxAPI;
        private:
            struct privData;

        public:
            // get model handle
            GfxHND GetModelHandle(const char *name);

            // get texture handle
            GfxHND GetTexHandle(const char *name);

            // create a texture of a given width/height, returns handle
            GfxHND CreateTexture(const unsigned width, const unsigned height);

            // resize a texture
            void ResizeTexture(GfxHND &handle, const unsigned width, const unsigned height);

            // destroy a texture resource
            void DestroyTexture(GfxHND &handle);

            //validate that a model/texture exists

            //get a list of available models

            //get a list of available textures

            // get a model
            ModelResource *GetModel(const GfxHND &handle);
            ModelResource *GetModel(const std::string &modelName);


        private:
            privData *m_privates;

        private:
            void SetPrivates(void *priv, void *priv2);
            void Initialize();
            void Uninitialize();
        };
    }
}