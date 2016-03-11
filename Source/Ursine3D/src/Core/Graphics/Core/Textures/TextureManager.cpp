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
#include "FontManager.h"

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

        // public methods

        TextureManager::TextureManager()
            : m_device( nullptr )
            , m_deviceContext( nullptr )
            , m_textureCount( 2 )
            , m_textureCache( { Texture(), Texture() } )
        {
        }

        void TextureManager::Initialize(ID3D11Device *device, ID3D11DeviceContext *context, std::string filePath)
        {
            m_device = device;
            m_deviceContext = context;

            m_textureCount = 0;

            ///////////////////////////////////////////////////////////////////
            // CREATE DEFAULT TEXTURE
            {
                InitalizeTexture(
                    const_cast<uint8_t*>( graphics_resources::kMissingTexture ), 
                    5608 * sizeof( uint8_t ), 
                    32, 
                    32, 
                    m_textureCache[ INTERNAL_MISSING_TEX ]
                );

                LoadTextureToGPU( m_textureCache[ INTERNAL_MISSING_TEX ] );
            }

            ///////////////////////////////////////////////////////////////////
            // CREATE BLANK TEXTURE
            {
                InitalizeTexture(
                    const_cast<uint8_t*>(graphics_resources::kBlankTexture),
                    232 * sizeof(uint8_t),
                    4,
                    4,
                    m_textureCache[ INTERNAL_BLANK_TEX ]
                );

                LoadTextureToGPU(m_textureCache[ INTERNAL_BLANK_TEX ]);
            }

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
            m_textureCache.clear( );

            for (int x = 0; x < SAMPLER_STATE_COUNT; ++x)
            {
                RELEASE_RESOURCE(m_samplerStateList_[ x ])
            }

            m_device = nullptr;
            m_deviceContext = nullptr;
        }

        GfxHND TextureManager::CreateTexture(const uint8_t *binaryData, size_t binarySize, unsigned width, unsigned height)
        {
            // if this texture already exists and we are updating it, release the prior resource, keep old ID
            size_t internalID;

            std::cout << "CREATE" << std::endl;

            GfxHND handle;
            _RESOURCEHND *id = HND_RSRCE( handle );

            internalID = m_textureCache.size( );
            m_textureCache.emplace_back( );

            // load it up into CPU memory
            InitalizeTexture(
                binaryData,
                binarySize,
                width,
                height,
                m_textureCache[ internalID ]
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
            RELEASE_RESOURCE(texture.m_shaderResource);
            RELEASE_RESOURCE(texture.m_texture2d);

            std::cout << "DESTROY" << std::endl;

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
            if (m_textureCache[ id ].m_referenceCount == 0)
            {
                std::cout << "LOAD TO GPU: " << id << std::endl;
                LoadTextureToGPU(m_textureCache[ id ]);
            }

            ++(m_textureCache[ id ].m_referenceCount);
        }

        void TextureManager::UnloadTexture(GfxHND handle)
        {
            URSINE_TODO( "Fix this." );
            return;
            int id;
            _RESOURCEHND *hnd = HND_RSRCE(handle);

            if (handle > INTERNAL_BLANK_TEX)
            {
                UAssert(hnd->ID_ == SANITY_RESOURCE, "Attempted to get texture with invalid handle!");
                UAssert(hnd->Type_ == ID_TEXTURE, "Attempted to get texture with handle of invalid type!");
            }

            id = hnd->Index_;
            if (id <= INTERNAL_BLANK_TEX)
                return;

            --(m_textureCache[ id ].m_referenceCount);
            
            // if out of references, free up the GPU
            if (m_textureCache[ id ].m_referenceCount == 0)
            {
                RELEASE_RESOURCE(m_textureCache[ id ].m_shaderResource);
                RELEASE_RESOURCE(m_textureCache[ id ].m_texture2d);
            }
        }

        void TextureManager::LoadFontTextures(GfxHND font, FontManager *manager)
        {
            if (font == 0)
                return;

            auto &bmf = manager->GetBitmapFont( font );

            for(auto &tex : bmf.GetPageData( ))
            {
                auto handle = manager->GetTextureHandle(
                    font, 
                    tex
                );

                LoadTexture( handle );
            }
        }

        void TextureManager::UnloadFontTextures(GfxHND font, FontManager *manager)
        {
            if(font == 0)
                return;

            auto &bmf = manager->GetBitmapFont( font );

            for(auto &tex : bmf.GetPageData( ))
            {
                auto handle = manager->GetTextureHandle(
                    font, 
                    tex
                );

                UnloadTexture( handle );
            }
        }


        void TextureManager::MapTextureByID(const unsigned ID, const unsigned int bufferIndex)
        {
            if (ID == INTERNAL_MISSING_TEX || ID > m_textureCache.size() || m_textureCache[ ID ].m_shaderResource == nullptr || 
                m_textureCache[ ID ].m_shaderResource == nullptr)
            {
                m_deviceContext->PSSetShaderResources(bufferIndex, 1, &m_textureCache[ INTERNAL_MISSING_TEX ].m_shaderResource);
                return;
            }

            m_deviceContext->PSSetShaderResources(bufferIndex, 1, &m_textureCache[ ID ].m_shaderResource);
        }

        void TextureManager::MapTextureArrayByIDs(const unsigned bufferIndex, std::initializer_list<unsigned> IDs)
        {
            std::vector<ID3D11ShaderResourceView *> textureArray;

            // create this buffer
            for(auto &x : IDs)
            {
                textureArray.push_back(m_textureCache[x].m_shaderResource);
            }

            // map all of them!
            m_deviceContext->PSSetShaderResources(static_cast<UINT>(bufferIndex), static_cast<UINT>(textureArray.size( )), textureArray.data( ));
        }

        void TextureManager::MapSamplerState(const SAMPLER_STATES type, const unsigned bufferIndex)
        {
            if( type == SAMPLER_STATE_COUNT)
                return;
            m_deviceContext->PSSetSamplers(bufferIndex, 1, &m_samplerStateList_[ type ]);
        }

        GfxHND TextureManager::CreateDynamicTexture(const unsigned width, const unsigned height)
        {
            HRESULT result;
            D3D11_TEXTURE2D_DESC desc;
            GfxHND handle;
            _RESOURCEHND *id = HND_RSRCE(handle);

            size_t internalID = m_textureCache.size( );

            m_textureCache.push_back( Texture( ) );


            //setup texture2d desc
            desc.Width = width;
            desc.Height = height;
            desc.MipLevels = desc.ArraySize = 1;
            desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
            desc.SampleDesc.Count = 1;
            desc.SampleDesc.Quality = 0;
            desc.Usage = D3D11_USAGE_DYNAMIC;
            desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
            desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            desc.MiscFlags = 0;

            //create the texture2d
            result = m_device->CreateTexture2D(&desc, nullptr, &m_textureCache[ internalID ].m_texture2d);
            UAssert(result == S_OK, "Failed to create texture2D description! (Error '%s')", DXCore::GetDXErrorMessage(result));

            //create shader resource
            D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
            shaderResourceViewDesc.Format = desc.Format;
            shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
            shaderResourceViewDesc.Texture2D.MipLevels = 1;

            result = m_device->CreateShaderResourceView(m_textureCache[ internalID ].m_texture2d, &shaderResourceViewDesc, &m_textureCache[ internalID ].m_shaderResource);
            UAssert(result == S_OK, "Failed to create shader resource view! (Error '%s')", DXCore::GetDXErrorMessage(result));

            id->ID_ = SANITY_RESOURCE;
            id->Type_ = ID_TEXTURE;
            id->Index_ = internalID;

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

            return &m_textureCache[ id ];
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

            //release current resources
            RELEASE_RESOURCE(m_textureCache[ id ].m_shaderResource);
            RELEASE_RESOURCE(m_textureCache[ id ].m_texture2d);

            //re-create the buffers, update the values in the texture
            m_textureCache[ id ].m_width = width;
            m_textureCache[ id ].m_height = height;

            //set data
            desc.Width = width;
            desc.Height = height;
            desc.MipLevels = desc.ArraySize = 1;
            desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
            desc.SampleDesc.Count = 1;
            desc.SampleDesc.Quality = 0;
            desc.Usage = D3D11_USAGE_DYNAMIC;
            desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
            desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            desc.MiscFlags = 0;

            //create the 2d tex
            result = m_device->CreateTexture2D(&desc, nullptr, &m_textureCache[ id ].m_texture2d);
            UAssert(result == S_OK, "Failed to create texture2D description! (Error '%s')", DXCore::GetDXErrorMessage(result));

            //create shader resource
            shaderResourceViewDesc.Format = desc.Format;
            shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
            shaderResourceViewDesc.Texture2D.MipLevels = 1;

            //create shader resource
            result = m_device->CreateShaderResourceView(m_textureCache[ id ].m_texture2d, &shaderResourceViewDesc, &m_textureCache[ id ].m_shaderResource);
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

            RELEASE_RESOURCE(m_textureCache[ id ].m_shaderResource);
            RELEASE_RESOURCE(m_textureCache[ id ].m_texture2d);

            handle = 0;
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

        // private methods

        void TextureManager::LoadTextureToGPU(Texture &texture) const
        {
            HRESULT result;
            D3D11_TEXTURE2D_DESC desc;

            //width/height
            desc.Width = texture.m_width;
            desc.Height = texture.m_height;
            desc.MipLevels = desc.ArraySize = 1;
            desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
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

        void TextureManager::InitalizeTexture(const uint8_t *binaryData, size_t binarySize, unsigned width, unsigned height, Texture &texture) const
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
