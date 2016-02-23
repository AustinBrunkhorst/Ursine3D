/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** BlendStateManager.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"
#include "BlendStateManager.h"
#include <d3d11.h>
#include "DXErrorHandling.h"

namespace ursine
{
    namespace graphics
    {
        namespace DXCore
        {
            void BlendStateManager::Initialize(ID3D11Device *device, ID3D11DeviceContext *devicecontext)
            {
                m_device = device;
                m_deviceContext = devicecontext;

                BlendStateArray_.resize(BLEND_STATE_COUNT);

                m_currentState = BLEND_STATE_COUNT;

                ///////////////////////////////////////////////////////////////
                // NORMAL BLEND DESCRIPTION ///////////////////////////////////
                HRESULT result;
                D3D11_BLEND_DESC blendDesc;
                ZeroMemory(&blendDesc, sizeof(blendDesc));
                blendDesc.AlphaToCoverageEnable = false;
                blendDesc.IndependentBlendEnable = false;
                blendDesc.RenderTarget[ 0 ].BlendEnable = TRUE;

                blendDesc.RenderTarget[ 0 ].BlendOp = D3D11_BLEND_OP_ADD;
                blendDesc.RenderTarget[ 0 ].SrcBlend = D3D11_BLEND_SRC_ALPHA;
                blendDesc.RenderTarget[ 0 ].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

                blendDesc.RenderTarget[ 0 ].BlendOpAlpha = D3D11_BLEND_OP_ADD;
                blendDesc.RenderTarget[ 0 ].SrcBlendAlpha = D3D11_BLEND_ZERO;
                blendDesc.RenderTarget[ 0 ].DestBlendAlpha = D3D11_BLEND_ZERO;

                blendDesc.RenderTarget[ 0 ].RenderTargetWriteMask = 0x0F;

                //create blend description
                m_device->CreateBlendState(&blendDesc, &BlendStateArray_[ BLEND_STATE_DEFAULT ]);

                //set blend state
                float blendFactor[ 4 ] = { 1.f, 1.f, 1.f, 1.f };
                m_deviceContext->OMSetBlendState(BlendStateArray_[ 0 ], blendFactor, 0xffffffff);

                ///////////////////////////////////////////////////////////////
                // ADDITIVE BLEND DESCRIPTION /////////////////////////////////
                ZeroMemory(&blendDesc, sizeof(blendDesc));
                blendDesc.AlphaToCoverageEnable = false;
                blendDesc.IndependentBlendEnable = true;
                blendDesc.RenderTarget[ 0 ].BlendEnable = TRUE;

                blendDesc.RenderTarget[ 0 ].BlendOp = D3D11_BLEND_OP_ADD;
                blendDesc.RenderTarget[ 0 ].SrcBlend = D3D11_BLEND_ONE;
                blendDesc.RenderTarget[ 0 ].DestBlend = D3D11_BLEND_ONE;

                blendDesc.RenderTarget[ 0 ].BlendOpAlpha = D3D11_BLEND_OP_ADD;
                blendDesc.RenderTarget[ 0 ].SrcBlendAlpha = D3D11_BLEND_ONE;
                blendDesc.RenderTarget[ 0 ].DestBlendAlpha = D3D11_BLEND_ONE;

                blendDesc.RenderTarget[ 0 ].RenderTargetWriteMask = 0x0F;

                //create blend description
                result = m_device->CreateBlendState(&blendDesc, &BlendStateArray_[ BLEND_STATE_ADDITIVE ]);
                UAssert(result == S_OK, "Failed to create blend state! (Error: '%s')", GetDXErrorMessage(result));

                ///////////////////////////////////////////////////////////////
                // NO BLENDING    DESCRIPTION /////////////////////////////////
                ZeroMemory(&blendDesc, sizeof(blendDesc));
                blendDesc.AlphaToCoverageEnable = false;
                blendDesc.IndependentBlendEnable = true;
                blendDesc.RenderTarget[ 0 ].BlendEnable = TRUE;

                blendDesc.RenderTarget[ 0 ].BlendOp = D3D11_BLEND_OP_ADD;
                blendDesc.RenderTarget[ 0 ].SrcBlend = D3D11_BLEND_ONE;
                blendDesc.RenderTarget[ 0 ].DestBlend = D3D11_BLEND_ZERO;

                blendDesc.RenderTarget[ 0 ].BlendOpAlpha = D3D11_BLEND_OP_ADD;
                blendDesc.RenderTarget[ 0 ].SrcBlendAlpha = D3D11_BLEND_ONE;
                blendDesc.RenderTarget[ 0 ].DestBlendAlpha = D3D11_BLEND_ZERO;

                blendDesc.RenderTarget[ 0 ].RenderTargetWriteMask = 0x0F;

                //create blend description
                result = m_device->CreateBlendState(&blendDesc, &BlendStateArray_[ BLEND_STATE_NONE ]);
                UAssert(result == S_OK, "Failed to create blend state! (Error: '%s')", GetDXErrorMessage(result));
            }

            void BlendStateManager::Uninitialize(void)
            {
                for (unsigned x = 0; x < BlendStateArray_.size(); ++x)
                {
                    if (BlendStateArray_[ x ] != nullptr)
                    {
                        BlendStateArray_[ x ]->Release();
                        BlendStateArray_[ x ] = nullptr;
                    }
                }

                m_device = nullptr;
                m_deviceContext = nullptr;
            }

            ID3D11BlendState *BlendStateManager::GetBlendState(const BLEND_STATES state)
            {
                return BlendStateArray_[ state ];
            }

            void BlendStateManager::SetBlendState(const BLEND_STATES state)
            {
                if(state == BLEND_STATE_COUNT )
                {
                    m_currentState = state;
                    float blendFactor[ 4 ] = { 1.f, 1.f, 1.f, 1.f };
                    m_deviceContext->OMSetBlendState(nullptr, blendFactor, 0xffffffff);
                    return;
                }

                if ( m_currentState == state )
                    return;

                m_currentState = state;

                m_deviceContext->OMSetBlendState(BlendStateArray_[ state ], 0, 0xffffffff);
            }

            void BlendStateManager::Invalidate(void)
            {
                m_currentState = BLEND_STATE_COUNT;
            }
        }
    }
}
