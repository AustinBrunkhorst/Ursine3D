#include "UrsinePrecompiled.h"
#include "RasterStateManager.h"

namespace ursine
{
    namespace DXCore
    {
        void RasterStateManager::Initialize(ID3D11Device *device, ID3D11DeviceContext *devicecontext)
        {
            m_device = device;
            m_deviceContext = devicecontext;

            m_rasterStateArray.resize( RASTER_STATE_COUNT );

            m_currentState = RASTER_STATE_COUNT;

            HRESULT result;
            D3D11_RASTERIZER_DESC rasterDesc;
            ZeroMemory( &rasterDesc, sizeof( rasterDesc ) );

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
            result = m_device->CreateRasterizerState( &rasterDesc, &m_rasterStateArray[ RASTER_STATE_SOLID_FRONTCULL ] );
            UAssert( result == S_OK, "Failed to make rasterizer state! (Error '%i')", result );

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
            result = m_device->CreateRasterizerState( &rasterDesc, &m_rasterStateArray[ RASTER_STATE_SOLID_BACKCULL ] );
            UAssert( result == S_OK, "Failed to make rasterizer state! (Error '%i')", result );

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
            result = m_device->CreateRasterizerState( &rasterDesc, &m_rasterStateArray[ RASTER_STATE_SOLID_NOCULL ] );
            UAssert( result == S_OK, "Failed to make rasterizer state! (Error '%i')", result );


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
            result = m_device->CreateRasterizerState( &rasterDesc, &m_rasterStateArray[ RASTER_STATE_WIREFRAME_BACKCULL ] );
            UAssert( result == S_OK, "Failed to make rasterizer state! (Error '%i')", result );

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
            result = m_device->CreateRasterizerState( &rasterDesc, &m_rasterStateArray[ RASTER_STATE_WIREFRAME_FRONTCULL ] );
            UAssert( result == S_OK, "Failed to make rasterizer state! (Error '%i')", result );

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
            result = m_device->CreateRasterizerState( &rasterDesc, &m_rasterStateArray[ RASTER_STATE_WIREFRAME_NOCULL ] );
            UAssert( result == S_OK, "Failed to make rasterizer state! (Error '%i')", result );

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
            result = m_device->CreateRasterizerState( &rasterDesc, &m_rasterStateArray[ RASTER_STATE_LINE_RENDERING ] );
            UAssert( result == S_OK, "Failed to make rasterizer state! (Error '%i')", result );

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
            result = m_device->CreateRasterizerState( &rasterDesc, &m_rasterStateArray[ RASTER_STATE_UI ] );
            UAssert( result == S_OK, "Failed to make rasterizer state! (Error '%i')", result );
        }

        void RasterStateManager::Uninitialize()
        {
            for (unsigned x = 0; x < m_rasterStateArray.size( ); ++x)
            {
                RELEASE_RESOURCE( m_rasterStateArray[ x ] );
            }

            m_device = NULL;
            m_deviceContext = NULL;
        }

        ID3D11RasterizerState *RasterStateManager::GetRasterState(RASTER_STATES state)
        {
            return m_rasterStateArray[ state ];
        }

        void RasterStateManager::SetRasterState(RASTER_STATES state)
        {
            if (m_currentState == state)
                return;

            m_currentState = state;

            m_deviceContext->RSSetState( m_rasterStateArray[ state ] );
        }

        void RasterStateManager::Invalidate()
        {
            m_currentState = RASTER_STATE_COUNT;
        }
    }
}
