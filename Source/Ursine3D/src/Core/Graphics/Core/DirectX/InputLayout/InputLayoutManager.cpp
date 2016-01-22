/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** InputLayoutManager.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"
#include "InputLayoutManager.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include "GfxDefines.h"
#include "DXErrorHandling.h"

namespace ursine
{
    namespace graphics
    {
        namespace DXCore
        {
            void InputLayoutManager::Initialize(ID3D11Device *device, ID3D11DeviceContext *devicecontext, ShaderManager *shdrmgr)
            {
                m_device = device;
                m_deviceContext = devicecontext;

                m_layoutArray.resize(SHADER_COUNT);

                //YEA BOI
                for (unsigned x = 0; x < SHADER_COUNT; ++x)
                {
                    m_layoutArray[ x ] = nullptr;
                    Shader *current = shdrmgr->GetShader((SHADER_TYPES)x);

                    if (current != nullptr && current->vs != nullptr && x != SHADER_DEFERRED_DEPTH)
                    {
                        HRESULT result = GetLayoutFromBlob(current, &m_layoutArray[ x ]);
                        UAssert(result == S_OK, "Failed to load layout from blob for shader %i.  (Error '%s')", x, GetDXErrorMessage(result));
                    }
                }

				// create super fucking finicky layout
				D3D11_INPUT_ELEMENT_DESC SKINNED_LAYOUT[5];

				SKINNED_LAYOUT[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
				SKINNED_LAYOUT[1] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
				SKINNED_LAYOUT[2] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
				SKINNED_LAYOUT[3] = { "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
				SKINNED_LAYOUT[4] = { "BLENDINDICES", 0, DXGI_FORMAT_R8G8B8A8_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
				
				HRESULT hr = m_device->CreateInputLayout(SKINNED_LAYOUT, static_cast<UINT>(5), shdrmgr->GetShader((SHADER_TYPES)SHADER_DEFERRED_DEPTH)->rawData, shdrmgr->GetShader((SHADER_TYPES)SHADER_DEFERRED_DEPTH)->size, &m_layoutArray[SHADER_DEFERRED_DEPTH]);
            }

            void InputLayoutManager::Uninitialize(void)
            {
                if (m_layoutArray.size() != 0)
                {
                    for (unsigned x = 0; x < SHADER_COUNT; ++x)
                    {
                        RELEASE_RESOURCE(m_layoutArray[ x ]);
                    }
                }
                m_device = nullptr;
                m_deviceContext = nullptr;
            }

            void InputLayoutManager::SetInputLayout(const SHADER_TYPES type)
            {
                if (m_currentState == type)
                    return;

                UAssert(m_layoutArray[ type ] != nullptr, "No input found for type %i!", type);

                m_currentState = type;

                m_deviceContext->IASetInputLayout(m_layoutArray[ type ]);
            }

            void InputLayoutManager::Invalidate(void)
            {
                m_currentState = SHADER_COUNT;
            }

            HRESULT InputLayoutManager::GetLayoutFromBlob(Shader *shader, ID3D11InputLayout **pInputLayout)
            {
                // Reflect shader info
                ID3D11ShaderReflection *pVertexShaderReflection = nullptr;
                HRESULT hr = D3DReflect(shader->rawData, shader->vsBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pVertexShaderReflection);
                UAssert(hr == S_OK, "failed to reflect! (Error '%s')", GetDXErrorMessage(hr));

                // Get shader info
                D3D11_SHADER_DESC shaderDesc;
                pVertexShaderReflection->GetDesc(&shaderDesc);
                std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;

                for (unsigned i = 0; i < shaderDesc.InputParameters; i++)
                {
                    D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
                    pVertexShaderReflection->GetInputParameterDesc(i, &paramDesc);

                    // fill out input element desc
                    D3D11_INPUT_ELEMENT_DESC elementDesc;
                    elementDesc.SemanticName = paramDesc.SemanticName;
                    elementDesc.SemanticIndex = paramDesc.SemanticIndex;
                    elementDesc.InputSlot = 0;
                    elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
                    elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
                    elementDesc.InstanceDataStepRate = 0;

                    std::string semanticName = std::string( paramDesc.SemanticName );

                    // check if we need to override the size
                    bool sizeOverride = false;
                    unsigned overrideSize = 0;
                    auto pos = semanticName.find( "_" );
                    if ( pos != std::string::npos )
                    {
                        sizeOverride = true;
                        std::string subStr = semanticName.substr( pos + 1 );

                        if ( subStr == "EIGHT" )
                            overrideSize = 8;
                        else
                            UAssert( false, "Failed to override vertex layout!" );
                    }

                    // determine DXGI format
                    if (paramDesc.Mask == 1)
                    {
                        // check if we need to override the size
                        if( sizeOverride)
                        {
                            if(overrideSize == 8)
                            {
                                if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32 )
                                    elementDesc.Format = DXGI_FORMAT_R8_UINT;
                                else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32 )
                                    elementDesc.Format = DXGI_FORMAT_R8_SINT;
                            }
                        }
                        else // handle it normally
                        {
                            if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32 )
                                elementDesc.Format = DXGI_FORMAT_R32_UINT;
                            else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32 )
                                elementDesc.Format = DXGI_FORMAT_R32_SINT;
                            else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32 )
                                elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
                        }
                    }
                    else if (paramDesc.Mask <= 3)
                    {
                        // check if we need to override the size
                        if ( sizeOverride )
                        {
                            if ( overrideSize == 8 )
                            {
                                if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32 )
                                    elementDesc.Format = DXGI_FORMAT_R8G8_UINT;
                                else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32 )
                                    elementDesc.Format = DXGI_FORMAT_R8G8_SINT;
                            }
                        }
                        else // handle it normally
                        {
                            if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32 )
                                elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
                            else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32 )
                                elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
                            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
                                elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
                        }
                    }
                    else if (paramDesc.Mask <= 7)
                    {
                        if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32 )
                            elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
                        else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32 )
                            elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
                        else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32 )
                            elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
                    }
                    else if (paramDesc.Mask <= 15)
                    {
                        // check if we need to override the size
                        if ( sizeOverride )
                        {
                            if ( overrideSize == 8 )
                            {
                                if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32 )
                                    elementDesc.Format = DXGI_FORMAT_R8G8B8A8_UINT;
                                else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32 )
                                    elementDesc.Format = DXGI_FORMAT_R8G8B8A8_SINT;
                            }
                        }
                        else // handle it normally
                        {
                            if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32 )
                                elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
                            else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32 )
                                elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
                            else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32 )
                                elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
                        }
                    }

                    //save element desc
                    inputLayoutDesc.push_back(elementDesc);
                }

                // Try to create Input Layout
                hr = m_device->CreateInputLayout(&inputLayoutDesc[ 0 ], static_cast<UINT>(inputLayoutDesc.size()), shader->rawData, shader->size, pInputLayout);
                shader->reflectionData = pVertexShaderReflection;
                return hr;
            }
        }
    }
}
