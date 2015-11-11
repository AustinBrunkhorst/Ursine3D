#include "UrsinePrecompiled.h"
#include "ShaderManager.h"
#include "GfxDefines.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include "DXErrorHandling.h"

#define CONCAT_END(file, ending) file ## ending
#define GetString(type) #type

namespace ursine
{
    namespace graphics
    {
        namespace DXCore
        {
            static bool CheckFile(const std::string &name)
            {
                FILE *file;

                if (0 == fopen_s(&file, name.c_str(), "r"))
                {
                    fclose(file);
                    return true;
                }
                else
                {
                    return false;
                }
            }

            static int _backendLoadShader(ID3D11Device *dev, Shader *shader, SHADERDEF shaderType, const char *filename, const char *filePath)
            {
                std::string path = filePath;
                path.append(filename);

                //make sure it exists, if not just return (assuming the shader doesn't exist)
                if (!CheckFile(path))
                    return 0;

                //VERTEX_SHADER = 0,
                //  PIXEL_SHADER,
                //  HULL_SHADER,
                //  DOMAIN_SHADER,
                //  GEOMETRY_SHADER,

                switch (shaderType)
                {
                case VERTEX_SHADER:
                    LogMessage("Vertex Shader Found", 4);
                    break;
                case PIXEL_SHADER:
                    LogMessage("Pixel Shader Found", 4);
                    break;
                case HULL_SHADER:
                    LogMessage("Hull Shader Found", 4);
                    break;
                case DOMAIN_SHADER:
                    LogMessage("Domain Shader Found", 4);
                    break;
                case GEOMETRY_SHADER:
                    LogMessage("Geometry Shader Found", 4);
                    break;
                default:
                    break;
                }
                //

                HRESULT result;
                struct stat *st = new struct stat();

                //get timestamp
                int error = stat(path.c_str(), st);
                time_t time = st->st_mtime;

                //open file
                std::ifstream in;
                in.open(path.c_str(), std::ifstream::in | std::ifstream::binary);

                //go to end, get size
                size_t binSize;
                void *binData;
                in.seekg(0, std::ios::end);
                binSize = size_t(in.tellg());
                binData = new char[ binSize ];
                in.seekg(0, std::ios::beg);

                //read in data
                in.read(reinterpret_cast<char*>(binData), binSize);
                in.close();

                //compile shader, save name data
                switch (shaderType)
                {
                case SHADERDEF::VERTEX_SHADER:
                    result = dev->CreateVertexShader(binData, binSize, nullptr, &shader->vs);
                    UAssert(result == S_OK, "Failed to load vert shader '%s'! (Error '%s')", GetDXErrorMessage(result));

                    result = D3DCreateBlob(binSize, &shader->vsBlob);
                    UAssert(result == S_OK, "Failed to create vert blob '%s'! (Error '%s')", GetDXErrorMessage(result));

                    shader->size = (unsigned)binSize;
                    shader->rawData = binData;
                    break;
                case SHADERDEF::PIXEL_SHADER:
                    result = dev->CreatePixelShader(binData, binSize, nullptr, &shader->ps);
                    UAssert(result == S_OK, "Failed to load pixel shader '%s'! (Error '%s')", GetDXErrorMessage(result));
                    break;
                case SHADERDEF::HULL_SHADER:
                    result = dev->CreateHullShader(binData, binSize, nullptr, &shader->hs);
                    UAssert(result == S_OK, "Failed to load hull shader '%s'! (Error '%s')", GetDXErrorMessage(result));
                    break;
                case SHADERDEF::DOMAIN_SHADER:
                    result = dev->CreateDomainShader(binData, binSize, nullptr, &shader->ds);
                    UAssert(result == S_OK, "Failed to load domain shader '%s'! (Error '%s')", GetDXErrorMessage(result));
                    break;
                case SHADERDEF::GEOMETRY_SHADER:
                    result = dev->CreateGeometryShader(binData, binSize, nullptr, &shader->gs);
                    UAssert(result == S_OK, "Failed to load geometry shader '%s'! (Error '%s')", GetDXErrorMessage(result));
                    break;
                }

                //update time, filename
                shader->timestamps[ shaderType ] = time;
                shader->filenames[ shaderType ].clear();
                shader->filenames[ shaderType ].append(filename);

                delete st;
                return 1;
            }

            void ShaderManager::Initialize(ID3D11Device *device, ID3D11DeviceContext *context, std::string filePath)
            {
                m_device = device;
                m_deviceContext = context;

                m_currentState = SHADER_COUNT;

                m_shaderArray.resize(SHADER_COUNT);

                m_shaderPath = filePath;

                for (unsigned x = 0; x < SHADER_COUNT; ++x)
                {
                    m_shaderArray[ x ] = nullptr;
                }
            }

            void ShaderManager::Uninitialize(void)
            {
                for (unsigned x = 0; x < SHADER_COUNT; ++x)
                {
                    if (m_shaderArray[ x ] != nullptr)
                    {
                        RELEASE_RESOURCE(m_shaderArray[ x ]->vs);
                        RELEASE_RESOURCE(m_shaderArray[ x ]->ps);
                        RELEASE_RESOURCE(m_shaderArray[ x ]->hs);
                        RELEASE_RESOURCE(m_shaderArray[ x ]->ds);
                        RELEASE_RESOURCE(m_shaderArray[ x ]->gs);
                        RELEASE_RESOURCE(m_shaderArray[ x ]->vsBlob);
                        RELEASE_RESOURCE(m_shaderArray[ x ]->reflectionData);

                        delete m_shaderArray[ x ];
                    }
                }

                m_device = nullptr;
                m_deviceContext = nullptr;
            }

            void ShaderManager::BindShader(const SHADER_TYPES shader)
            {
                m_currentState = shader;

                //if a given shader exists, bind it
                if (m_shaderArray[ shader ]->vs != nullptr)
                    m_deviceContext->VSSetShader(m_shaderArray[ shader ]->vs, nullptr, 0);
                if (m_shaderArray[ shader ]->ps != nullptr)
                    m_deviceContext->PSSetShader(m_shaderArray[ shader ]->ps, nullptr, 0);
                if (m_shaderArray[ shader ]->hs != nullptr)
                    m_deviceContext->HSSetShader(m_shaderArray[ shader ]->hs, nullptr, 0);
                if (m_shaderArray[ shader ]->ds != nullptr)
                    m_deviceContext->DSSetShader(m_shaderArray[ shader ]->ds, nullptr, 0);
                if (m_shaderArray[ shader ]->gs != nullptr)
                    m_deviceContext->GSSetShader(m_shaderArray[ shader ]->gs, nullptr, 0);
                else
                    m_deviceContext->GSSetShader(nullptr, nullptr, 0);
            }

            void ShaderManager::LoadShader(const SHADER_TYPES shader, const char *filename)
            {
                UAssert(m_shaderArray[ shader ] == nullptr, "Attempted to load shader twice");
                m_shaderArray[ shader ] = new Shader;

                LogMessage("Loading Shader: %s", 2, filename);
                //LogMessage("Path: ", 3);
                LogMessage("Shader Types Found:", 3);

                //init to 0
                m_shaderArray[ shader ]->vsBlob = nullptr;
                m_shaderArray[ shader ]->vs = nullptr;
                m_shaderArray[ shader ]->ps = nullptr;
                m_shaderArray[ shader ]->hs = nullptr;
                m_shaderArray[ shader ]->ds = nullptr;
                m_shaderArray[ shader ]->gs = nullptr;

                //attempt to load all shaders
                int result = 0;
                result |= _backendLoadShader(m_device, m_shaderArray[ shader ], VERTEX_SHADER, std::string(filename).append("_VS.cso").c_str(), m_shaderPath.c_str());
                result |= _backendLoadShader(m_device, m_shaderArray[ shader ], PIXEL_SHADER, std::string(filename).append("_PS.cso").c_str(), m_shaderPath.c_str());
                result |= _backendLoadShader(m_device, m_shaderArray[ shader ], HULL_SHADER, std::string(filename).append("_HS.cso").c_str(), m_shaderPath.c_str());
                result |= _backendLoadShader(m_device, m_shaderArray[ shader ], DOMAIN_SHADER, std::string(filename).append("_DS.cso").c_str(), m_shaderPath.c_str());
                result |= _backendLoadShader(m_device, m_shaderArray[ shader ], GEOMETRY_SHADER, std::string(filename).append("_GS.cso").c_str(), m_shaderPath.c_str());

                //if they all return 0, we have a problem
                UAssert(result > 0, "Failed to load shader '%s'", filename);
            }

            void ShaderManager::Render(const unsigned vert_count)
            {
                UAssert(vert_count > 0, "Attempted to render with 0 vert count!");
                m_deviceContext->DrawIndexed(vert_count, 0, 0);
            }

            Shader *ShaderManager::GetShader(const SHADER_TYPES shader)
            {
                return m_shaderArray[ shader ];
            }

            void ShaderManager::Invalidate(void)
            {
                m_currentState = SHADER_COUNT;
            }
        }
    }
}
