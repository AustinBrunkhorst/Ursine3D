/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** TextureManager.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <vector>
#include <map>
#include <string>
#include "D3D11Forward.h"
#include "Texture.h"
#include "SamplerList.h"
#include "GfxDefines.h"

namespace ursine
{
    namespace graphics
    {
        class TextureManager
        {
        public:
            TextureManager(void);

            void Initialize(ID3D11Device *device, ID3D11DeviceContext *context, std::string filePath);
            void Uninitialize(void);

            // create/destroy
            GfxHND CreateTexture(uint8_t *binaryData, size_t binarySize, unsigned width, unsigned height);
            void DestroyTexture(GfxHND &handle);

            // load/unload
            void LoadTexture(GfxHND handle);
            void UnloadTexture(GfxHND handle);

            // mapping stuff
            void MapTextureByID(const unsigned ID, const unsigned int bufferIndex = 0);
            void MapSamplerState(const SAMPLER_STATES type, const unsigned bufferIndex = 0);

            // dynamic textures for UI and stuff
            GfxHND CreateDynamicTexture(const unsigned width, const unsigned height);
            Texture *GetDynamicTexture(GfxHND &handle);
            void ResizeDynamicTexture(GfxHND &handle, const unsigned width, const unsigned height);
            void DestroyDynamicTexture(GfxHND &handle);

            void GetBinaryInformation(GfxHND handle, uint8_t **dataPtr, size_t &binarySize);

        private:
            void LoadTextureToGPU(Texture &texture) const;

            void InitalizeTexture(uint8_t *binaryData, size_t binarySize, unsigned width, unsigned height, Texture &texture) const;

            ID3D11Device *m_device;
            ID3D11DeviceContext *m_deviceContext;

            unsigned m_textureCount;

            std::vector<ID3D11SamplerState*> m_samplerStateList_;

            // NEW STUFF
            std::vector<Texture> m_textureCache;
        };
    }
}
