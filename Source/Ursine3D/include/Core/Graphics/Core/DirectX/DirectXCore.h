/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** DirectXCore.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

/* Start Header ---------------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
=============================================================================*/
/*!
File Name:      DirectXCore.h
Module:         Graphics
Purpose:        Core dx class for managing all dx resources
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/
#pragma once

#include "GfxInfo.h"
#include "BlendStateManager.h"
#include "DepthStencilStateManager.h"
#include "RenderTargetManager.h"
#include "RasterStateManager.h"
#include "DepthStencilManager.h"
#include "D3D11Forward.h"
#include <D3D11SDKLayers.h>
#include <d3d11_1.h>

namespace ursine
{
    namespace graphics
    {
        namespace DXCore
        {
            enum PRIMITIVE_TOPOLOGY : unsigned
            {
                TOPOLOGY_LINE_LIST = D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
                TOPOLOGY_LINE_STRIP = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,
                TOPOLOGY_TRIANGLE_LIST = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
                TOPOLOGY_TRIANGLE_STRIP = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
                TOPOLOGY_POINT_LIST = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,

                TOPOLOGY_COUNT
            };

            class DirectXCore
            {
            public:
                //initialize everything
                void Initialize(const unsigned width, const unsigned height, HWND hWindow, GfxInfo *gfxInfo, bool fullscreen, bool debug);
                void Uninitialize(void);

                //methods
                //clears buffers
                void ClearDeferredBuffers(void);
                void ClearDepthBuffers(void);
                void ClearSwapchain(void);
                void ClearDebugBuffer(void);
                void ClearTargetBuffers(void);

                //swaps the swapchain
                void SwapChainBuffer(void);

                //get the device
                ID3D11Device *GetDevice(void);

                //get the devcon
                ID3D11DeviceContext *GetDeviceContext(void);

                //set blend
                void SetBlendState(const BLEND_STATES bt);
                //set depth stencil
                void SetDepthState(const DEPTH_STATES dt);
                //set target
                void SetRenderTarget(const RENDER_TARGETS rt, DEPTH_STENCIL_LIST depthTarget = DEPTH_STENCIL_MAIN);

                //set raster state
                void SetRasterState(const RASTER_STATES state);

                //get blend
                BlendStateManager *GetBlendMgr(void);
                //get depth stencil
                DepthStencilManager *GetDepthMgr(void);
                //get target
                RenderTargetManager *GetRenderTargetMgr(void);
                //get stencil manager
                DepthStencilStateManager *GetDepthStencilMgr(void);

                void ResizeDX(const int width, const int height);

                void CheckSize(void);

                void Invalidate(void);

                void SetFullscreenState( const bool state );

                // debug stuff
                void StartDebugEvent(std::string eventStr);

                void EndDebugEvent();

                //private methods
            private:
                void backendResizeDX(const int width, const int height);

            private:
                bool m_debug;
                int m_newWidth;
                int m_newHeight;
                bool m_shouldResize;

                ID3D11Device *m_device;
                ID3D11DeviceContext *m_deviceContext;
                IDXGISwapChain *m_swapChain;
                ID3D11Debug *m_debugInterface;
                ID3D11InfoQueue *m_infoQueue;
                ID3DUserDefinedAnnotation *m_userAnnotation;

                BlendStateManager *m_blendManager;
                DepthStencilStateManager *m_depthStateManager;
                DepthStencilManager *m_depthStencilManager;
                RenderTargetManager *m_targetManager;
                RasterStateManager *m_rasterStateManager;
            };
        }
    }
}
