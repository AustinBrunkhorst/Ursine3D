#include "UrsinePrecompiled.h"
#include "ShaderBufferManager.h"
#include <d3d11.h>
#include "DXErrorHandling.h"


namespace ursine
{
    namespace graphics
    {
        namespace DXCore
        {
            void ShaderBufferManager::Initialize(ID3D11Device *device, ID3D11DeviceContext *context)
            {
                m_device = device;
                m_deviceContext = context;

                //init
                m_bufferArray.resize(BUFFER_COUNT);

                //make all the buffers
                MakeBuffer<CameraBuffer>(BUFFER_CAMERA);
                MakeBuffer<TransformBuffer>(BUFFER_TRANSFORM);
                MakeBuffer<DirectionalLightBuffer>(BUFFER_DIRECTIONAL_LIGHT);
                MakeBuffer<PointLightBuffer>(BUFFER_POINT_LIGHT);
                MakeBuffer<InvProjBuffer>(BUFFER_INV_PROJ);
                MakeBuffer<PrimitiveColorBuffer>(BUFFER_PRIM_COLOR);
                MakeBuffer<PointGeometryBuffer>(BUFFER_POINT_GEOM);
                MakeBuffer<BillboardSpriteBuffer>(BUFFER_BILLBOARDSPRITE);
                MakeBuffer<GBufferUnpackBuffer>(BUFFER_GBUFFER_UNPACK);
                MakeBuffer<TransformBuffer>(BUFFER_LIGHT_PROJ);
                MakeBuffer<MaterialDataBuffer>(BUFFER_MATERIAL_DATA);
                MakeBuffer<SpotlightBuffer>(BUFFER_SPOTLIGHT);
				MakeBuffer<MatrixPalBuffer>(BUFFER_MATRIX_PAL);
            }

            void ShaderBufferManager::Uninitialize(void)
            {
                //release buffers
                for (unsigned x = 0; x < BUFFER_COUNT; ++x)
                {
                    RELEASE_RESOURCE(m_bufferArray[ x ]);
                }

                m_device = nullptr;
                m_deviceContext = nullptr;
            }

            /////////////////////////////////////////////////////////////////
            // SHADER BUFFER MAPPING ////////////////////////////////////////
            void ShaderBufferManager::MapCameraBuffer(const SMat4 &view, const SMat4 &projection, const SHADERDEF shader, const unsigned int bufferIndex)
            {
                UAssert(bufferIndex < MAX_CONST_BUFF, "ResourceManager attempted to map buffer to invalid index (index #%i)", bufferIndex);
                HRESULT result;
                D3D11_MAPPED_SUBRESOURCE mappedResource;

                CameraBuffer *dataPtr;

                //lock the buffer
                result = m_deviceContext->Map(m_bufferArray[ BUFFER_CAMERA ], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
                UAssert(result == S_OK, "Failed to lock shader buffer for camera! (Error '%s')", GetDXErrorMessage(result));

                //grab data
                dataPtr = (CameraBuffer*)mappedResource.pData;

                //set data
                dataPtr->view = SMat4::Transpose(view).ToD3D( );
                dataPtr->projection = SMat4::Transpose(projection).ToD3D( );

                //unlock buffer
                m_deviceContext->Unmap(m_bufferArray[ BUFFER_CAMERA ], 0);

                //map to a given buffer
                SetBuffer(shader, bufferIndex, m_bufferArray[ BUFFER_CAMERA ]);
            }

            void ShaderBufferManager::MapTransformBuffer(const SMat4 &transform, const SHADERDEF shader, const unsigned int bufferIndex)
            {
                UAssert(bufferIndex < MAX_CONST_BUFF, "ResourceManager attempted to map buffer to invalid index (index #%i)", bufferIndex);
                HRESULT result;
                D3D11_MAPPED_SUBRESOURCE mappedResource;

                TransformBuffer *dataPtr;

                //lock the buffer
                result = m_deviceContext->Map(m_bufferArray[ BUFFER_TRANSFORM ], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
                UAssert(result == S_OK, "Failed to lock shader buffer for transform! (Error '%s')", GetDXErrorMessage(result));

                //grab data
                dataPtr = (TransformBuffer*)mappedResource.pData;

                //set data
                dataPtr->transform = transform.ToD3D( );

                //unlock buffer
                m_deviceContext->Unmap(m_bufferArray[ BUFFER_TRANSFORM ], 0);

                //map to a given buffer
                SetBuffer(shader, bufferIndex, m_bufferArray[ BUFFER_TRANSFORM ]);
            }

            /////////////////////////////////////////////////////////////////
            // PRIVATE METHODS //////////////////////////////////////////////
            void ShaderBufferManager::SetBuffer(const SHADERDEF shader, const unsigned bufferIndex, ID3D11Buffer *buffer)
            {
                switch (shader)
                {
                case VERTEX_SHADER:
                    m_deviceContext->VSSetConstantBuffers(bufferIndex, 1, &buffer);
                    break;
                case PIXEL_SHADER:
                    m_deviceContext->PSSetConstantBuffers(bufferIndex, 1, &buffer);
                    break;
                case HULL_SHADER:
                    m_deviceContext->HSSetConstantBuffers(bufferIndex, 1, &buffer);
                    break;
                case DOMAIN_SHADER:
                    m_deviceContext->DSSetConstantBuffers(bufferIndex, 1, &buffer);
                    break;
                case GEOMETRY_SHADER:
                    m_deviceContext->GSSetConstantBuffers(bufferIndex, 1, &buffer);
                    break;
                }
            }

            template<typename T>
            void ShaderBufferManager::MakeBuffer(const BUFFER_LIST type)
            {
                UAssert(sizeof(T) % 16 == 0, "Invalid constant buffer! Constant buffer must have a multiple of 16 as its byte width!");

                HRESULT result;
                D3D11_BUFFER_DESC matrixBufferDesc;

                matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
                matrixBufferDesc.ByteWidth = sizeof(T);
                matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
                matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
                matrixBufferDesc.MiscFlags = 0;
                matrixBufferDesc.StructureByteStride = 0;

                //Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
                result = m_device->CreateBuffer(&matrixBufferDesc, nullptr, &m_bufferArray[ type ]);
                UAssert(result == S_OK, "Failed to make buffer! (type: %i)  (Error '%s')", type, GetDXErrorMessage(result));
            }
        }
    }
}
