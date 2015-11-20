/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** RasterStateManager.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"
#include "RasterStateManager.h"
#include <d3d11.h>
#include "DXErrorHandling.h"


namespace ursine
{
    namespace graphics
    {
        namespace DXCore
        {
            void RasterStateManager::Initialize(ID3D11Device *device, ID3D11DeviceContext *devicecontext)
            {
                m_device = device;
                m_deviceContext = devicecontext;

                m_rasterStateArray.resize(RASTER_STATE_COUNT);

                m_currentState = RASTER_STATE_COUNT;

                HRESULT result;
                D3D11_RASTERIZER_DESC rasterDesc;
                ZeroMemory(&rasterDesc, sizeof(rasterDesc));

                ///////////////////////////////////////////////////////////////
                // FRONT FACE
                //Setup the raster description which will determine how and what polygons will be drawn.
                rasterDesc.AntialiasedLineEnable = true;
                rasterDesc.CullMode = D3D11_CULL_FRONT;
                rasterDesc.DepthBias = 0;
                rasterDesc.DepthBiasClamp = 0.0f;
                rasterDesc.DepthClipEnable = false;
                rasterDesc.FillMode = D3D11_FILL_SOLID;
                rasterDesc.FrontCounterClockwise = false;
                rasterDesc.MultisampleEnable = true;
                rasterDesc.ScissorEnable = false;
                rasterDesc.SlopeScaledDepthBias = 0.0f;

                //Create the rasterizer state from the description we just filled out.
                result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterStateArray[ RASTER_STATE_SOLID_FRONTCULL ]);
                UAssert(result == S_OK, "Failed to make rasterizer state! (Error '%s')", GetDXErrorMessage(result));

                ///////////////////////////////////////////////////////////////
                // BACK FACE
                //Setup the raster description which will determine how and what polygons will be drawn.
                rasterDesc.AntialiasedLineEnable = true;
                rasterDesc.CullMode = D3D11_CULL_BACK;
                rasterDesc.DepthBias = 0;
                rasterDesc.DepthBiasClamp = 0.0f;
                rasterDesc.DepthClipEnable = false;
                rasterDesc.FillMode = D3D11_FILL_SOLID;
                rasterDesc.FrontCounterClockwise = false;
                rasterDesc.MultisampleEnable = true;
                rasterDesc.ScissorEnable = false;
                rasterDesc.SlopeScaledDepthBias = 0.0f;

                //Create the rasterizer state from the description we just filled out.
                result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterStateArray[ RASTER_STATE_SOLID_BACKCULL ]);
                UAssert(result == S_OK, "Failed to make rasterizer state! (Error '%s')", GetDXErrorMessage(result));

                ///////////////////////////////////////////////////////////////
                // NO CULLING
                //Setup the raster description which will determine how and what polygons will be drawn.
                rasterDesc.AntialiasedLineEnable = true;
                rasterDesc.CullMode = D3D11_CULL_NONE;
                rasterDesc.DepthBias = 0;
                rasterDesc.DepthBiasClamp = 0.0f;
                rasterDesc.DepthClipEnable = false;
                rasterDesc.FillMode = D3D11_FILL_SOLID;
                rasterDesc.FrontCounterClockwise = false;
                rasterDesc.MultisampleEnable = true;
                rasterDesc.ScissorEnable = false;
                rasterDesc.SlopeScaledDepthBias = 0.0f;

                //Create the rasterizer state from the description we just filled out.
                result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterStateArray[ RASTER_STATE_SOLID_NOCULL ]);
                UAssert(result == S_OK, "Failed to make rasterizer state! (Error '%s')", GetDXErrorMessage(result));


                ///////////////////////////////////////////////////////////////
                // WIREFRAME RENDER, BACKFACE CULL
                //Setup the raster description which will determine how and what polygons will be drawn.
                rasterDesc.AntialiasedLineEnable = true;
                rasterDesc.CullMode = D3D11_CULL_BACK;
                rasterDesc.DepthBias = 0;
                rasterDesc.DepthBiasClamp = 0.0f;
                rasterDesc.DepthClipEnable = false;
                rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
                rasterDesc.FrontCounterClockwise = false;
                rasterDesc.MultisampleEnable = true;
                rasterDesc.ScissorEnable = false;
                rasterDesc.SlopeScaledDepthBias = 0.0f;

                //Create the rasterizer state from the description we just filled out.
                result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterStateArray[ RASTER_STATE_WIREFRAME_BACKCULL ]);
                UAssert(result == S_OK, "Failed to make rasterizer state! (Error '%s')", GetDXErrorMessage(result));

                ///////////////////////////////////////////////////////////////
                // WIREFRAME RENDER, FRONTFACE CULL
                //Setup the raster description which will determine how and what polygons will be drawn.
                rasterDesc.AntialiasedLineEnable = true;
                rasterDesc.CullMode = D3D11_CULL_FRONT;
                rasterDesc.DepthBias = 0;
                rasterDesc.DepthBiasClamp = 0.0f;
                rasterDesc.DepthClipEnable = false;
                rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
                rasterDesc.FrontCounterClockwise = false;
                rasterDesc.MultisampleEnable = true;
                rasterDesc.ScissorEnable = false;
                rasterDesc.SlopeScaledDepthBias = 0.0f;

                //Create the rasterizer state from the description we just filled out.
                result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterStateArray[ RASTER_STATE_WIREFRAME_FRONTCULL ]);
                UAssert(result == S_OK, "Failed to make rasterizer state! (Error '%s')", GetDXErrorMessage(result));

                ///////////////////////////////////////////////////////////////
                // WIREFRAME RENDER, NO CULL
                //Setup the raster description which will determine how and what polygons will be drawn.
                rasterDesc.AntialiasedLineEnable = true;
                rasterDesc.CullMode = D3D11_CULL_NONE;
                rasterDesc.DepthBias = 0;
                rasterDesc.DepthBiasClamp = 0.0f;
                rasterDesc.DepthClipEnable = false;
                rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
                rasterDesc.FrontCounterClockwise = false;
                rasterDesc.MultisampleEnable = true;
                rasterDesc.ScissorEnable = false;
                rasterDesc.SlopeScaledDepthBias = 0.0f;

                //Create the rasterizer state from the description we just filled out.
                result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterStateArray[ RASTER_STATE_WIREFRAME_NOCULL ]);
                UAssert(result == S_OK, "Failed to make rasterizer state! (Error '%s')", GetDXErrorMessage(result));

                ///////////////////////////////////////////////////////////////
                // LINE RENDERING
                //Setup the raster description which will determine how and what polygons will be drawn.
                rasterDesc.AntialiasedLineEnable = false;
                rasterDesc.CullMode = D3D11_CULL_NONE;
                rasterDesc.DepthBias = 0;
                rasterDesc.DepthBiasClamp = 0.0f;
                rasterDesc.DepthClipEnable = false;
                rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
                rasterDesc.FrontCounterClockwise = false;
                rasterDesc.MultisampleEnable = false;
                rasterDesc.ScissorEnable = false;
                rasterDesc.SlopeScaledDepthBias = 0.0f;

                //Create the rasterizer state from the description we just filled out.
                result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterStateArray[ RASTER_STATE_LINE_RENDERING ]);
                UAssert(result == S_OK, "Failed to make rasterizer state! (Error '%s')", GetDXErrorMessage(result));

                ///////////////////////////////////////////////////////////////
                // UI RENDERING   
                //Setup the raster description which will determine how and what polygons will be drawn.
                rasterDesc.AntialiasedLineEnable = false;
                rasterDesc.CullMode = D3D11_CULL_NONE;
                rasterDesc.DepthBias = 0;
                rasterDesc.DepthBiasClamp = 0.0f;
                rasterDesc.DepthClipEnable = false;
                rasterDesc.FillMode = D3D11_FILL_SOLID;
                rasterDesc.FrontCounterClockwise = false;
                rasterDesc.MultisampleEnable = false;
                rasterDesc.ScissorEnable = false;
                rasterDesc.SlopeScaledDepthBias = 0.0f;

                //Create the rasterizer state from the description we just filled out.
                result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterStateArray[ RASTER_STATE_UI ]);
                UAssert(result == S_OK, "Failed to make rasterizer state! (Error '%s')", GetDXErrorMessage(result));
            }

            void RasterStateManager::Uninitialize(void)
            {
                for (unsigned x = 0; x < m_rasterStateArray.size(); ++x)
                {
                    RELEASE_RESOURCE(m_rasterStateArray[ x ]);
                }

                m_device = nullptr;
                m_deviceContext = nullptr;
            }

            ID3D11RasterizerState *RasterStateManager::GetRasterState(RASTER_STATES state)
            {
                return m_rasterStateArray[ state ];
            }

            void RasterStateManager::SetRasterState(const RASTER_STATES state)
            {
                if (m_currentState == state)
                    return;

                m_currentState = state;

                m_deviceContext->RSSetState(m_rasterStateArray[ state ]);
            }

            void RasterStateManager::Invalidate(void)
            {
                m_currentState = RASTER_STATE_COUNT;
            }
        }
    }
}
