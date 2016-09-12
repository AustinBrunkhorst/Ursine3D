/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** DepthStencilManager.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"
#include "DepthStencilManager.h"
#include <d3d11.h>
#include "GfxDefines.h"
#include "DXErrorHandling.h"

namespace ursine
{
    namespace graphics
    {
        namespace DXCore
        {
            void DepthStencilManager::Initialize(ID3D11Device *device, ID3D11DeviceContext *devicecontext, const int width, const int height, GfxInfo *gfxInfo)
            {
                m_device        = device;
                m_deviceContext = devicecontext;
                m_gfxInfo       = gfxInfo;

                m_depthStencilArray.resize( DEPTH_STENCIL_COUNT );
                
                createInternalDepthTarget(
                    DEPTH_STENCIL_MAIN, 
                    width, 
                    height, 
                    DXGI_FORMAT_R32_TYPELESS,
                    DXGI_FORMAT_D32_FLOAT,
                    DXGI_FORMAT_R32_FLOAT
                );

                createInternalDepthTarget(
                    DEPTH_STENCIL_OVERDRAW, 
                    width, 
                    height, 
                    DXGI_FORMAT_R32_TYPELESS,
                    DXGI_FORMAT_D32_FLOAT,
                    DXGI_FORMAT_R32_FLOAT
                );

                createInternalDepthTarget(
                    DEPTH_STENCIL_SHADOWMAP, 
                    1024, 
                    1024, 
                    DXGI_FORMAT_R32_TYPELESS, 
                    DXGI_FORMAT_D32_FLOAT,
                    DXGI_FORMAT_R32_FLOAT
                );
            }

            void DepthStencilManager::Uninitialize(void)
            {
                for (auto x : m_depthStencilArray)
                {
                    destroyTarget(x);
                }

                for (auto x : m_shadowMapArray)
                {
                    destroyTarget( x );
                }

                m_deviceContext = nullptr;
                m_device = nullptr;
            }

            ID3D11DepthStencilView *DepthStencilManager::GetDepthStencilView(const DEPTH_STENCIL_LIST stencil)
            {
                if(stencil != DEPTH_STENCIL_COUNT)
                    return m_depthStencilArray[ stencil ].depthStencilView;

                return nullptr;
            }

            ID3D11ShaderResourceView *DepthStencilManager::GetDepthStencilSRV(const DEPTH_STENCIL_LIST stencil)
            {
                return m_depthStencilArray[ stencil ].depthStencilSRV;
            }

            GfxHND DepthStencilManager::CreateShadowmapDepthTarget(unsigned width, unsigned height)
            {
                GfxHND handle;
                _RESOURCEHND *newResource = reinterpret_cast<_RESOURCEHND*>( &handle );
                unsigned index;

                // figure out what index to use
                if(m_shadowmapFreeStack.size( ) == 0)
                {
                    index = static_cast<unsigned>( m_shadowMapArray.size( ) );
                    m_shadowMapArray.push_back( DepthStencil( ) );
                }
                else
                {
                    index = m_shadowmapFreeStack.top( );
                    m_shadowmapFreeStack.pop( );
                }

                // create the target
                createShadowmapTarget(
                    index, 
                    width, 
                    height, 
                    DXGI_FORMAT_R32_TYPELESS,
                    DXGI_FORMAT_D32_FLOAT,
                    DXGI_FORMAT_R32_FLOAT
                );

                // assemble handle
                newResource->Index_ = index;
                newResource->ID_    = ID_DEPTH_TARGET;
                newResource->Type_  = ID_DEPTH_TARGET;

                return handle;
            }

            void DepthStencilManager::DestroyShadowmapDepthTarget(GfxHND &handle)
            {
                _RESOURCEHND *shadowmap = reinterpret_cast<_RESOURCEHND*>(&handle);

                UAssert( shadowmap->ID_ == ID_DEPTH_TARGET, "Tried to destroy shadowmap with invalid handle!" );
                UAssert( shadowmap->Type_ == ID_DEPTH_TARGET, "Tried to destroy shadowmap with invalid type!" );

                destroyTarget( m_shadowMapArray[ shadowmap->Index_ ] );
                m_shadowmapFreeStack.push( shadowmap->Index_ );
            }

            DepthStencilManager::DepthStencil & DepthStencilManager::GetShadowmapDepthStencil(GfxHND handle)
            {
                _RESOURCEHND *shadowmap = reinterpret_cast<_RESOURCEHND*>(&handle);

                UAssert(shadowmap->ID_ == ID_DEPTH_TARGET, "Tried to get shadowmap with invalid handle!");
                UAssert(shadowmap->Type_ == ID_DEPTH_TARGET, "Tried to get shadowmap with invalid type!");

                return m_shadowMapArray[ shadowmap->Index_ ];
            }

            void DepthStencilManager::ResizeShadowmapDepthTarget(GfxHND handle, unsigned width, unsigned height)
            {
                _RESOURCEHND *shadowmap = reinterpret_cast<_RESOURCEHND*>(&handle);

                UAssert(shadowmap->ID_ == ID_DEPTH_TARGET, "Tried to destroy shadowmap with invalid handle!");
                UAssert(shadowmap->Type_ == ID_DEPTH_TARGET, "Tried to destroy shadowmap with invalid type!");

                destroyTarget(m_shadowMapArray[ shadowmap->Index_ ]);

                createShadowmapTarget(
                    shadowmap->Index_, 
                    width, 
                    height, 
                    DXGI_FORMAT_R32_TYPELESS,
                    DXGI_FORMAT_D32_FLOAT,
                    DXGI_FORMAT_R32_FLOAT
                );
            }

            void DepthStencilManager::ResizeMainDepthTargets(const int width, const int height)
            {
                for (auto x : m_depthStencilArray)
                {
                    destroyTarget( x );
                }

                Initialize(m_device, m_deviceContext, width, height, m_gfxInfo);
            }

            void DepthStencilManager::createInternalDepthTarget(DEPTH_STENCIL_LIST target, unsigned width, unsigned height, DXGI_FORMAT depthBufferDescFormat, DXGI_FORMAT viewDescFormat, DXGI_FORMAT srvFormat)
            {
                /////////////////////////////////////////////////////////////////
                //create main depth stencil
                HRESULT result;
                D3D11_TEXTURE2D_DESC depthBufferDesc;
                ZeroMemory( &depthBufferDesc, sizeof(depthBufferDesc) );

                //Set up the description of the depth buffer.
                depthBufferDesc.Width               = width;
                depthBufferDesc.Height              = height;
                depthBufferDesc.MipLevels           = 1;
                depthBufferDesc.ArraySize           = 1;
                depthBufferDesc.Format              = depthBufferDescFormat;
                depthBufferDesc.SampleDesc.Count    = m_gfxInfo->GetSampleCount( );
                depthBufferDesc.SampleDesc.Quality  = m_gfxInfo->GetSampleQuality( );
                depthBufferDesc.Usage               = D3D11_USAGE_DEFAULT;
                depthBufferDesc.BindFlags           = D3D11_BIND_DEPTH_STENCIL | D3D10_BIND_SHADER_RESOURCE;
                depthBufferDesc.CPUAccessFlags      = 0;
                depthBufferDesc.MiscFlags           = 0;

                // Create the texture for the depth buffer using the filled out description.
                result = m_device->CreateTexture2D(
                    &depthBufferDesc, 
                    nullptr, 
                    &m_depthStencilArray[ target ].depthStencilTex2D
                );
                UAssert( result == S_OK, "Depth buffer texture failed! (Error '%s')", GetDXErrorMessage( result ) );

                // create the view description
                D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
                ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

                depthStencilViewDesc.Format             = viewDescFormat;
                depthStencilViewDesc.Texture2D.MipSlice = 0;
                depthStencilViewDesc.ViewDimension      = 
                    (depthBufferDesc.SampleDesc.Count > 1)
                    ? D3D11_DSV_DIMENSION_TEXTURE2DMS 
                    : D3D11_DSV_DIMENSION_TEXTURE2D;

                result = m_device->CreateDepthStencilView(
                    m_depthStencilArray[ target ].depthStencilTex2D,
                    &depthStencilViewDesc, 
                    &m_depthStencilArray[ target ].depthStencilView
                );
                UAssert( result == S_OK, "Failed to make depth stencil view! (Error '%s')", GetDXErrorMessage( result ) );

                // create the shader resource view
                D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
                srvDesc.Format                          = srvFormat;
                srvDesc.Texture2D.MipLevels             = 1;
                srvDesc.Texture2D.MostDetailedMip       = 0;
                srvDesc.ViewDimension                   = 
                    depthStencilViewDesc.ViewDimension == D3D11_DSV_DIMENSION_TEXTURE2D 
                    ? D3D11_SRV_DIMENSION_TEXTURE2D 
                    : D3D11_SRV_DIMENSION_TEXTURE2DMS;

                

                result = m_device->CreateShaderResourceView(
                    m_depthStencilArray[ target ].depthStencilTex2D, 
                    &srvDesc, 
                    &m_depthStencilArray[ target ].depthStencilSRV
                );

                UAssert( result == S_OK, "Failed to make depth stencil shader resource view! (Error '%s')", GetDXErrorMessage(result) );

                m_depthStencilArray[ target ].width     = width;
                m_depthStencilArray[ target ].height    = height;
            }

            void DepthStencilManager::createShadowmapTarget(unsigned index, unsigned width, unsigned height, DXGI_FORMAT depthBufferDescFormat, DXGI_FORMAT viewDescFormat, DXGI_FORMAT srvFormat)
            {
                /////////////////////////////////////////////////////////////////
                //create main depth stencil
                HRESULT result;
                D3D11_TEXTURE2D_DESC depthBufferDesc;
                ZeroMemory( &depthBufferDesc, sizeof(depthBufferDesc) );

                //Set up the description of the depth buffer.
                depthBufferDesc.Width               = width;
                depthBufferDesc.Height              = height;
                depthBufferDesc.MipLevels           = 1;
                depthBufferDesc.ArraySize           = 1;
                depthBufferDesc.Format              = depthBufferDescFormat;
                depthBufferDesc.SampleDesc.Count    = m_gfxInfo->GetSampleCount( );
                depthBufferDesc.SampleDesc.Quality  = m_gfxInfo->GetSampleQuality( );
                depthBufferDesc.Usage               = D3D11_USAGE_DEFAULT;
                depthBufferDesc.BindFlags           = D3D11_BIND_DEPTH_STENCIL | D3D10_BIND_SHADER_RESOURCE;
                depthBufferDesc.CPUAccessFlags      = 0;
                depthBufferDesc.MiscFlags           = 0;

                // Create the texture for the depth buffer using the filled out description.
                result = m_device->CreateTexture2D(
                    &depthBufferDesc, 
                    nullptr, 
                    &m_shadowMapArray[ index ].depthStencilTex2D
                );
                UAssert( result == S_OK, "Depth buffer texture failed! (Error '%s')", GetDXErrorMessage( result ) );

                // create the view description
                D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
                ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

                depthStencilViewDesc.Format             = viewDescFormat;
                depthStencilViewDesc.Texture2D.MipSlice = 0;
                depthStencilViewDesc.ViewDimension      = 
                    (depthBufferDesc.SampleDesc.Count > 1)
                    ? D3D11_DSV_DIMENSION_TEXTURE2DMS 
                    : D3D11_DSV_DIMENSION_TEXTURE2D;

                result = m_device->CreateDepthStencilView(
                    m_shadowMapArray[ index ].depthStencilTex2D,
                    &depthStencilViewDesc, 
                    &m_shadowMapArray[ index ].depthStencilView
                );
                UAssert( result == S_OK, "Failed to make depth stencil view! (Error '%s')", GetDXErrorMessage( result ) );

                // create the shader resource view
                D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
                srvDesc.Format                          = srvFormat;
                srvDesc.Texture2D.MipLevels             = 1;
                srvDesc.Texture2D.MostDetailedMip       = 0;
                srvDesc.ViewDimension                   = 
                    depthStencilViewDesc.ViewDimension == D3D11_DSV_DIMENSION_TEXTURE2D 
                    ? D3D11_SRV_DIMENSION_TEXTURE2D 
                    : D3D11_SRV_DIMENSION_TEXTURE2DMS;

                

                result = m_device->CreateShaderResourceView(
                    m_shadowMapArray[ index ].depthStencilTex2D,
                    &srvDesc, 
                    &m_shadowMapArray[ index ].depthStencilSRV
                );

                UAssert( result == S_OK, "Failed to make depth stencil shader resource view! (Error '%s')", GetDXErrorMessage(result) );

                m_shadowMapArray[ index ].width     = width;
                m_shadowMapArray[ index ].height    = height;
            }

            void DepthStencilManager::destroyTarget(DepthStencil &depthTarget)
            {
                RELEASE_RESOURCE( depthTarget.depthStencilView );
                RELEASE_RESOURCE( depthTarget.depthStencilSRV );
                RELEASE_RESOURCE( depthTarget.depthStencilTex2D );
            }
        }
    }
}
