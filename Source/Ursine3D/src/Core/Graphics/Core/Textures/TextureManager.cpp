#include "UrsinePrecompiled.h"
#include "TextureManager.h"
#include "DDSTextureLoader.h"
#include <d3d11.h>

namespace ursine
{
    namespace graphics
    {
        static const wchar_t *strToWchart(std::string sInput)
        {
            wchar_t *wCharOutput = new wchar_t[ 1023 ];
            size_t *sizeOut = new size_t;
            size_t sizeInWords = 256;

            const char *cStr;
            cStr = sInput.c_str();
            mbstowcs_s(sizeOut, wCharOutput, sizeInWords, cStr, strlen(cStr) + 1);

            return wCharOutput;
        }

        void TextureManager::Initialize(ID3D11Device *device, ID3D11DeviceContext *context, std::string filePath)
        {
            m_device = device;
            m_deviceContext = context;

            m_textureCount = 0;

            /////////////////////////////////////////////////////////////////
            // LOADING TEXTURES /////////////////////////////////////////////
            char buffer[ 512 ];
            std::ifstream input;
            std::string fileText = filePath;
            fileText.append("TEXTURELIST.8.0.gfx");
            input.open(fileText, std::ios_base::in);

            UAssert(input.is_open(), "Failed to open texture list file! (Path '%s')", filePath.c_str());
            while (input.eof() == false)
            {
                //zero it out
                memset(buffer, 0, sizeof(char) * 512);

                //get the line
                input.getline(buffer, 512);

                //if nothing on line, or # comment, continue;
                if (buffer[ 0 ] == '#' || strlen(buffer) == 0)
                    continue;

                //use string, and vector for holding tokens
                std::string data(buffer);
                std::vector<std::string> tokens;

                //deal with data, chop it up by space
                size_t pos = data.find_first_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", 0);
                while (pos != std::string::npos)
                {
                    size_t end = data.find_first_of(" ", pos + 1);
                    if (end - pos > 1)
                        tokens.push_back(data.substr(pos, end - pos + 1));
                    pos = data.find_first_of(" ", end);
                }

                //0 is filename
                tokens[ 0 ].insert(0, filePath);
                tokens[ 0 ].pop_back();
                //1 is name
                tokens[ 1 ].erase(0, 1);
                tokens[ 1 ].pop_back();
                //width
                unsigned width = atoi(tokens[ 2 ].c_str());
                //height
                unsigned height = atoi(tokens[ 2 ].c_str());


                TextureLoadBackend(tokens[ 1 ], tokens[ 0 ], width, height);
            }

            input.close();

            /////////////////////////////////////////////////////////////////
            // CREATING SAMPLER STATES //////////////////////////////////////
            m_samplerStateList_.resize(SAMPLER_COUNT);

            HRESULT result;
            D3D11_SAMPLER_DESC samplerDesc;

            // Texture Wrap /////////////////////////////////////////////////
            samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
            samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
            samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
            samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
            samplerDesc.MipLODBias = 0.0f;
            samplerDesc.MaxAnisotropy = 16;
            samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
            samplerDesc.BorderColor[ 0 ] = 0;
            samplerDesc.BorderColor[ 1 ] = 0;
            samplerDesc.BorderColor[ 2 ] = 0;
            samplerDesc.BorderColor[ 3 ] = 0;
            samplerDesc.MinLOD = 0;
            samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

            //Create the texture sampler state.
            result = device->CreateSamplerState(&samplerDesc, &m_samplerStateList_[ SAMPLER_WRAP_TEX ]);
            UAssert(result == S_OK, "Failed to make sampler state!");

            // no sampling //////////////////////////////////////////////////
            samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
            samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
            samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
            samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
            samplerDesc.MipLODBias = 0.0f;
            samplerDesc.MaxAnisotropy = 16;
            samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
            samplerDesc.BorderColor[ 0 ] = 0;
            samplerDesc.BorderColor[ 1 ] = 0;
            samplerDesc.BorderColor[ 2 ] = 0;
            samplerDesc.BorderColor[ 3 ] = 0;
            samplerDesc.MinLOD = 0;
            samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

            //Create the texture sampler state.
            result = device->CreateSamplerState(&samplerDesc, &m_samplerStateList_[ SAMPLER_NO_FILTERING ]);
            UAssert(result == S_OK, "Failed to make sampler state!");
        }

        void TextureManager::Uninitialize(void)
        {
            for (auto i : m_textureList)
            {
                if (i.second != NULL)
                    RELEASE_RESOURCE(i.second->Texture_);
            }

            m_textureList.clear();

            for (int x = 0; x < SAMPLER_COUNT; ++x)
            {
                RELEASE_RESOURCE(m_samplerStateList_[ x ])
            }

            m_device = nullptr;
            m_deviceContext = nullptr;
        }

        void TextureManager::MapTextureByName(const std::string name, const unsigned int bufferIndex)
        {
            m_deviceContext->PSSetShaderResources(bufferIndex, 1, &m_textureList[ name ]->Texture_);
        }

        void TextureManager::MapTextureByID(const unsigned ID, const unsigned int bufferIndex)
        {
            m_deviceContext->PSSetShaderResources(bufferIndex, 1, &m_hashTextureList[ ID ]->Texture_);
        }

        void TextureManager::MapSamplerState(const Sampler type, const unsigned bufferIndex)
        {
            m_deviceContext->PSSetSamplers(bufferIndex, 1, &m_samplerStateList_[ type ]);
        }

        unsigned TextureManager::GetTextureIDByName(const std::string name)
        {
            return m_lookupTextureList[ name ];
        }

        void TextureManager::TextureLoadBackend(const std::string name, const std::string path, const unsigned width, const unsigned height)
        {
            HRESULT result;
            D3D11_TEXTURE2D_DESC desc;

            LogMessage("Texture: %s", 2, name.c_str());
            LogMessage("Path: %s", 3, path.c_str());
            LogMessage("Width: %i", 3, width);
            LogMessage("Height: %i", 3, height);

            //width/height
            desc.Width = width;
            desc.Height = height;
            desc.MipLevels = desc.ArraySize = 1;
            desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            desc.SampleDesc.Count = 2;
            desc.SampleDesc.Quality = 0;
            desc.Usage = D3D11_USAGE_IMMUTABLE;
            desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
            desc.CPUAccessFlags = 0;
            desc.MiscFlags = 0;

            m_textureList[ name ] = new Texture();

            result = DirectX::CreateDDSTextureFromFile(m_device, strToWchart(path), nullptr, &m_textureList[ name ]->Texture_);

            UAssert(result != HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND), "Texture '%s' was not found!", path.c_str());
            UAssert(result == S_OK, "Failed to load texture: '%s'", name.c_str());

            m_textureList[ name ]->Width_ = width;
            m_textureList[ name ]->Height_ = height;
            m_lookupTextureList[ name ] = m_textureCount;
            m_hashTextureList[ m_textureCount++ ] = m_textureList[ name ];
        }
    }
}
