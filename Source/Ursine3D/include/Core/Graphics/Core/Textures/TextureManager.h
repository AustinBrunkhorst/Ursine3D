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
            void Initialize(ID3D11Device *device, ID3D11DeviceContext *context, std::string filePath);
            void Uninitialize(void);

            void MapTextureByName(const std::string name, const unsigned int bufferIndex = 0);
            void MapTextureByID(const unsigned ID, const unsigned int bufferIndex = 0);

            void MapSamplerState(const Sampler type, const unsigned bufferIndex = 0);

            unsigned GetTextureIDByName(const std::string name);

            GfxHND CreateDynamicTexture(const unsigned width, const unsigned height);
            Texture *GetDynamicTexture(GfxHND &handle);
            void ResizeDynamicTexture(GfxHND &handle, const unsigned width, const unsigned height);
            void DestroyDynamicTexture(GfxHND &handle);

            // new methods
            GfxHND CreateTexture(uint8_t *binaryData, size_t binarySize, unsigned width, unsigned height);
            void DestroyTexture(GfxHND &handle);

            void LoadTexture(GfxHND handle);
            void UnloadTexture(GfxHND handle);

            void GetBinaryInformation(GfxHND handle, uint8_t **dataPtr, size_t &binarySize);

            void MapResourceTextureByID(const unsigned ID, const unsigned int bufferIndex = 0);

        private:
            void TextureLoadBackend(const std::string name, const std::string path, const unsigned width, const unsigned height);

            void LoadTextureToGPU(Texture &texture) const;

            void InitalizeTexture(uint8_t *binaryData, size_t binarySize, unsigned width, unsigned height, Texture &texture) const;

        private:
            ID3D11Device *m_device;
            ID3D11DeviceContext *m_deviceContext;

            unsigned m_textureCount;

            std::map<std::string, Texture*> m_textureList;
            std::map<unsigned int, Texture*> m_hashTextureList;
            std::map<std::string, unsigned int> m_lookupTextureList;

            std::vector<ID3D11SamplerState*> m_samplerStateList_;

            // NEW STUFF
            std::vector<Texture> m_textureCache;
        };
    }
}
