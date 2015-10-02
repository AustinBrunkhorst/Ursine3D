/* Start Header ---------------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
=============================================================================*/
/*!
File Name:      DepthStencilManager.h
Module:         Graphics
Purpose:        manager for handling depth states
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/
#pragma once

#include "D3D11Forward.h"
#include <vector>
#include "DepthStencilList.h"


namespace ursine
{
    class DepthStencilManager
    {
    public:
        void Initialize(ID3D11Device *device, ID3D11DeviceContext *devicecontext, const int width, const int height);
        void Uninitialize(void);

        ID3D11DepthStencilView *GetDepthStencilView(const DEPTH_STENCIL_LIST stencil);
        ID3D11ShaderResourceView *GetDepthStencilSRV(const DEPTH_STENCIL_LIST stencil);

        void Resize(const int width, const int height);

        void Invalidate(void);
    private:
        ID3D11Device *m_device;
        ID3D11DeviceContext *m_deviceContext;

        std::vector<ID3D11DepthStencilView*> m_depthStencilViewArray;
        std::vector<ID3D11Texture2D*> m_depthStencilTextureArray;
        std::vector<ID3D11ShaderResourceView*> m_depthStencilResourceArray;

        DEPTH_STENCIL_LIST m_currentState;
    };
}
