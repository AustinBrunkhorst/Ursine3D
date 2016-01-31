/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** TextureManager.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"
#include "TextureManager.h"
#include "DDSTextureLoader.h"
#include <d3d11.h>
#include "DXErrorHandling.h"

namespace ursine
{
    namespace graphics
    {
        static const wchar_t *strToWchart(std::string sInput)
        {
            wchar_t *wCharOutput = new wchar_t[ 1023 ];
            size_t *sizeOut = new size_t;
            size_t sizeInWords = 256;

            const char *cStr;
            cStr = sInput.c_str();
            mbstowcs_s(sizeOut, wCharOutput, sizeInWords, cStr, strlen(cStr) + 1);

            return wCharOutput;
        }

        void TextureManager::Initialize(ID3D11Device *device, ID3D11DeviceContext *context, std::string filePath)
        {
            m_device = device;
            m_deviceContext = context;

            m_textureCount = 0;

            /////////////////////////////////////////////////////////////////
            // LOADING TEXTURES /////////////////////////////////////////////
            char buffer[ 512 ];
            std::ifstream input;
            std::string fileText = filePath;
            fileText.append("TEXTURELIST.8.0.gfx");
            input.open(fileText, std::ios_base::in);

            UAssert(input.is_open(), "Failed to open texture list file! (Path '%s')", filePath.c_str());
            while (input.eof() == false)
            {
                //zero it out
                memset(buffer, 0, sizeof(char) * 512);

                //get the line
                input.getline(buffer, 512);

                //if nothing on line, or # comment, continue;
                if (buffer[ 0 ] == '#' || strlen(buffer) == 0)
                    continue;

                //use string, and vector for holding tokens
                std::string data(buffer);
                std::vector<std::string> tokens;

                //deal with data, chop it up by space
                size_t pos = data.find_first_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", 0);
                while (pos != std::string::npos)
                {
                    size_t end = data.find_first_of(" ", pos + 1);
                    if (end - pos > 1)
                        tokens.push_back(data.substr(pos, end - pos + 1));
                    pos = data.find_first_of(" ", end);
                }

                //0 is filename
                tokens[ 0 ].insert(0, filePath);
                tokens[ 0 ].pop_back();
                //1 is name
                tokens[ 1 ].erase(0, 1);
                tokens[ 1 ].pop_back();
                //width
                unsigned width = atoi(tokens[ 2 ].c_str());
                //height
                unsigned height = atoi(tokens[ 2 ].c_str());


                TextureLoadBackend(tokens[ 1 ], tokens[ 0 ], width, height);
            }

            input.close();

            /////////////////////////////////////////////////////////////////
            // CREATING SAMPLER STATES //////////////////////////////////////
            m_samplerStateList_.resize(SAMPLER_COUNT);

            HRESULT result;
            D3D11_SAMPLER_DESC samplerDesc;

            // Texture Wrap /////////////////////////////////////////////////
            samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
            samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
            samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
            samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
            samplerDesc.MipLODBias = 0.0f;
            samplerDesc.MaxAnisotropy = 16;
            samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
            samplerDesc.BorderColor[ 0 ] = 0;
            samplerDesc.BorderColor[ 1 ] = 0;
            samplerDesc.BorderColor[ 2 ] = 0;
            samplerDesc.BorderColor[ 3 ] = 0;
            samplerDesc.MinLOD = 0;
            samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

            //Create the texture sampler state.
            result = device->CreateSamplerState(&samplerDesc, &m_samplerStateList_[ SAMPLER_WRAP_TEX ]);
            UAssert(result == S_OK, "Failed to make sampler state!");

            // no sampling //////////////////////////////////////////////////
            samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
            samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
            samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
            samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
            samplerDesc.MipLODBias = 0.0f;
            samplerDesc.MaxAnisotropy = 16;
            samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
            samplerDesc.BorderColor[ 0 ] = 0;
            samplerDesc.BorderColor[ 1 ] = 0;
            samplerDesc.BorderColor[ 2 ] = 0;
            samplerDesc.BorderColor[ 3 ] = 0;
            samplerDesc.MinLOD = 0;
            samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

            //Create the texture sampler state.
            result = device->CreateSamplerState(&samplerDesc, &m_samplerStateList_[ SAMPLER_NO_FILTERING ]);
            UAssert(result == S_OK, "Failed to make sampler state!");
        }

        void TextureManager::Uninitialize(void)
        {
            for (auto i : m_textureList)
            {
                if (i.second != nullptr)
                {
                    RELEASE_RESOURCE(i.second->m_shaderResource);
                    RELEASE_RESOURCE(i.second->m_texture2d);
                }

                delete i.second;
            }

            m_textureList.clear();

            for (int x = 0; x < SAMPLER_COUNT; ++x)
            {
                RELEASE_RESOURCE(m_samplerStateList_[ x ])
            }

            m_device = nullptr;
            m_deviceContext = nullptr;
        }

        void TextureManager::MapTextureByName(const std::string name, const unsigned int bufferIndex)
        {
            m_deviceContext->PSSetShaderResources(bufferIndex, 1, &m_textureList[ name ]->m_shaderResource);
        }

        void TextureManager::MapTextureByID(const unsigned ID, const unsigned int bufferIndex)
        {
            m_deviceContext->PSSetShaderResources(bufferIndex, 1, &m_hashTextureList[ ID ]->m_shaderResource);
        }

        void TextureManager::MapSamplerState(const Sampler type, const unsigned bufferIndex)
        {
            m_deviceContext->PSSetSamplers(bufferIndex, 1, &m_samplerStateList_[ type ]);
        }

        unsigned TextureManager::GetTextureIDByName(const std::string name)
        {
            return m_lookupTextureList[ name ];
        }

        GfxHND TextureManager::CreateDynamicTexture(const unsigned width, const unsigned height)
        {
            HRESULT result;
            D3D11_TEXTURE2D_DESC desc;
            GfxHND handle;
            _RESOURCEHND *id = HND_RSRCE(handle);

            //create a name for this guy
            std::stringstream strName;
            strName << "DynamicTex" << m_textureCount;

            std::string name = strName.str();

            //alloc texture
            m_textureList[ name ] = new Texture();

            //setup texture2d desc
            desc.Width = width;
            desc.Height = height;
            desc.MipLevels = desc.ArraySize = 1;
            desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            desc.SampleDesc.Count = 1;
            desc.SampleDesc.Quality = 0;
            desc.Usage = D3D11_USAGE_DYNAMIC;
            desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
            desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            desc.MiscFlags = 0;

            //create the texture2d
            result = m_device->CreateTexture2D(&desc, nullptr, &m_textureList[ name ]->m_texture2d);
            UAssert(result == S_OK, "Failed to create texture2D description! (Error '%s')", DXCore::GetDXErrorMessage(result));

            //create shader resource
            D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
            shaderResourceViewDesc.Format = desc.Format;
            shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
            shaderResourceViewDesc.Texture2D.MipLevels = 1;

            result = m_device->CreateShaderResourceView(m_textureList[ name ]->m_texture2d, &shaderResourceViewDesc, &m_textureList[ name ]->m_shaderResource);
            UAssert(result == S_OK, "Failed to create shader resource view! (Error '%s')", DXCore::GetDXErrorMessage(result));

            //set information
            m_textureList[ name ]->m_width = width;
            m_textureList[ name ]->m_height = height;
            m_lookupTextureList[ name ] = m_textureCount;
            m_hashTextureList[ m_textureCount ] = m_textureList[ name ];

            id->ID_ = SANITY_RESOURCE;
            id->Type_ = ID_TEXTURE;
            id->Index_ = m_textureCount++;

            return handle;
        }

        Texture* TextureManager::GetDynamicTexture(GfxHND &handle)
        {
            int id;

            _RESOURCEHND *hnd = HND_RSRCE(handle);

            //make sure it's a resource
            UAssert(hnd->ID_ == SANITY_RESOURCE, "Attempted to get dynamic texture with invalid handle!");

            //make sure it's of the right type
            UAssert(hnd->Type_ == ID_TEXTURE, "Attempted to get dynamic texture with handle of invalid type!");

            id = hnd->Index_;

            //make sure valid resource was given
            UAssert(m_hashTextureList[ id ] != nullptr, "Tried to get invalid dynamic texture!");

            //check to see if this texture is actually a dynamic texture
            //if it's static, its tex2d ptr will be null
            UAssert(m_hashTextureList[ id ]->m_texture2d != nullptr, "Tried to get static texture!");

            return m_hashTextureList[ id ];
        }

        void TextureManager::ResizeDynamicTexture(GfxHND &handle, const unsigned width, const unsigned height)
        {
            D3D11_TEXTURE2D_DESC desc;
            D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
            HRESULT result;
            int id;

            _RESOURCEHND *hnd = HND_RSRCE(handle);

            //make sure it's a resource
            UAssert(hnd->ID_ == SANITY_RESOURCE, "Attempted to resize dynamic texture with invalid handle!");

            //make sure it's of the right type
            UAssert(hnd->Type_ == ID_TEXTURE, "Attempted to resize dynamic texture with handle of invalid type!");

            id = hnd->Index_;

            //make sure valid resource was given
            UAssert(m_hashTextureList[ id ] != nullptr, "Tried to resize invalid dynamic texture!");

            //check to see if this texture is actually a dynamic texture
                //if it's static, its tex2d ptr will be null
            UAssert(m_hashTextureList[ id ]->m_texture2d != nullptr, "Tried to resize static texture!");

            //release current resources
            RELEASE_RESOURCE(m_hashTextureList[ id ]->m_shaderResource);
            RELEASE_RESOURCE(m_hashTextureList[ id ]->m_texture2d);

            //re-create the buffers, update the values in the texture
            m_hashTextureList[ id ]->m_width = width;
            m_hashTextureList[ id ]->m_height = height;

            //set data
            desc.Width = width;
            desc.Height = height;
            desc.MipLevels = desc.ArraySize = 1;
            desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            desc.SampleDesc.Count = 1;
            desc.SampleDesc.Quality = 0;
            desc.Usage = D3D11_USAGE_DYNAMIC;
            desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
            desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            desc.MiscFlags = 0;

            //create the 2d tex
            result = m_device->CreateTexture2D(&desc, nullptr, &m_hashTextureList[ id ]->m_texture2d);
            UAssert(result == S_OK, "Failed to create texture2D description! (Error '%s')", DXCore::GetDXErrorMessage(result));

            //create shader resource
            shaderResourceViewDesc.Format = desc.Format;
            shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
            shaderResourceViewDesc.Texture2D.MipLevels = 1;

            //create shader resource
            result = m_device->CreateShaderResourceView(m_hashTextureList[ id ]->m_texture2d, &shaderResourceViewDesc, &m_hashTextureList[ id ]->m_shaderResource);
            UAssert(result == S_OK, "Failed to create shader resource view! (Error '%s')", DXCore::GetDXErrorMessage(result));
        }

        void TextureManager::DestroyDynamicTexture(GfxHND& handle)
        {
            int id;

            _RESOURCEHND *hnd = HND_RSRCE(handle);

            //make sure it's a resource
            UAssert(hnd->ID_ == SANITY_RESOURCE, "Attempted to destroy dynamic texture with invalid handle!");

            //make sure it's of the right type
            UAssert(hnd->Type_ == ID_TEXTURE, "Attempted to destroy dynamic texture with handle of invalid type!");

            id = hnd->ID_;

            //make sure valid resource was given
            UAssert(m_hashTextureList[ id ] != nullptr, "Tried to destroy invalid dynamic texture!");

            //check to see if this texture is actually a dynamic texture
            //if it's static, its tex2d ptr will be null
            UAssert(m_hashTextureList[ id ]->m_texture2d != nullptr, "Tried to destroy static texture!");
        
            RELEASE_RESOURCE(m_hashTextureList[ id ]->m_shaderResource);
            RELEASE_RESOURCE(m_hashTextureList[ id ]->m_texture2d);

            handle = 0;
        }   

        void TextureManager::TextureLoadBackend(const std::string name, const std::string path, const unsigned width, const unsigned height)
        {
            HRESULT result;
            D3D11_TEXTURE2D_DESC desc;

            LogMessage("Texture: %s", 2, name.c_str());
            LogMessage("Path: %s", 3, path.c_str());
            LogMessage("Width: %i", 3, width);
            LogMessage("Height: %i", 3, height);

            //width/height
            desc.Width = width;
            desc.Height = height;
            desc.MipLevels = desc.ArraySize = 1;
            desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            desc.SampleDesc.Count = 2;
            desc.SampleDesc.Quality = 0;
            desc.Usage = D3D11_USAGE_IMMUTABLE;
            desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
            desc.CPUAccessFlags = 0;
            desc.MiscFlags = 0;

            m_textureList[ name ] = new Texture();

            result = DirectX::CreateDDSTextureFromFile(m_device, strToWchart(path), nullptr, &m_textureList[ name ]->m_shaderResource);

            UAssert(result != HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND), "Texture '%s' was not found!", path.c_str());
            UAssert(result == S_OK, "Failed to load texture: '%s'", name.c_str());

            m_textureList[ name ]->m_width = width;
            m_textureList[ name ]->m_height = height;
            m_lookupTextureList[ name ] = m_textureCount;
            m_hashTextureList[ m_textureCount++ ] = m_textureList[ name ];
        }
    }
}
