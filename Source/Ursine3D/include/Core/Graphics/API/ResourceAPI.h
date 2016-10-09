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

#include "GFXDefines.h"
#include "ModelResource.h"
#include "UModelData.h"
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
            ResourceAPI(void);

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
            
            void LoadFontTextures(GfxHND handle);
            void UnloadFontTextures(GfxHND handle);

            void GetBinaryInformation(GfxHND handle, uint8_t **dataPtr, size_t &binarySize);

            /////////////////////////////////////////////////////////
            // MODEL
            /////////////////////////////////////////////////////////

            // create a model
            GfxHND CreateModel(const resources::UModelData::Handle &model);
            void DestroyModel(GfxHND &handle);

            // load/unload from GPU
            void LoadModel(GfxHND handle);
            void UnloadModel(GfxHND handle);

            // get model data
            ModelResource *GetModelResource(GfxHND handle);

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
            void PrepForFont(void);

            GfxHND CreateBitmapFont(const uint8_t *binaryData, size_t binarySize);
            void DestroyBitmapFont(GfxHND &fontHandle);
            void RegisterTexture(GfxHND fontHandle, const std::string &fontName, GfxHND textureHandle);

            void FinishFont(void);

        private:
            privData *m_privates;

        private:
            void SetPrivates(void *priv, void *priv2, void *priv3);
            void Initialize();
            void Uninitialize();
        };
    }
}