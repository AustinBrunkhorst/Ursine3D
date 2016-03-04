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

namespace ursine
{
    namespace graphics
    {
        namespace DXCore
        {
            class DepthStencilManager
            {
            public:
                void Initialize(ID3D11Device *device, ID3D11DeviceContext *devicecontext, const int width, const int height, GfxInfo *gfxInfo);
                void Uninitialize(void);

                ID3D11DepthStencilView *GetDepthStencilView(const DEPTH_STENCIL_LIST stencil);
                ID3D11ShaderResourceView *GetDepthStencilSRV(const DEPTH_STENCIL_LIST stencil);

                void Resize(const int width, const int height);

                void Invalidate(void);
            private:
                ID3D11Device *m_device;
                ID3D11DeviceContext *m_deviceContext;

                GfxInfo *m_gfxInfo;

                std::vector<ID3D11DepthStencilView*> m_depthStencilViewArray;
                std::vector<ID3D11Texture2D*> m_depthStencilTextureArray;
                std::vector<ID3D11ShaderResourceView*> m_depthStencilResourceArray;

                DEPTH_STENCIL_LIST m_currentState;
            };
        }
    }
}
