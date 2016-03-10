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
#include "FontData.h"

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

            // create a texture of a given width/height, returns handle
            GfxHND CreateDynamicTexture(const unsigned width, const unsigned height);

            // resize a texture
            void ResizeDynamicTexture(GfxHND &handle, const unsigned width, const unsigned height);

            // destroy a texture resource
            void DestroyDynamicTexture(GfxHND &handle);
            
            /////////////////////////////////////////////////////////
            // NEW LOADING METHODS
            GfxHND CreateTexture(const uint8_t *binaryData, size_t binarySize, unsigned width, unsigned height);
            void DestroyTexture(GfxHND &handle);

            void LoadTexture(GfxHND handle);
            void UnloadTexture(GfxHND handle);

            void GetBinaryInformation(GfxHND handle, uint8_t **dataPtr, size_t &binarySize);

            /////////////////////////////////////////////////////////
            // MODEL
            /////////////////////////////////////////////////////////

            // create a model
            bool CheckModelExistence(const std::string &modelName);
            GfxHND CreateModel(const ufmt_loader::ModelInfo &modelInfo);
            void DestroyModel(GfxHND &handle);

            // load/unload from GPU
            void LoadModel(GfxHND handle);
            void UnloadModel(GfxHND handle);

            // get model data
            ModelResource *GetModelResource(GfxHND handle);

            // get a model info
            ufmt_loader::ModelInfo *GetModelInfo(const GfxHND &handle);

            /////////////////////////////////////////////////////////
            // Animation
            /////////////////////////////////////////////////////////
            bool CheckAnimExistence(const std::string &animeName);
            GfxHND CreateAnimation(const ufmt_loader::AnimInfo &animeInfo);
            void DestroyAnimation(GfxHND &handle);

            ufmt_loader::AnimInfo *GetAnimInfo(const GfxHND &handle);

            /////////////////////////////////////////////////////////
            // FONT
            /////////////////////////////////////////////////////////
            GfxHND CreateBitmapFont(uint8_t *binaryData, size_t binarySize);
            void DestroyBitmapFont(GfxHND &fontHandle);
            void RegisterTexture(GfxHND fontHandle, const std::string &fontName, GfxHND textureHandle);

        private:
            privData *m_privates;

        private:
            void SetPrivates(void *priv, void *priv2, void *priv3);
            void Initialize();
            void Uninitialize();
        };
    }
}