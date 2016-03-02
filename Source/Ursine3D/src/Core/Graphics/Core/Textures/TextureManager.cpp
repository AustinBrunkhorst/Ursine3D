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
#include "InternalResourceByteCode.h"

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

        TextureManager::TextureManager()
            : m_device( nullptr )
            , m_deviceContext( nullptr )
            , m_textureCount( 1 )
            , m_textureCache( { Texture() } )
        {
        }

        void TextureManager::Initialize(ID3D11Device *device, ID3D11DeviceContext *context, std::string filePath)
        {
            m_device = device;
            m_deviceContext = context;

            m_textureCount = 0;

            /////////////////////////////////////////////////////////////////
            // CREATE DEFAULT TEXTURE
            {
                InitalizeTexture(
                    const_cast<uint8_t*>( graphics_resources::kMissingTexture ), 
                    5608 * sizeof( uint8_t ), 
                    32, 
                    32, 
                    m_textureCache[ 0 ]
                );

                LoadTextureToGPU( m_textureCache[ 0 ] );
            }

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


                CreateTexture(tokens[ 1 ], tokens[ 0 ], width, height);
            }

            input.close();

            /////////////////////////////////////////////////////////////////
            // CREATING SAMPLER STATES //////////////////////////////////////
            m_samplerStateList_.resize(SAMPLER_STATE_COUNT);

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
            result = device->CreateSamplerState(&samplerDesc, &m_samplerStateList_[ SAMPLER_STATE_WRAP_TEX ]);
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
            result = device->CreateSamplerState(&samplerDesc, &m_samplerStateList_[ SAMPLER_STATE_NO_FILTERING ]);
            UAssert(result == S_OK, "Failed to make sampler state!");

            // shadow sampler //////////////////////////////////////////////////
            samplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
            samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
            samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
            samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
            samplerDesc.MipLODBias = 0.0f;
            samplerDesc.MaxAnisotropy = 0;
            samplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS;
            samplerDesc.BorderColor[ 0 ] = 1.0;
            samplerDesc.BorderColor[ 1 ] = 1.0;
            samplerDesc.BorderColor[ 2 ] = 1.0;
            samplerDesc.BorderColor[ 3 ] = 1.0;
            samplerDesc.MinLOD = 0;
            samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

            //Create the texture sampler state.
            result = device->CreateSamplerState(&samplerDesc, &m_samplerStateList_[ SAMPLER_STATE_SHADOW ]);
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

            for (int x = 0; x < SAMPLER_STATE_COUNT; ++x)
            {
                RELEASE_RESOURCE(m_samplerStateList_[ x ])
            }

            m_device = nullptr;
            m_deviceContext = nullptr;
        }


        GfxHND TextureManager::CreateTexture(const std::string name, const std::string path, const unsigned width, const unsigned height)
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

            GfxHND handle;
            _RESOURCEHND *id = HND_RSRCE(handle);

            id->ID_ = SANITY_RESOURCE;
            id->Type_ = ID_TEXTURE;
            id->Index_ = m_textureCount++;

            return handle;
        }

        GfxHND TextureManager::CreateTexture(const uint8_t* binaryData, size_t size, const std::string& name, const unsigned width, const unsigned height)
        {
            HRESULT result;
            D3D11_TEXTURE2D_DESC desc;

            LogMessage("Texture: %s", 2, name.c_str());
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

            result = DirectX::CreateDDSTextureFromMemory(m_device, binaryData, size, nullptr, &m_textureList[ name ]->m_shaderResource);

            UAssert(result == S_OK, "Failed to load texture: '%s'", name.c_str());

            m_textureList[ name ]->m_width = width;
            m_textureList[ name ]->m_height = height;
            m_lookupTextureList[ name ] = m_textureCount;
            m_hashTextureList[ m_textureCount++ ] = m_textureList[ name ];

            GfxHND handle;
            _RESOURCEHND *id = HND_RSRCE(handle);

            id->ID_ = SANITY_RESOURCE;
            id->Type_ = ID_TEXTURE;
            id->Index_ = m_textureCount++;

            return handle;
        }

        void TextureManager::MapTextureByName(const std::string name, const unsigned int bufferIndex)
        {
            if( m_textureList[ name ] == nullptr )
                m_deviceContext->PSSetShaderResources(bufferIndex, 1, &m_textureList[ "Blank" ]->m_shaderResource);
            else
                m_deviceContext->PSSetShaderResources(bufferIndex, 1, &m_textureList[ name ]->m_shaderResource);
        }

        void TextureManager::MapTextureByID(const unsigned ID, const unsigned int bufferIndex)
        {
            m_deviceContext->PSSetShaderResources(bufferIndex, 1, &m_hashTextureList[ ID ]->m_shaderResource);
        }

        void TextureManager::MapSamplerState(const SAMPLER_STATES type, const unsigned bufferIndex)
        {
            if( type == SAMPLER_STATE_COUNT)
                return;
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

        GfxHND TextureManager::CreateTexture(uint8_t * binaryData, size_t binarySize, unsigned width, unsigned height)
        {
            // if this texture already exists and we are updating it, release the prior resource, keep old ID
            size_t internalID;

            GfxHND handle;
            _RESOURCEHND *id = HND_RSRCE(handle);

            internalID = m_textureCache.size( );
            m_textureCache.push_back( Texture( ) );

            // load it up into CPU memory
            InitalizeTexture(
                binaryData, 
                binarySize, 
                width, 
                height, 
                m_textureCache[internalID]
            );

            // initialize handle
            id->ID_ = SANITY_RESOURCE;
            id->Type_ = ID_TEXTURE;
            id->Index_ = static_cast<unsigned>( internalID );

            return handle;
        }

        void TextureManager::DestroyTexture(GfxHND &handle)
        {
            int id;
            _RESOURCEHND *hnd = HND_RSRCE(handle);
            UAssert(hnd->ID_ == SANITY_RESOURCE, "Attempted to get dynamic texture with invalid handle!");
            UAssert(hnd->Type_ == ID_TEXTURE, "Attempted to get dynamic texture with handle of invalid type!");

            id = hnd->Index_;

            Texture &texture = m_textureCache[ id ];
            RELEASE_RESOURCE( texture.m_shaderResource );
            RELEASE_RESOURCE( texture.m_texture2d );

            delete[] texture.m_binaryData;
            texture.m_binarySize = 0;
            texture.m_width = 0;
            texture.m_height = 0;
            texture.m_internalID = 0;
            texture.m_referenceCount = 0;

            handle = 0;
        }

        void TextureManager::LoadTexture(GfxHND handle)
        {
            int id;
            _RESOURCEHND *hnd = HND_RSRCE(handle);

            if (handle != 0)
            {
                UAssert(hnd->ID_ == SANITY_RESOURCE, "Attempted to get texture with invalid handle!");
                UAssert(hnd->Type_ == ID_TEXTURE, "Attempted to get texture with handle of invalid type!");
            }

            id = hnd->Index_;

            if (id == 0)
                return;

            // if it doesn't exist on the GPU, load it up
            if(m_textureCache[ id ].m_referenceCount == 0)
                LoadTextureToGPU( m_textureCache[ id ] );

            ++(m_textureCache[id].m_referenceCount);
        }

        void TextureManager::UnloadTexture(GfxHND handle)
        {
            int id;
            _RESOURCEHND *hnd = HND_RSRCE(handle);

            if(handle != 0)
            {
                UAssert(hnd->ID_ == SANITY_RESOURCE, "Attempted to get texture with invalid handle!");
                UAssert(hnd->Type_ == ID_TEXTURE, "Attempted to get texture with handle of invalid type!");
            }

            id = hnd->Index_;
            if(id == 0)
                return;

            --(m_textureCache[ id ].m_referenceCount);

            // if out of references, free up the GPU
            if(m_textureCache[ id ].m_referenceCount == 0)
            {
                RELEASE_RESOURCE( m_textureCache[id].m_shaderResource );
                RELEASE_RESOURCE( m_textureCache[id].m_texture2d );
            }
        }

        void TextureManager::GetBinaryInformation(GfxHND handle, uint8_t **dataPtr, size_t &binarySize)
        {
            int id;
            _RESOURCEHND *hnd = HND_RSRCE(handle);
            UAssert(hnd->ID_ == SANITY_RESOURCE, "Attempted to get dynamic texture with invalid handle!");
            UAssert(hnd->Type_ == ID_TEXTURE, "Attempted to get dynamic texture with handle of invalid type!");

            id = hnd->Index_;

            Texture &texture = m_textureCache[ id ];

            *dataPtr = texture.m_binaryData;
            binarySize = texture.m_binarySize;
        }

        void TextureManager::MapResourceTextureByID(const unsigned ID, const unsigned int bufferIndex)
        {
            if(ID >= m_textureCache.size( ) || m_textureCache[ ID ].m_shaderResource == nullptr)
            {
                m_deviceContext->PSSetShaderResources(bufferIndex, 1, &m_textureCache[ 0 ].m_shaderResource);
                return;
            }

            m_deviceContext->PSSetShaderResources(bufferIndex, 1, &m_textureCache[ ID ].m_shaderResource);
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

        void TextureManager::LoadTextureToGPU(Texture &texture) const
        {
            HRESULT result;
            D3D11_TEXTURE2D_DESC desc;

            //width/height
            desc.Width = texture.m_width;
            desc.Height = texture.m_height;
            desc.MipLevels = desc.ArraySize = 1;
            desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            desc.SampleDesc.Count = 2;
            desc.SampleDesc.Quality = 0;
            desc.Usage = D3D11_USAGE_IMMUTABLE;
            desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
            desc.CPUAccessFlags = 0;
            desc.MiscFlags = 0;

            result = DirectX::CreateDDSTextureFromMemory(
                m_device, 
                texture.m_binaryData, 
                texture.m_binarySize, 
                nullptr, 
                &texture.m_shaderResource
            );

            UAssert( result == S_OK, "Failed to load texture (%s)", DXCore::GetDXErrorMessage(result) );
        }

        void TextureManager::InitalizeTexture(uint8_t *binaryData, size_t binarySize, unsigned width, unsigned height, Texture &texture) const
        {
            texture.m_binaryData = new uint8_t[ binarySize ];
            texture.m_binarySize = binarySize;
            memcpy(texture.m_binaryData, binaryData, binarySize);

            uint32_t dwMagicNumber = *(const uint32_t*)(binaryData);

            UAssert(0x20534444 == dwMagicNumber, "Binary .dds file has mismatching magic number! (%u provided)", dwMagicNumber);

            texture.m_width = width;
            texture.m_height = height;
            texture.m_referenceCount = 0;
        }
    }
}
