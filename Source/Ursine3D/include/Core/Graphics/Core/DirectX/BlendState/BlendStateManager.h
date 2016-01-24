/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** BlendStateManager.h
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
File Name:      BlendStateManager.h
Module:         Graphics
Purpose:        backend for managing blend states
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/
#pragma once

#include "D3D11Forward.h"
#include <vector>
#include "BlendStateList.h"

namespace ursine
{
    namespace graphics
    {
        namespace DXCore
        {
            class BlendStateManager
            {
            public:
                void Initialize(ID3D11Device *device, ID3D11DeviceContext *devicecontext);
                void Uninitialize(void);

                ID3D11BlendState *GetBlendState(const BLEND_STATES state);
                void SetBlendState(const BLEND_STATES state);

                void Invalidate(void);

            private:
                ID3D11Device *m_device;
                ID3D11DeviceContext *m_deviceContext;

                std::vector<ID3D11BlendState*> BlendStateArray_;

                BLEND_STATES m_currentState;
            };
        }
    }
}
