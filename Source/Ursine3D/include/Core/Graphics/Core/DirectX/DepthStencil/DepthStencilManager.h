/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** DepthStencilManager.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "D3D11Forward.h"
#include <vector>
#include "DepthStencilList.h"
#include "GfxInfo.h"

enum DXGI_FORMAT;

namespace ursine
{
    namespace graphics
    {
        namespace DXCore
        {
            class DepthStencilManager
            {
                struct DepthStencil
                {
                    ID3D11DepthStencilView      *depthStencilView;
                    ID3D11Texture2D             *depthStencilTex2D;
                    ID3D11ShaderResourceView    *depthStencilSRV;

                    unsigned width;
                    unsigned height;
                };

            public:
                void Initialize(ID3D11Device *device, ID3D11DeviceContext *devicecontext, const int width, const int height, GfxInfo *gfxInfo);
                void Uninitialize(void);

                ID3D11DepthStencilView *GetDepthStencilView(const DEPTH_STENCIL_LIST stencil);
                ID3D11ShaderResourceView *GetDepthStencilSRV(const DEPTH_STENCIL_LIST stencil);

                GfxHND CreateShadowmapDepthTarget(unsigned width, unsigned height);
                void DestroyShadowmapDepthTarget(GfxHND &handle);

                DepthStencil &GetShadowmapDepthStencil(GfxHND handle);

                void ResizeShadowmapDepthTarget(GfxHND handle, unsigned width, unsigned height);

                void ResizeMainDepthTargets(const int width, const int height);

            private:
                void createInternalDepthTarget(DEPTH_STENCIL_LIST target, unsigned width, unsigned height, DXGI_FORMAT depthBufferDescFormat, DXGI_FORMAT viewDescFormat, DXGI_FORMAT srvFormat);
                void createShadowmapTarget(unsigned index, unsigned width, unsigned height, DXGI_FORMAT depthBufferDescFormat, DXGI_FORMAT viewDescFormat, DXGI_FORMAT srvFormat);
                
                void destroyTarget(DepthStencil &depthTarget);

                ID3D11Device        *m_device;
                ID3D11DeviceContext *m_deviceContext;

                GfxInfo             *m_gfxInfo;

                std::vector<DepthStencil>   m_depthStencilArray;
                std::vector<DepthStencil>   m_shadowMapArray;
                std::stack<unsigned>        m_shadowmapFreeStack;
            };
        }
    }
}
