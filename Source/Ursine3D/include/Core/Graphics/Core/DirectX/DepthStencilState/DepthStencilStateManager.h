/* Start Header ---------------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
=============================================================================*/
/*!
File Name:      DepthStencilStateManager.h
Module:         Graphics
Purpose:        Manager for the depth stencil states
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/

#pragma once

#include <vector>

#include "D3D11Forward.h"
#include "DepthStencilStateList.h"

namespace ursine
{
    namespace graphics
    {
        namespace DXCore
        {
            class DepthStencilStateManager
            {
            public:
                void Initialize(ID3D11Device *device, ID3D11DeviceContext *devicecontext);
                void Uninitialize(void);

                ID3D11DepthStencilState *GetStencilState(const DEPTH_STATES state);
                void SetDepthState(const DEPTH_STATES state);

                void Invalidate(void);
            private:
                ID3D11Device *m_device;
                ID3D11DeviceContext *m_deviceContext;

                std::vector<ID3D11DepthStencilState*> m_depthStateArray;

                DEPTH_STATES m_currentState;
            };
        }
    }
}
