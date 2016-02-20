/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** InputLayoutManager.h
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
File Name:      InputLayoutManager.h
Module:         Graphics
Purpose:        manager for input layouts
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/

#pragma once

#include <vector>

#include "ShaderTypes.h"
#include "ShaderManager.h"
#include "D3D11Forward.h"

namespace ursine
{
    namespace graphics
    {
        namespace DXCore
        {
            class InputLayoutManager
            {
            public:
                void Initialize(ID3D11Device *device, ID3D11DeviceContext *devicecontext, ShaderManager *shdrmgr);
                void Uninitialize(void);

                void SetInputLayout(const SHADER_LIST type);

                void Invalidate(void);
            private:
                HRESULT GetLayoutFromBlob(Shader *shader, ID3D11InputLayout **pInputLayout);
                ID3D11Device *m_device;
                ID3D11DeviceContext *m_deviceContext;

                std::vector<ID3D11InputLayout *> m_layoutArray;

                SHADER_LIST m_currentState;
            };
        }
    }
}
