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
                m_computeBufferArray.resize(COMPUTE_BUFFER_COUNT);
                m_computeSRV.resize(COMPUTE_BUFFER_COUNT);
                m_computeUAV.resize(COMPUTE_BUFFER_COUNT);

                //make all the buffers

                //these buffers are GPU read only, CPU-write
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
                MakeBuffer<MouseBuffer>(BUFFER_MOUSEPOS);

                // COMPUTE SHADERS //////////////////////////////////
                //GPU-readonly buffer that can only be written to by the CPU 
                MakeComputeBuffer<MouseBuffer>(1, COMPUTE_BUFFER_MOUSEPOS, D3D11_USAGE_DYNAMIC, D3D11_BIND_SHADER_RESOURCE, D3D11_CPU_ACCESS_WRITE);
                //requires SRV to set 
                MakeComputeSRV(1, COMPUTE_BUFFER_MOUSEPOS);

                //this is output from compute shader, GPU write only. CPU can't read
                //                   type     1 element  buffer enum           usage                  binding         cpu access
                MakeComputeBuffer<ComputeIDOutput>(5, COMPUTE_BUFFER_ID, D3D11_USAGE_DEFAULT, D3D11_BIND_UNORDERED_ACCESS, 0);

                //requires UAV to write as compute output
                MakeComputeUAV(5, COMPUTE_BUFFER_ID);

                //create a buffer for copying data from the gpu onto the cpu
                //                   type     1 element  buffer enum           usage           binding     cpu access
                MakeComputeBuffer<ComputeIDOutput>(5, COMPUTE_BUFFER_ID_CPU, D3D11_USAGE_STAGING, 0, D3D11_CPU_ACCESS_READ, false);
            }

            void ShaderBufferManager::Uninitialize(void)
            {
                //release buffers
                for (unsigned x = 0; x < BUFFER_COUNT; ++x)
                {
                    RELEASE_RESOURCE(m_bufferArray[ x ]);
                }

                for (unsigned x = 0; x < COMPUTE_BUFFER_COUNT; ++x)
                {
                    RELEASE_RESOURCE(m_computeBufferArray[ x ]);
                    RELEASE_RESOURCE(m_computeSRV[ x ]);
                    RELEASE_RESOURCE(m_computeUAV[ x ]);
                }

                m_device = nullptr;
                m_deviceContext = nullptr;
            }

            /////////////////////////////////////////////////////////////////
            // SHADER BUFFER MAPPING ////////////////////////////////////////
            void ShaderBufferManager::MapCameraBuffer(const SMat4 &view, const SMat4 &projection, const SHADERTYPE shader, const unsigned int bufferIndex)
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

            void ShaderBufferManager::MapTransformBuffer(const SMat4 &transform, const SHADERTYPE shader, const unsigned int bufferIndex)
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
            void ShaderBufferManager::SetBuffer(const SHADERTYPE shader, const unsigned bufferIndex, ID3D11Buffer *buffer)
            {
                switch (shader)
                {
                case SHADERTYPE_VERTEX:
                    m_deviceContext->VSSetConstantBuffers(bufferIndex, 1, &buffer);
                    break;
                case SHADERTYPE_PIXEL:
                    m_deviceContext->PSSetConstantBuffers(bufferIndex, 1, &buffer);
                    break;
                case SHADERTYPE_HULL:
                    m_deviceContext->HSSetConstantBuffers(bufferIndex, 1, &buffer);
                    break;
                case SHADERTYPE_DOMAIN:
                    m_deviceContext->DSSetConstantBuffers(bufferIndex, 1, &buffer);
                    break;
                case SHADERTYPE_GEOMETRY:
                    m_deviceContext->GSSetConstantBuffers(bufferIndex, 1, &buffer);
                    break;
                case SHADERTYPE_COMPUTE:
                    m_deviceContext->CSSetConstantBuffers(bufferIndex, 1, &buffer);
                    break;
                default:
                    UAssert(false, "Failed to set buffer!");
                    break;
                }
            }

            template<typename T>
            void ShaderBufferManager::MakeBuffer(const BUFFER_LIST type, unsigned gpuUsage, unsigned cpuAccess)
            {
                UAssert(sizeof(T) % 16 == 0, "Invalid constant buffer! Constant buffer must have a multiple of 16 as its byte width!");

                HRESULT result;
                D3D11_BUFFER_DESC bufferDesc;

                //how to use this? is it for shaders?
                bufferDesc.Usage = static_cast<D3D11_USAGE>(gpuUsage);

                //how big is this buffer
                bufferDesc.ByteWidth = sizeof(T);

                //how will it be used?
                bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

                //how does the CPU access this?
                bufferDesc.CPUAccessFlags = cpuAccess;

                //other stuff
                bufferDesc.MiscFlags = 0;
                bufferDesc.StructureByteStride = 0;

                //Create the constant buffer pointer so we can access the shader constant buffer from within this class.
                result = m_device->CreateBuffer(&bufferDesc, nullptr, &m_bufferArray[ type ]);
                UAssert(result == S_OK, "Failed to make buffer! (type: %i)  (Error '%s')", type, GetDXErrorMessage(result));
            }

            template<typename T>
            void ShaderBufferManager::MakeComputeBuffer(const int count, const COMPUTE_BUFFER_LIST type, unsigned usageFlag, unsigned bindFlags, unsigned cpuAccess, bool bufferStruct2D)
            {
                HRESULT result;
                D3D11_BUFFER_DESC inputDesc;
                inputDesc.Usage = static_cast<D3D11_USAGE>(usageFlag);
                inputDesc.ByteWidth = sizeof(T) * count;                    //size of total buffer
                inputDesc.BindFlags = bindFlags;
                inputDesc.CPUAccessFlags = cpuAccess;
                inputDesc.StructureByteStride = sizeof(T);          //size of 1 element
                inputDesc.MiscFlags = bufferStruct2D == true ? D3D11_RESOURCE_MISC_BUFFER_STRUCTURED : 0;

                //create the buffer
                result = m_device->CreateBuffer(&inputDesc, nullptr, &m_computeBufferArray[ type ]);
                UAssert(result == S_OK, "Failed to make compute buffer! (type: %i)  (Error '%s')", type, GetDXErrorMessage(result));
            }

            void ShaderBufferManager::MakeComputeSRV(const int count, const COMPUTE_BUFFER_LIST type)
            {
                HRESULT result;

                D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
                srvDesc.Format = DXGI_FORMAT_UNKNOWN;
                srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
                srvDesc.BufferEx.FirstElement = 0;
                srvDesc.BufferEx.Flags = 0;
                srvDesc.BufferEx.NumElements = count;

                result = m_device->CreateShaderResourceView(m_computeBufferArray[ type ], &srvDesc, &m_computeSRV[ type ]);
                UAssert(result == S_OK, "Failed to make compute buffer srv! (type: %i)  (Error '%s')", type, GetDXErrorMessage(result));
            }

            void ShaderBufferManager::MakeComputeUAV(const int count, const COMPUTE_BUFFER_LIST type)
            {
                //since this will be output, we need to define a UAV
                HRESULT result;
                D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
                uavDesc.Format = DXGI_FORMAT_UNKNOWN;
                uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
                uavDesc.Buffer.FirstElement = 0;
                uavDesc.Buffer.Flags = 0;
                uavDesc.Buffer.NumElements = count;

                result = m_device->CreateUnorderedAccessView(m_computeBufferArray[ type ], &uavDesc, &m_computeUAV[ type ]);
                UAssert(result == S_OK, "Failed to make compute buffer uav! (type: %i)  (Error '%s')", type, GetDXErrorMessage(result));
            }
        }
    }
}
