/* Start Header ---------------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
=============================================================================*/
/*!
File Name:      RenderTargetManager.h
Module:         Graphics
Purpose:        backend manager for render targets
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/

#pragma once
#include <d3d11.h>
#include <list>
#include "RenderTargetList.h"
#include "GraphicsDefines.h"

namespace ursine
{
    namespace DXCore
    {
        //a single render target
        struct RenderTarget
        {
            ID3D11RenderTargetView *RenderTargetView;
            ID3D11ShaderResourceView *ShaderMap;
            ID3D11Texture2D *TextureMap;
        };

        //manages
        class RenderTargetManager
        {
        public:
            void Initialize(ID3D11Device *device, ID3D11DeviceContext *devicecontext);
            void Uninitialize();
            void InitializeAllRenderTargets(unsigned width, unsigned height);
            void UnInitializeAllRenderTargets();
            void CreateTargets();
            RenderTarget *GetRenderTarget(RENDER_TARGETS target);
            void SetRenderTarget(RENDER_TARGETS target, ID3D11DepthStencilView *view);

            void SetDeferredTargets(ID3D11DepthStencilView *view);

            RENDER_TARGETS CreateRT(unsigned width, unsigned height);
            void DestroyRT(RENDER_TARGETS rt);

            void ResizeDeferred(unsigned width, unsigned height);

            void ResizeEngineTargets(unsigned width, unsigned height);

            void ResizeUI(unsigned width, unsigned height, RENDER_TARGETS target = RENDER_TARGET_UI);

            void Invalidate()
            {
                m_currentTarget = RENDER_TARGET_COUNT;
            }

        private:
            void CreateRenderTarget(RENDER_TARGETS target, DXGI_FORMAT format, unsigned width, unsigned height);

            ID3D11Device *m_device;
            ID3D11DeviceContext *m_deviceContext;

            //the targets
            std::vector<RenderTarget*> m_renderTargets;
            std::list<RENDER_TARGETS> m_availableTargets;

            //deferred rendering
            ID3D11RenderTargetView **m_deferredRenderTargetView;
            ID3D11ShaderResourceView **m_deferredShaderMap;
            ID3D11Texture2D **m_deferredTextureMap;

            //what target am I on?
            RENDER_TARGETS m_currentTarget;
        };
    }
}
