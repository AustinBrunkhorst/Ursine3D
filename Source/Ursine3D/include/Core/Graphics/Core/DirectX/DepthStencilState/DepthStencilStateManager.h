/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** DepthStencilStateManager.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

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
                DEPTH_STATES GetCurrentDepthState(void) const;

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
