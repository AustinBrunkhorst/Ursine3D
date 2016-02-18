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
            void DepthStencilManager::Initialize(ID3D11Device *device, ID3D11DeviceContext *devicecontext, const int width, const int height)
            {
                m_device = device;
                m_deviceContext = devicecontext;

                m_depthStencilViewArray.resize(DEPTH_STENCIL_COUNT);
                m_depthStencilTextureArray.resize(DEPTH_STENCIL_COUNT);
                m_depthStencilResourceArray.resize(DEPTH_STENCIL_COUNT);

                //m_currentState = DEPTH_STATE_COUNT  


                /////////////////////////////////////////////////////////////////
                //create main depth stencil
                HRESULT result;
                D3D11_TEXTURE2D_DESC depthBufferDesc;
                ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

                //Set up the description of the depth buffer.
                depthBufferDesc.Width = width;
                depthBufferDesc.Height = height;
                depthBufferDesc.MipLevels = 1;
                depthBufferDesc.ArraySize = 1;
                depthBufferDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
                depthBufferDesc.SampleDesc.Count = 1;
                depthBufferDesc.SampleDesc.Quality = 0;
                depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
                depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D10_BIND_SHADER_RESOURCE;
                depthBufferDesc.CPUAccessFlags = 0;
                depthBufferDesc.MiscFlags = 0;

                //Create the texture for the depth buffer using the filled out description.
                result = m_device->CreateTexture2D(&depthBufferDesc, nullptr, &m_depthStencilTextureArray[ DEPTH_STENCIL_MAIN ]);
                UAssert(result == S_OK, "Depth buffer texture failed! (Error '%s')", GetDXErrorMessage(result));

                D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
                ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

                depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
                depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
                depthStencilViewDesc.Texture2D.MipSlice = 0;

                result = m_device->CreateDepthStencilView(m_depthStencilTextureArray[ DEPTH_STENCIL_MAIN ], &depthStencilViewDesc, &m_depthStencilViewArray[ DEPTH_STENCIL_MAIN ]);
                UAssert(result == S_OK, "Failed to make depth stencil view! (Error '%s')", GetDXErrorMessage(result));

                D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
                srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
                srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
                srvDesc.Texture2D.MipLevels = 1;
                srvDesc.Texture2D.MostDetailedMip = 0;

                result = m_device->CreateShaderResourceView(m_depthStencilTextureArray[ DEPTH_STENCIL_MAIN ], &srvDesc, &m_depthStencilResourceArray[ DEPTH_STENCIL_MAIN ]);
                UAssert(result == S_OK, "Failed to make depth stencil shader resource view! (Error '%s')", GetDXErrorMessage(result));

                /////////////////////////////////////////////////////////////////
                // CREATING SHADOW MAP BUFFER
                ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

                //Set up the description of the depth buffer.
                depthBufferDesc.Width = width;
                depthBufferDesc.Height = height;
                depthBufferDesc.MipLevels = 1;
                depthBufferDesc.ArraySize = 1;
                depthBufferDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
                depthBufferDesc.SampleDesc.Count = 1;
                depthBufferDesc.SampleDesc.Quality = 0;
                depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
                depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D10_BIND_SHADER_RESOURCE;
                depthBufferDesc.CPUAccessFlags = 0;
                depthBufferDesc.MiscFlags = 0;

                //Create the texture for the depth buffer using the filled out description.
                result = m_device->CreateTexture2D(&depthBufferDesc, nullptr, &m_depthStencilTextureArray[ DEPTH_STENCIL_SHADOWMAP ]);
                UAssert(result == S_OK, "Depth buffer texture failed! (Error '%s')", GetDXErrorMessage(result));

                ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

                depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
                depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
                depthStencilViewDesc.Texture2D.MipSlice = 0;

                result = m_device->CreateDepthStencilView(m_depthStencilTextureArray[ DEPTH_STENCIL_SHADOWMAP ], &depthStencilViewDesc, &m_depthStencilViewArray[ DEPTH_STENCIL_SHADOWMAP ]);
                UAssert(result == S_OK, "Failed to make depth stencil view! (Error '%s')", GetDXErrorMessage(result));

                srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
                srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
                srvDesc.Texture2D.MipLevels = 1;
                srvDesc.Texture2D.MostDetailedMip = 0;

                result = m_device->CreateShaderResourceView(m_depthStencilTextureArray[ DEPTH_STENCIL_SHADOWMAP ], &srvDesc, &m_depthStencilResourceArray[ DEPTH_STENCIL_SHADOWMAP ]);
                UAssert(result == S_OK, "Failed to make depth stencil shader resource view! (Error '%s')", GetDXErrorMessage(result));

                /////////////////////////////////////////////////////////////////
                // CREATING OVERDRAW BUFFER
                ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

                //Set up the description of the depth buffer.
                depthBufferDesc.Width = width;
                depthBufferDesc.Height = height;
                depthBufferDesc.MipLevels = 1;
                depthBufferDesc.ArraySize = 1;
                depthBufferDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
                depthBufferDesc.SampleDesc.Count = 1;
                depthBufferDesc.SampleDesc.Quality = 0;
                depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
                depthBufferDesc.BindFlags = D3D10_BIND_DEPTH_STENCIL | D3D10_BIND_SHADER_RESOURCE;
                depthBufferDesc.CPUAccessFlags = 0;
                depthBufferDesc.MiscFlags = 0;

                //Create the texture for the depth buffer using the filled out description.
                result = m_device->CreateTexture2D(&depthBufferDesc, nullptr, &m_depthStencilTextureArray[ DEPTH_STENCIL_OVERDRAW ]);
                UAssert(result == S_OK, "Depth buffer texture failed! (Error '%s')", GetDXErrorMessage(result));

                ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

                depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
                depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
                depthStencilViewDesc.Texture2D.MipSlice = 0;

                result = m_device->CreateDepthStencilView(m_depthStencilTextureArray[ DEPTH_STENCIL_OVERDRAW ], &depthStencilViewDesc, &m_depthStencilViewArray[ DEPTH_STENCIL_OVERDRAW ]);
                UAssert(result == S_OK, "Failed to make depth stencil view! (Error '%s')", GetDXErrorMessage(result));

                srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
                srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
                srvDesc.Texture2D.MipLevels = 1;
                srvDesc.Texture2D.MostDetailedMip = 0;

                result = m_device->CreateShaderResourceView(m_depthStencilTextureArray[ DEPTH_STENCIL_OVERDRAW ], &srvDesc, &m_depthStencilResourceArray[ DEPTH_STENCIL_OVERDRAW ]);
                UAssert(result == S_OK, "Failed to make depth stencil shader resource view! (Error '%s')", GetDXErrorMessage(result));
            }

            void DepthStencilManager::Uninitialize(void)
            {
                for (auto x : m_depthStencilTextureArray)
                {
                    RELEASE_RESOURCE(x);
                }

                for (auto x : m_depthStencilResourceArray)
                {
                    RELEASE_RESOURCE(x);
                }

                for (auto x : m_depthStencilViewArray)
                {
                    RELEASE_RESOURCE(x);
                }

                m_deviceContext = nullptr;
                m_device = nullptr;
            }

            ID3D11DepthStencilView *DepthStencilManager::GetDepthStencilView(const DEPTH_STENCIL_LIST stencil)
            {
                return m_depthStencilViewArray[ stencil ];
            }

            ID3D11ShaderResourceView *DepthStencilManager::GetDepthStencilSRV(const DEPTH_STENCIL_LIST stencil)
            {
                return m_depthStencilResourceArray[ stencil ];
            }

            void DepthStencilManager::Invalidate(void)
            {
                m_currentState = DEPTH_STENCIL_COUNT;
            }

            void DepthStencilManager::Resize(const int width, const int height)
            {
                for (auto x : m_depthStencilTextureArray)
                {
                    RELEASE_RESOURCE(x);
                }

                for (auto x : m_depthStencilResourceArray)
                {
                    RELEASE_RESOURCE(x);
                }

                for (auto x : m_depthStencilViewArray)
                {
                    RELEASE_RESOURCE(x);
                }

                Initialize(m_device, m_deviceContext, width, height);
            }
        }
    }
}
