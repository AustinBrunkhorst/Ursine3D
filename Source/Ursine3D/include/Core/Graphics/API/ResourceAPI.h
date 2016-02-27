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
#include "ModelResource.h"
#include "ModelInfo.h"
#include "AnimationInfo.h"
#include "GUID.h"

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
            /////////////////////////////////////////////////////////
            // TEXTURE
            /////////////////////////////////////////////////////////
            // get texture handle
            GfxHND GetTexHandle(const char *name);

            // create a texture of a given width/height, returns handle
            GfxHND CreateDynamicTexture(const unsigned width, const unsigned height);

            // resize a texture
            void ResizeDynamicTexture(GfxHND &handle, const unsigned width, const unsigned height);

            // destroy a texture resource
            void DestroyDynamicTexture(GfxHND &handle);
            
            /////////////////////////////////////////////////////////
            // NEW LOADING METHODS
            GfxHND CreateTexture(uint8_t *binaryData, size_t binarySize, unsigned width, unsigned height);
            void DestroyTexture(GfxHND &handle);

            void LoadTexture(GfxHND handle);
            void UnloadTexture(GfxHND handle);

            void GetBinaryInformation(GfxHND handle, uint8_t **dataPtr, size_t &binarySize);

            /////////////////////////////////////////////////////////
            // MODEL
            /////////////////////////////////////////////////////////
            // get model handle
            GfxHND GetModelHandle(const char *name);

            // get a model
            ModelResource *GetModelResource(const GfxHND &handle);
            ModelResource *GetModelResource(const std::string &modelName);

            GfxHND CreateModel(ufmt_loader::ModelInfo *modelInfo);
            void DestroyModel(GfxHND &handle);

            void LoadModel(GfxHND handle);
            void UnloadModel(GfxHND handle);

            ufmt_loader::ModelInfo *GetModelInfo(const GfxHND &handle);

            /////////////////////////////////////////////////////////
            // Animation
            /////////////////////////////////////////////////////////
            // get model handle
            GfxHND GetAnimeHandle(const char *name);

            //// get a model
            //ModelResource *GetModelResource(const GfxHND &handle);
            //ModelResource *GetModelResource(const std::string &modelName);

            GfxHND CreateAnimation(ufmt_loader::AnimInfo *animeInfo);
            void DestroyAnimation(GfxHND &handle);

            //void LoadModel(GfxHND handle);
            //void UnloadModel(GfxHND handle);

            ufmt_loader::AnimInfo *GetAnimInfo(const GfxHND &handle);
            
        private:
            privData *m_privates;

        private:
            void SetPrivates(void *priv, void *priv2);
            void Initialize();
            void Uninitialize();
        };
    }
}