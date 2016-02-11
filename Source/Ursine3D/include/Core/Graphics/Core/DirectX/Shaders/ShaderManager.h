/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ShaderManager.h
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
File Name:      ShaderManager.h
Module:         Graphics
Purpose:        backend manager for shader management
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/
#pragma once

#include "D3D11Forward.h"
#include <string>
#include <vector>

#include "ShaderTypes.h"

namespace ursine
{
    namespace graphics
    {
        namespace DXCore
        {
            struct Shader
            {
                SHADER_TYPES type;

                ID3D11VertexShader          *vs;
                ID3D11PixelShader           *ps;
                ID3D11HullShader            *hs;
                ID3D11DomainShader          *ds;
                ID3D11GeometryShader        *gs;
                ID3D11ComputeShader         *cs;

                ID3D11ShaderReflection      *reflectionData;
                ID3DBlob                    *vsBlob;
                void                        *rawData;
                unsigned                    size;

                std::vector<std::string>    filenames;
                std::vector<time_t>         timestamps;

                Shader() 
                    : type(SHADER_COUNT)
                    , vs(nullptr)
                    , ps(nullptr)
                    , hs(nullptr)
                    , ds(nullptr)
                    , gs(nullptr)
                    , cs(nullptr)
                    , reflectionData(nullptr)
                    , vsBlob(nullptr)
                    , rawData(nullptr)
                    , size(0)
                {
                    filenames.resize(SHADERTYPE_COUNT);
                    timestamps.resize(SHADERTYPE_COUNT);
                }
            };

            class ShaderManager
            {
            //public methods
            public:
                void Initialize(ID3D11Device *device, ID3D11DeviceContext *context, std::string filePath = "");
                void Uninitialize(void);

                void BindShader(const SHADER_TYPES shader);

                void LoadShader(const SHADER_TYPES shader, const char *filename);

                void Render(const unsigned vert_count);

                Shader *GetShader(const SHADER_TYPES shader);

                void Invalidate(void);

            //private members
            private:
                std::string m_shaderPath;
                ID3D11Device *m_device;
                ID3D11DeviceContext *m_deviceContext;

                SHADER_TYPES m_currentState;
                std::vector<Shader*> m_shaderArray;
            };
        }
    }
}
