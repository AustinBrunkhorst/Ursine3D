/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** RenderTargetManager.h
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
File Name:      RenderTargetManager.h
Module:         Graphics
Purpose:        backend manager for render targets
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/

#pragma once


#include <list>
#include "RenderTargetList.h"
#include "D3D11Forward.h"
#include <dxgiformat.h>
#include "GfxInfo.h"

namespace ursine
{
    namespace graphics
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
                void Initialize(ID3D11Device *device, ID3D11DeviceContext *devicecontext, GfxInfo *gfxInfo);
                void Uninitialize(void);
                void InitializeAllRenderTargets(const unsigned width, const unsigned height);
                void UnInitializeAllRenderTargets(void);
                void CreateTargets(void);

                RenderTarget *GetRenderTarget(const RENDER_TARGETS target);
                void SetRenderTarget(const RENDER_TARGETS target, ID3D11DepthStencilView *view);

                void SetDeferredTargets(ID3D11DepthStencilView *view);
                void SetForwardTargets(ID3D11DepthStencilView *view);

                RENDER_TARGETS CreateRT(const unsigned width, const unsigned height);
                void DestroyRT(RENDER_TARGETS rt);

                void ResizeDeferred(const unsigned width, const unsigned height);
                void ResizeEngineTargets(const unsigned width, const unsigned height);
                void ResizeUI(const unsigned width, const unsigned height, const RENDER_TARGETS target = RENDER_TARGET_UI);

                void Invalidate(void);
            private:
                void CreateRenderTarget(const RENDER_TARGETS target, const  DXGI_FORMAT format, const unsigned width, const unsigned height);

                ID3D11Device *m_device;
                ID3D11DeviceContext *m_deviceContext;

                GfxInfo *m_gfxInfo;

                //the targets
                std::vector<RenderTarget*> m_renderTargets;
                std::list<RENDER_TARGETS> m_availableTargets;

                //deferred rendering
                ID3D11RenderTargetView **m_deferredRenderTargetView;
                ID3D11ShaderResourceView **m_deferredShaderMap;
                ID3D11Texture2D **m_deferredTextureMap;

                // forward rendering
                ID3D11RenderTargetView **m_forwardRenderTargetView;

                //what target am I on?
                RENDER_TARGETS m_currentTarget;
            };
        }
    }
}
