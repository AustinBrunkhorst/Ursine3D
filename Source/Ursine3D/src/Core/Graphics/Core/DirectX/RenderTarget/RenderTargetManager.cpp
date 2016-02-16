/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** RenderTargetManager.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"
#include "RenderTargetManager.h"
#include "GfxDefines.h"
#include <d3d11.h>
#include "DXErrorHandling.h"


namespace ursine
{
    namespace graphics
    {
        namespace DXCore
        {
            void RenderTargetManager::Initialize(ID3D11Device *device, ID3D11DeviceContext *devicecontext)
            {
                m_device = device;
                m_deviceContext = devicecontext;

                m_renderTargets.resize(RENDER_TARGET_COUNT);

                m_currentTarget = RENDER_TARGET_COUNT;

                for (int x = 0; x < RENDER_TARGET_MAX_VIEWPORT; ++x)
                {
                    m_availableTargets.push_back((RENDER_TARGETS)(x + RENDER_TARGET_VIEWPORT_1));
                }
            }

            void RenderTargetManager::Uninitialize(void)
            {
                for (int x = 0; x < RENDER_TARGET_COUNT; ++x)
                {
                    if (m_renderTargets[ x ] != nullptr)
                    {
                        RELEASE_RESOURCE(m_renderTargets[ x ]->RenderTargetView);
                        RELEASE_RESOURCE(m_renderTargets[ x ]->ShaderMap);
                        RELEASE_RESOURCE(m_renderTargets[ x ]->TextureMap);

                        delete m_renderTargets[ x ];
                    }
                }

                UnInitializeAllRenderTargets();

                m_device = nullptr;
                m_deviceContext = nullptr;
            }

            void RenderTargetManager::InitializeAllRenderTargets(const unsigned width, const unsigned height)
            {
                //alloc arrays
                //this is 3 for a reason
                m_deferredTextureMap = new ID3D11Texture2D*[ 3 ];
                m_deferredRenderTargetView = new ID3D11RenderTargetView*[ 3 ];
                m_deferredShaderMap = new ID3D11ShaderResourceView*[ 3 ];
                m_forwardRenderTargetView = new ID3D11RenderTargetView*[ 3 ];

                //deferred render targets
                CreateRenderTarget(RENDER_TARGET_DEFERRED_COLOR, DXGI_FORMAT_R8G8B8A8_UNORM, width, height);
                CreateRenderTarget(RENDER_TARGET_DEFERRED_NORMAL, DXGI_FORMAT_R16G16B16A16_FLOAT, width, height);
                CreateRenderTarget(RENDER_TARGET_DEFERRED_SPECPOW, DXGI_FORMAT_R8G8B8A8_UNORM, width, height);
                CreateRenderTarget(RENDER_TARGET_LIGHTMAP, DXGI_FORMAT_R16G16B16A16_FLOAT, width, height);

                //create the reserved render targets
                CreateRenderTarget(RENDER_TARGET_UI, DXGI_FORMAT_R8G8B8A8_UNORM, width, height);
                CreateRenderTarget(RENDER_TARGET_DEBUG, DXGI_FORMAT_R16G16B16A16_FLOAT, width, height);

                //set this for mapping data
                m_deferredRenderTargetView[ 0 ] = m_renderTargets[ RENDER_TARGET_DEFERRED_COLOR ]->RenderTargetView;
                m_deferredShaderMap[ 0 ] = m_renderTargets[ RENDER_TARGET_DEFERRED_COLOR ]->ShaderMap;
                m_deferredTextureMap[ 0 ] = m_renderTargets[ RENDER_TARGET_DEFERRED_COLOR ]->TextureMap;

                m_deferredRenderTargetView[ 1 ] = m_renderTargets[ RENDER_TARGET_DEFERRED_NORMAL ]->RenderTargetView;
                m_deferredShaderMap[ 1 ] = m_renderTargets[ RENDER_TARGET_DEFERRED_NORMAL ]->ShaderMap;
                m_deferredTextureMap[ 1 ] = m_renderTargets[ RENDER_TARGET_DEFERRED_NORMAL ]->TextureMap;

                m_deferredRenderTargetView[ 2 ] = m_renderTargets[ RENDER_TARGET_DEFERRED_SPECPOW ]->RenderTargetView;
                m_deferredShaderMap[ 2 ] = m_renderTargets[ RENDER_TARGET_DEFERRED_SPECPOW ]->ShaderMap;
                m_deferredTextureMap[ 2 ] = m_renderTargets[ RENDER_TARGET_DEFERRED_SPECPOW ]->TextureMap;

                m_forwardRenderTargetView[ 0 ] = m_renderTargets[ RENDER_TARGET_SWAPCHAIN ]->RenderTargetView;
                m_forwardRenderTargetView[ 1 ] = m_renderTargets[ RENDER_TARGET_DEFERRED_NORMAL ]->RenderTargetView;
                m_forwardRenderTargetView[ 2 ] = m_renderTargets[ RENDER_TARGET_DEFERRED_SPECPOW ]->RenderTargetView;
            }

            void RenderTargetManager::UnInitializeAllRenderTargets(void)
            {
                for (int x = 0; x < 3; x++)
                {
                    m_deferredTextureMap[ x ] = nullptr;
                    m_deferredRenderTargetView[ x ] = nullptr;
                    m_deferredShaderMap[ x ] = nullptr;
                    m_forwardRenderTargetView[ x ] = nullptr;
                }

                delete[] m_deferredTextureMap;
                delete[] m_deferredRenderTargetView;
                delete[] m_deferredShaderMap;
                delete[] m_forwardRenderTargetView;
            }

            void RenderTargetManager::CreateTargets(void)
            {
                for (int x = 0; x < RENDER_TARGET_COUNT; ++x)
                {
                    m_renderTargets[ x ] = new RenderTarget;
                    m_renderTargets[ x ]->RenderTargetView = nullptr;
                    m_renderTargets[ x ]->ShaderMap = nullptr;
                    m_renderTargets[ x ]->TextureMap = nullptr;
                }
            }

            RenderTarget *RenderTargetManager::GetRenderTarget(const RENDER_TARGETS target)
            {
                UAssert(target < RENDER_TARGET_COUNT, "failed to get render target: out of bounds");
                return m_renderTargets[ target ];
            }

            void RenderTargetManager::SetRenderTarget(const RENDER_TARGETS target, ID3D11DepthStencilView *view)
            {
                if (m_currentTarget == target)
                    return;

                m_currentTarget = target;
                m_deviceContext->OMSetRenderTargets(1, &m_renderTargets[ target ]->RenderTargetView, view);
            }

            void RenderTargetManager::SetDeferredTargets(ID3D11DepthStencilView *view)
            {
                m_deviceContext->OMSetRenderTargets(3, m_deferredRenderTargetView, view);
            }

            void RenderTargetManager::SetForwardTargets(ID3D11DepthStencilView* view)
            {
                m_deviceContext->OMSetRenderTargets(3, m_forwardRenderTargetView, view);
            }

            RENDER_TARGETS RenderTargetManager::CreateRT(const unsigned width, const unsigned height)
            {
                UAssert(m_availableTargets.size() > 0, "Out of render targets!");

                RENDER_TARGETS newRT = m_availableTargets.front();
                m_availableTargets.pop_front();

                CreateRenderTarget(newRT, DXGI_FORMAT_R16G16B16A16_FLOAT, width, height);

                return (RENDER_TARGETS)(newRT - RENDER_TARGET_VIEWPORT_1);
            }

            void RenderTargetManager::DestroyRT(RENDER_TARGETS rt)
            {
                rt = (RENDER_TARGETS)(rt + RENDER_TARGET_VIEWPORT_1);
                RELEASE_RESOURCE(m_renderTargets[ rt ]->RenderTargetView);
                RELEASE_RESOURCE(m_renderTargets[ rt ]->ShaderMap);
                RELEASE_RESOURCE(m_renderTargets[ rt ]->TextureMap);

                m_availableTargets.push_front(rt);
            }

            void RenderTargetManager::ResizeDeferred(const unsigned width, const unsigned height)
            {
                for (int x = RENDER_TARGET_LIGHTMAP; x <= RENDER_TARGET_DEFERRED_SPECPOW; ++x)
                {
                    if (m_renderTargets[ x ] != nullptr)
                    {
                        RELEASE_RESOURCE(m_renderTargets[ x ]->RenderTargetView);
                        RELEASE_RESOURCE(m_renderTargets[ x ]->ShaderMap);
                        RELEASE_RESOURCE(m_renderTargets[ x ]->TextureMap);
                    }
                }

                CreateRenderTarget(RENDER_TARGET_DEFERRED_COLOR, DXGI_FORMAT_R8G8B8A8_UNORM, width, height);
                CreateRenderTarget(RENDER_TARGET_DEFERRED_NORMAL, DXGI_FORMAT_R16G16B16A16_FLOAT, width, height);
                CreateRenderTarget(RENDER_TARGET_DEFERRED_SPECPOW, DXGI_FORMAT_R8G8B8A8_UNORM, width, height);
                CreateRenderTarget(RENDER_TARGET_LIGHTMAP, DXGI_FORMAT_R16G16B16A16_FLOAT, width, height);

                //set this for mapping data
                m_deferredRenderTargetView[ 0 ] = m_renderTargets[ RENDER_TARGET_DEFERRED_COLOR ]->RenderTargetView;
                m_deferredShaderMap[ 0 ] = m_renderTargets[ RENDER_TARGET_DEFERRED_COLOR ]->ShaderMap;
                m_deferredTextureMap[ 0 ] = m_renderTargets[ RENDER_TARGET_DEFERRED_COLOR ]->TextureMap;

                m_deferredRenderTargetView[ 1 ] = m_renderTargets[ RENDER_TARGET_DEFERRED_NORMAL ]->RenderTargetView;
                m_deferredShaderMap[ 1 ] = m_renderTargets[ RENDER_TARGET_DEFERRED_NORMAL ]->ShaderMap;
                m_deferredTextureMap[ 1 ] = m_renderTargets[ RENDER_TARGET_DEFERRED_NORMAL ]->TextureMap;

                m_deferredRenderTargetView[ 2 ] = m_renderTargets[ RENDER_TARGET_DEFERRED_SPECPOW ]->RenderTargetView;
                m_deferredShaderMap[ 2 ] = m_renderTargets[ RENDER_TARGET_DEFERRED_SPECPOW ]->ShaderMap;
                m_deferredTextureMap[ 2 ] = m_renderTargets[ RENDER_TARGET_DEFERRED_SPECPOW ]->TextureMap;

                m_forwardRenderTargetView[ 0 ] = m_renderTargets[ RENDER_TARGET_SWAPCHAIN ]->RenderTargetView;
                m_forwardRenderTargetView[ 1 ] = m_renderTargets[ RENDER_TARGET_DEFERRED_NORMAL ]->RenderTargetView;
                m_forwardRenderTargetView[ 2 ] = m_renderTargets[ RENDER_TARGET_DEFERRED_SPECPOW ]->RenderTargetView;
            }

            void RenderTargetManager::ResizeEngineTargets(const unsigned width, const unsigned height)
            {
                RELEASE_RESOURCE(m_renderTargets[ RENDER_TARGET_DEBUG ]->RenderTargetView);
                RELEASE_RESOURCE(m_renderTargets[ RENDER_TARGET_DEBUG ]->ShaderMap);
                RELEASE_RESOURCE(m_renderTargets[ RENDER_TARGET_DEBUG ]->TextureMap);

                CreateRenderTarget(RENDER_TARGET_DEBUG, DXGI_FORMAT_R8G8B8A8_UNORM, width, height);
            }

            void RenderTargetManager::ResizeUI(const unsigned width, const unsigned height, const RENDER_TARGETS target)
            {
                RELEASE_RESOURCE(m_renderTargets[ target ]->RenderTargetView);
                RELEASE_RESOURCE(m_renderTargets[ target ]->ShaderMap);
                RELEASE_RESOURCE(m_renderTargets[ target ]->TextureMap);

                CreateRenderTarget(RENDER_TARGET_UI, DXGI_FORMAT_R8G8B8A8_UNORM, width, height);
            }

            void RenderTargetManager::Invalidate(void)
            {
                m_currentTarget = RENDER_TARGET_COUNT;
            }


            void RenderTargetManager::CreateRenderTarget(const RENDER_TARGETS target, const DXGI_FORMAT format, const unsigned width, const unsigned height)
            {
                D3D11_TEXTURE2D_DESC textureDesc;
                HRESULT result;
                D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
                D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

                // Initialize the render target texture description.
                ZeroMemory(&textureDesc, sizeof(textureDesc));

                ///////////////////////////////////////////////////////////////
                // Setup the render target texture description.
                textureDesc.Width = width;
                textureDesc.Height = height;
                textureDesc.MipLevels = 1;
                textureDesc.ArraySize = 1;
                textureDesc.Format = format;
                textureDesc.SampleDesc.Count = 1;
                textureDesc.SampleDesc.Quality = 0;
                textureDesc.Usage = D3D11_USAGE_DEFAULT;
                textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
                textureDesc.CPUAccessFlags = 0;
                textureDesc.MiscFlags = 0;

                result = m_device->CreateTexture2D(&textureDesc, nullptr, &m_renderTargets[ target ]->TextureMap);
                UAssert(result == S_OK, "Failed to create texture2D description! (Error '%s')", GetDXErrorMessage(result));
                ///////////////////////////////////////////////////////////////
                // Setup the description of the render target view.
                renderTargetViewDesc.Format = textureDesc.Format;
                renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
                renderTargetViewDesc.Texture2D.MipSlice = 0;

                result = m_device->CreateRenderTargetView(m_renderTargets[ target ]->TextureMap, &renderTargetViewDesc, &m_renderTargets[ target ]->RenderTargetView);
                UAssert(result == S_OK, "Failed to create render target view! (Error '%s')", GetDXErrorMessage(result));

                ///////////////////////////////////////////////////////////////
                // Setup the description of the shader resource view.
                shaderResourceViewDesc.Format = textureDesc.Format;
                shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
                shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
                shaderResourceViewDesc.Texture2D.MipLevels = 1;

                result = m_device->CreateShaderResourceView(m_renderTargets[ target ]->TextureMap, &shaderResourceViewDesc, &m_renderTargets[ target ]->ShaderMap);
                UAssert(result == S_OK, "Failed to create shader resource view! (Error '%s')", GetDXErrorMessage(result));
            }
        }
    }
}
