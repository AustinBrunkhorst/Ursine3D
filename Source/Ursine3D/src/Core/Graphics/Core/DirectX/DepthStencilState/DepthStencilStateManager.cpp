/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** DepthStencilStateManager.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "DepthStencilStateManager.h"
#include <d3d11.h>
#include "GfxDefines.h"
#include "DXErrorHandling.h"


namespace ursine
{
    namespace graphics
    {
        namespace DXCore
        {
            void DepthStencilStateManager::Initialize(ID3D11Device *device, ID3D11DeviceContext *devicecontext)
            {
                m_device = device;
                m_deviceContext = devicecontext;

                m_depthStateArray.resize(DEPTH_STATE_COUNT);

                m_currentState = DEPTH_STATE_COUNT;

                /////////////////////////////////////////////////////////////////
                // DEPTH CHECKING ///////////////////////////////////////////////
                HRESULT result;
                D3D11_DEPTH_STENCIL_DESC depthCheckStencilDesc;
                ZeroMemory(&depthCheckStencilDesc, sizeof(depthCheckStencilDesc));

                //Set up the description of the stencil state.
                depthCheckStencilDesc.DepthEnable = true;
                depthCheckStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
                depthCheckStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

                depthCheckStencilDesc.StencilEnable = false;
                depthCheckStencilDesc.StencilReadMask = 0xFF;
                depthCheckStencilDesc.StencilWriteMask = 0xFF;

                //Stencil operations if pixel is front-facing.
                depthCheckStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
                depthCheckStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
                depthCheckStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
                depthCheckStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

                //Stencil operations if pixel is back-facing.
                depthCheckStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
                depthCheckStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
                depthCheckStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
                depthCheckStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

                //Create the depth stencil state.
                result = m_device->CreateDepthStencilState(&depthCheckStencilDesc, &m_depthStateArray[ DEPTH_STATE_DEPTH_NOSTENCIL ]);
                UAssert(result == S_OK, "Failed to create depth stencil! (Error '%s')", GetDXErrorMessage(result));

                /////////////////////////////////////////////////////////////////
                // NO DEPTH CHECKING ////////////////////////////////////////////
                ZeroMemory(&depthCheckStencilDesc, sizeof(depthCheckStencilDesc));

                //Set up the description of the stencil state.
                depthCheckStencilDesc.DepthEnable = false;
                depthCheckStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
                depthCheckStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

                depthCheckStencilDesc.StencilEnable = false;
                depthCheckStencilDesc.StencilReadMask = 0xFF;
                depthCheckStencilDesc.StencilWriteMask = 0xFF;

                //Stencil operations if pixel is front-facing.
                depthCheckStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
                depthCheckStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
                depthCheckStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
                depthCheckStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

                //Stencil operations if pixel is back-facing.
                depthCheckStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
                depthCheckStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
                depthCheckStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
                depthCheckStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

                //Create the depth stencil state.
                result = m_device->CreateDepthStencilState(&depthCheckStencilDesc, &m_depthStateArray[ DEPTH_STATE_NODEPTH_NOSTENCIL ]);
                UAssert(result == S_OK, "Failed to create depth stencil! (Error '%s')", GetDXErrorMessage(result));

                /////////////////////////////////////////////////////////////////
                // DEPTH CHECK AND STENCIL
                ZeroMemory(&depthCheckStencilDesc, sizeof(depthCheckStencilDesc));

                //Set up the description of the stencil state.
                depthCheckStencilDesc.DepthEnable = true;
                depthCheckStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
                depthCheckStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

                depthCheckStencilDesc.StencilEnable = true;
                depthCheckStencilDesc.StencilReadMask = 0xFF;
                depthCheckStencilDesc.StencilWriteMask = 0xFF;

                //Stencil operations if pixel is front-facing.
                depthCheckStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
                depthCheckStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
                depthCheckStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
                depthCheckStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

                //Stencil operations if pixel is back-facing.
                depthCheckStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
                depthCheckStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
                depthCheckStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
                depthCheckStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

                //Create the depth stencil state.
                result = m_device->CreateDepthStencilState(&depthCheckStencilDesc, &m_depthStateArray[ DEPTH_STATE_DEPTH_STENCIL ]);
                UAssert(result == S_OK, "Failed to create depth stencil! (Error '%s')", GetDXErrorMessage(result));

                /////////////////////////////////////////////////////////////////
                // NO DEPTH CHECKING, STENCIL CHECK
                ZeroMemory(&depthCheckStencilDesc, sizeof(depthCheckStencilDesc));

                //Set up the description of the stencil state.
                depthCheckStencilDesc.DepthEnable = false;
                depthCheckStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
                depthCheckStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

                depthCheckStencilDesc.StencilEnable = true;
                depthCheckStencilDesc.StencilReadMask = 0xFF;
                depthCheckStencilDesc.StencilWriteMask = 0xFF;

                //Stencil operations if pixel is front-facing.
                depthCheckStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
                depthCheckStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
                depthCheckStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
                depthCheckStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;

                //Stencil operations if pixel is back-facing.
                depthCheckStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
                depthCheckStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
                depthCheckStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
                depthCheckStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;

                //Create the depth stencil state.
                result = m_device->CreateDepthStencilState(&depthCheckStencilDesc, &m_depthStateArray[ DEPTH_STATE_NODEPTH_STENCIL ]);
                UAssert(result == S_OK, "Failed to create depth stencil! (Error '%s')", GetDXErrorMessage(result));

                /////////////////////////////////////////////////////////////////
                // point light sutff
                ZeroMemory(&depthCheckStencilDesc, sizeof(depthCheckStencilDesc));

                //Set up the description of the stencil state.
                depthCheckStencilDesc.DepthEnable = true;
                depthCheckStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
                depthCheckStencilDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;

                //Create the depth stencil state.
                result = m_device->CreateDepthStencilState(&depthCheckStencilDesc, &m_depthStateArray[ DEPTH_STATE_POINTLIGHT ]);
                UAssert(result == S_OK, "Failed to create depth stencil! (Error '%s')", GetDXErrorMessage(result));

                /////////////////////////////////////////////////////////////////
                // ALWAYS PASS DEPTH AND STENCIL
                ZeroMemory(&depthCheckStencilDesc, sizeof(depthCheckStencilDesc));

                //Set up the description of the stencil state.
                depthCheckStencilDesc.DepthEnable = true;
                depthCheckStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
                depthCheckStencilDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;

                depthCheckStencilDesc.StencilEnable = true;
                depthCheckStencilDesc.StencilReadMask = 0xFF;
                depthCheckStencilDesc.StencilWriteMask = 0xFF;

                //Stencil operations if pixel is front-facing.
                depthCheckStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
                depthCheckStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
                depthCheckStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
                depthCheckStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

                //Stencil operations if pixel is back-facing.
                depthCheckStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
                depthCheckStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
                depthCheckStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
                depthCheckStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_LESS;

                //Create the depth stencil state.
                result = m_device->CreateDepthStencilState(&depthCheckStencilDesc, &m_depthStateArray[ DEPTH_STATE_PASSDEPTH_WRITESTENCIL ]);
                UAssert(result == S_OK, "Failed to create depth stencil! (Error '%s')", GetDXErrorMessage(result));

                /////////////////////////////////////////////////////////////////
                // DEPTH CHECK NO WRITE /////////////////////////////////////////
                ZeroMemory(&depthCheckStencilDesc, sizeof(depthCheckStencilDesc));

                //Set up the description of the stencil state.
                depthCheckStencilDesc.DepthEnable = true;
                depthCheckStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
                depthCheckStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

                depthCheckStencilDesc.StencilEnable = false;
                depthCheckStencilDesc.StencilReadMask = 0xFF;
                depthCheckStencilDesc.StencilWriteMask = 0xFF;

                //Stencil operations if pixel is front-facing.
                depthCheckStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
                depthCheckStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
                depthCheckStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
                depthCheckStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

                //Stencil operations if pixel is back-facing.
                depthCheckStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
                depthCheckStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
                depthCheckStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
                depthCheckStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

                //Create the depth stencil state.
                result = m_device->CreateDepthStencilState(&depthCheckStencilDesc, &m_depthStateArray[ DEPTH_STATE_CHECKDEPTH_NOWRITE_NOSTENCIL ]);
                UAssert(result == S_OK, "Failed to create depth stencil! (Error '%s')", GetDXErrorMessage(result));

                /////////////////////////////////////////////////////////////////
                // WRITE DEPTH FOR CLEARNING
                ZeroMemory( &depthCheckStencilDesc, sizeof( depthCheckStencilDesc ) );

                //Set up the description of the stencil state.
                depthCheckStencilDesc.DepthEnable = true;
                depthCheckStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
                depthCheckStencilDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;

                depthCheckStencilDesc.StencilEnable = false;

                //Create the depth stencil state.
                result = m_device->CreateDepthStencilState( &depthCheckStencilDesc, &m_depthStateArray[ DEPTH_STATE_PASSDEPTH_NOSTENCIL ] );
                UAssert( result == S_OK, "Failed to create depth stencil! (Error '%s')", GetDXErrorMessage( result ) );
            }

            void DepthStencilStateManager::Uninitialize(void)
            {
                for (unsigned x = 0; x < m_depthStateArray.size(); ++x)
                {
                    RELEASE_RESOURCE(m_depthStateArray[ x ]);
                }

                m_device = nullptr;
                m_deviceContext = nullptr;
            }

            ID3D11DepthStencilState *DepthStencilStateManager::GetStencilState(const DEPTH_STATES state)
            {
                return m_depthStateArray[ state ];
            }

            void DepthStencilStateManager::SetDepthState(const DEPTH_STATES state)
            {
                if (m_currentState == state)
                    return;

                m_currentState = state;

                m_deviceContext->OMSetDepthStencilState(m_depthStateArray[ state ], 1);
            }

            void DepthStencilStateManager::Invalidate(void)
            {
                m_currentState = DEPTH_STATE_COUNT;
            }
        }
    }
}
