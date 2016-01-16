/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ModelManager.cpp
**
** Author:
** - Jason Burch - j.orion@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"
#include "ModelManager.h"
#include "GfxDefines.h"
#include "VertexDefinitions.h"
#include <d3d11.h>
#include "AnimationBuilder.h"
#include "BoneInfo.h"


#include "CFBXLoader.h"

namespace ursine
{
    namespace graphics
    {
        void ModelManager::Initialize(ID3D11Device *device, ID3D11DeviceContext *context, std::string filePath)
        {
            m_device = device;
            m_deviceContext = context;

            m_modelCount = 0;
            m_currentState = -1;

            AnimationBuilder::InitializeStaticData( );

            //loading all models
            char buffer[ 1024 ];
			std::ifstream input;
			std::string fileText = filePath;
			fileText.append("MODELS.8.0.gfx");
			input.open(fileText, std::ios_base::in);

			UAssert(input.is_open(), "Failed to open file for model loading! ('%s')", filePath.c_str());
			while (input.eof() == false)
			{
				//zero it out
				memset(buffer, 0, sizeof(char) * 1024);

				//get the line
				input.getline(buffer, 1024);

				//if nothing on line, or # comment, continue;
				if (buffer[0] == '#' || strlen(buffer) == 0)
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
				tokens[0].insert(0, filePath);
				tokens[1].erase(0, 1);
				//1 is name

				//determine what loading format to use
				if (tokens[0].find("8.0.mdl") != std::string::npos)
					LoadModel(tokens[1], tokens[0]);
				else if (tokens[0].find(".fbx") != std::string::npos)
					LoadModel_Fbx(tokens[1], tokens[0]);
				else if (tokens[0].find(".jdl") != std::string::npos)
					LoadModel_Ursine(tokens[1], tokens[0]);
				else if (tokens[0].find(".jlvl") != std::string::npos)
					LoadLevel_Ursine(tokens[1], tokens[0]);
			}

			input.close();

            std::vector<AnimationVertex> temp;
            temp.resize(6);

            temp[ 0 ].vPos = DirectX::XMFLOAT3(-0.5, -0.5, 0.5);
            temp[ 0 ].vUv = DirectX::XMFLOAT2(1, 1);
            temp[ 0 ].vNor = DirectX::XMFLOAT3(0, 0, 0);

            temp[ 1 ].vPos = DirectX::XMFLOAT3(0.5, -0.5, 0.5);
            temp[ 1 ].vUv = DirectX::XMFLOAT2(0, 1);
            temp[ 1 ].vNor = DirectX::XMFLOAT3(0, 0, 0);

            temp[ 2 ].vPos = DirectX::XMFLOAT3(0.5, 0.5, 0.5);
            temp[ 2 ].vUv = DirectX::XMFLOAT2(0, 0);
            temp[ 2 ].vNor = DirectX::XMFLOAT3(0, 0, 0);

            temp[ 3 ].vPos = DirectX::XMFLOAT3(0.5, 0.5, 0.5);
            temp[ 3 ].vUv = DirectX::XMFLOAT2(0, 0);
            temp[ 3 ].vNor = DirectX::XMFLOAT3(0, 0, 0);

            temp[ 4 ].vPos = DirectX::XMFLOAT3(-0.5, 0.5, 0.5);
            temp[ 4 ].vUv = DirectX::XMFLOAT2(1, 0);
            temp[ 4 ].vNor = DirectX::XMFLOAT3(0, 0, 0);

            temp[ 5 ].vPos = DirectX::XMFLOAT3(-0.5, -0.5, 0.5);
            temp[ 5 ].vUv = DirectX::XMFLOAT2(1, 1);
            temp[ 5 ].vNor = DirectX::XMFLOAT3(0, 0, 0);

            std::string name = "internalQuad";

            m_modelArray[ name ] = new ModelResource();
            auto *newMesh = new Mesh( );
            m_modelArray[ name ]->AddMesh(newMesh);

            D3D11_BUFFER_DESC vertexBufferDesc;
            D3D11_SUBRESOURCE_DATA vertexData;
            HRESULT result;

            //Set up the description of the static vertex buffer.
            vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
            vertexBufferDesc.ByteWidth = sizeof(AnimationVertex) * 6;
            vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            vertexBufferDesc.CPUAccessFlags = 0;
            vertexBufferDesc.MiscFlags = 0;
            vertexBufferDesc.StructureByteStride = 0;

            //Give the subresource structure a pointer to the vertex data.
            vertexData.pSysMem = &temp[ 0 ];
            vertexData.SysMemPitch = 0;
            vertexData.SysMemSlicePitch = 0;

            //Now create the vertex buffer.
            result = m_device->CreateBuffer(&vertexBufferDesc, &vertexData, &newMesh->GetVertexBuffer( ));
            UAssert(result == S_OK, "Failed to make vertex buffer!");
            newMesh->SetVertexCount(6);

            newMesh->SetIndexCount(6);
            auto &indexArray = newMesh->GetRawIndices( );

            for (unsigned x = 0; x < 6; ++x)
                indexArray[ x ] = x;

            D3D11_BUFFER_DESC indexBufferDesc;
            D3D11_SUBRESOURCE_DATA indexData;

            //Set up the description of the static index buffer.
            indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
            indexBufferDesc.ByteWidth = sizeof(unsigned) * newMesh->GetIndexCount( );
            indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
            indexBufferDesc.CPUAccessFlags = 0;
            indexBufferDesc.MiscFlags = 0;
            indexBufferDesc.StructureByteStride = 0;

            //Give the subresource structure a pointer to the index data.
            indexData.pSysMem = indexArray.data();
            indexData.SysMemPitch = 0;
            indexData.SysMemSlicePitch = 0;

            //Create the index buffer.
            result = m_device->CreateBuffer(&indexBufferDesc, &indexData, &newMesh->GetIndexBuffer( ));
            UAssert(result == S_OK, "Failed to make index buffer!");

            m_s2uTable[ name ] = m_modelCount;
            m_u2mTable[ m_modelCount ] = m_modelArray[ name ];
			++m_modelCount;
        }

        void ModelManager::Uninitialize()
        {
            for (auto &x : m_modelArray)
            {
                if (x.second == nullptr)
                    continue;

                delete x.second;
            }

            m_device = nullptr;
            m_deviceContext = nullptr;
        }

        void ModelManager::LoadModel(std::string name, std::string fileName)
        {
            UAssert(m_modelArray[ name ] == nullptr, "Model with name '%' has already been loaded (new source file '%s')", name.c_str(), fileName.c_str());

            std::ifstream input;
            std::vector<DiffuseTextureVertex> buffer;
            std::vector<AnimationVertex> realbuffer;

            input.open(fileName, std::ios::in | std::ios::binary);

            UAssert(input.is_open() == true, "Failed to open model '%s'", fileName.c_str());

            if (!input.is_open())
                return;

            /////////////////////////////////////////////////////////////////
            // ALLOCATE MODEL ///////////////////////////////////////////////
            m_modelArray[ name ] = new ModelResource();
            auto *newMesh = new Mesh();
            newMesh->SetName(name);
            m_modelArray[ name ]->AddMesh(newMesh);

            /////////////////////////////////////////////////////////////////
            // LOAD FILE INTO MEMORY ////////////////////////////////////////
            char size[ 32 ];
            input.getline(size, 32);
            unsigned vertCount = atoi(size);

            newMesh->SetVertexCount(vertCount);

            auto &newMeshVertData = newMesh->GetRawVertices( );

            UAssert(vertCount > 0, "Mesh '%s' has 0 vertices!", fileName.c_str());

            buffer.resize(vertCount);
            realbuffer.resize(vertCount);
            input.read((char*)&buffer[ 0 ], sizeof(DiffuseTextureVertex) * vertCount);

            for (uint x = 0; x < vertCount; ++x)
            {
                newMeshVertData[ x ] = Vec3(buffer[ x ].pos.x, buffer[ x ].pos.y, buffer[ x ].pos.z);
                realbuffer[ x ].vPos = DirectX::XMFLOAT3(buffer[ x ].pos.x, buffer[ x ].pos.y, buffer[ x ].pos.z);
                realbuffer[ x ].vNor = DirectX::XMFLOAT3(buffer[ x ].normal.x, buffer[ x ].normal.y, buffer[ x ].normal.z);
                realbuffer[ x ].vUv = buffer[ x ].UV;
                realbuffer[ x ].vBIdx[ 0 ] = 0;// = DirectX::XMINT4( 0, 0, 0, 0 );
                realbuffer[ x ].vBIdx[ 1 ] = 0;
                realbuffer[ x ].vBIdx[ 2 ] = 0;
                realbuffer[ x ].vBIdx[ 3 ] = 0;
                realbuffer[ x ].vBWeight = DirectX::XMFLOAT4(1, 0, 0, 0);
            }

            /////////////////////////////////////////////////////////////////
            // CREATE VERTEX BUFFER /////////////////////////////////////////
            D3D11_BUFFER_DESC vertexBufferDesc;
            D3D11_SUBRESOURCE_DATA vertexData;
            HRESULT result;

            //Set up the description of the static vertex buffer.
            vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
            vertexBufferDesc.ByteWidth = sizeof(AnimationVertex) * vertCount;
            vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            vertexBufferDesc.CPUAccessFlags = 0;
            vertexBufferDesc.MiscFlags = 0;
            vertexBufferDesc.StructureByteStride = 0;

            //Give the subresource structure a pointer to the vertex data.
            vertexData.pSysMem = &realbuffer[ 0 ];
            vertexData.SysMemPitch = 0;
            vertexData.SysMemSlicePitch = 0;

            //Now create the vertex buffer.
            result = m_device->CreateBuffer(&vertexBufferDesc, &vertexData, &newMesh->GetVertexBuffer());
            UAssert(result == S_OK, "Failed to make vertex buffer!");

            /////////////////////////////////////////////////////////////////
            // CREATE INDEX BUFFER //////////////////////////////////////////
            newMesh->SetIndexCount(vertCount);
            
            auto &indexArray = newMesh->GetRawIndices( );

            for (unsigned x = 0; x < vertCount; ++x)
                indexArray[ x ] = x;

            D3D11_BUFFER_DESC indexBufferDesc;
            D3D11_SUBRESOURCE_DATA indexData;

            //Set up the description of the static index buffer.
            indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
            indexBufferDesc.ByteWidth = sizeof(unsigned) * newMesh->GetIndexCount();
            indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
            indexBufferDesc.CPUAccessFlags = 0;
            indexBufferDesc.MiscFlags = 0;
            indexBufferDesc.StructureByteStride = 0;

            //Give the subresource structure a pointer to the index data.
            indexData.pSysMem = indexArray.data();
            indexData.SysMemPitch = 0;
            indexData.SysMemSlicePitch = 0;

            //Create the index buffer.
            result = m_device->CreateBuffer(&indexBufferDesc, &indexData, &newMesh->GetIndexBuffer( ));
            UAssert(result == S_OK, "Failed to make index buffer!");

            m_s2uTable[ name ] = m_modelCount;
            m_u2mTable[ m_modelCount++ ] = m_modelArray[ name ];
        }

		void ModelManager::LoadModel_Fbx(std::string name, std::string fileName)
		{
			UAssert(m_modelArray[name] == nullptr, "Model with name '%' has already been loaded (new source file '%s')", name.c_str(), fileName.c_str());
		
			std::vector<AnimationVertex> buffer;
			CFBXLoader fbx_model;
			if (false == fbx_model.LoadFBX(fileName.c_str()))
				return;

			ufmt_loader::ModelInfo ufmt_model = fbx_model.GetModelInfo();
			ufmt_loader::AnimInfo ufmt_anim = fbx_model.GetAnimInfo();
		
			/////////////////////////////////////////////////////////
			// GENERATING BONE DATA /////////////////////////////////

			// 1. load rig
			unsigned rigIndex = 0;
			if (ufmt_model.mboneCount > 0)
				rigIndex = AnimationBuilder::LoadBoneData(ufmt_model, name);

			// 2. load animation
			unsigned animationIndex = 0;
			if (ufmt_anim.animCount > 0)
				animationIndex = AnimationBuilder::LoadAnimation(ufmt_anim, name);

			/////////////////////////////////////////////////////////////////
			// CREATE VERTEX BUFFER /////////////////////////////////////////
			D3D11_BUFFER_DESC vertexBufferDesc;
			D3D11_SUBRESOURCE_DATA vertexData;
			HRESULT result;

            // create initial model
			m_modelArray[name] = new ModelResource();

            // for each mesh
			for (uint mesh_idx = 0; mesh_idx < ufmt_model.mmeshCount; ++mesh_idx)
			{
                // create a new mesh
                Mesh *newMesh = new Mesh( );
                newMesh->SetID(mesh_idx);


				/////////////////////////////////////////////////////////////////
				// ALLOCATE MODEL ///////////////////////////////////////////////				

				ufmt_loader::MeshInfo* currMesh = &ufmt_model.mMeshInfoVec[mesh_idx];

                newMesh->SetName(currMesh->name);

				uint vertCount = ufmt_model.mMeshInfoVec[mesh_idx].meshVtxInfoCount;
                newMesh->SetVertexCount(vertCount);
                auto &meshVertArray = newMesh->GetRawVertices( );

				//Set up the description of the static vertex buffer.
				vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
				vertexBufferDesc.ByteWidth = sizeof(AnimationVertex) * vertCount;
				vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
				vertexBufferDesc.CPUAccessFlags = 0;
				vertexBufferDesc.MiscFlags = 0;
				vertexBufferDesc.StructureByteStride = 0;

				//Give the subresource structure a pointer to the vertex data. - need layout_type to determine if static or skinned
				//can do this with skincount
				buffer.resize(vertCount);
				for (size_t i = 0; i < vertCount; ++i)
				{
                    // update raw verts for physics
                    meshVertArray[ i ] = Vec3(
                        currMesh->meshVtxInfos[ i ].pos.x,
                        currMesh->meshVtxInfos[ i ].pos.y,
                        currMesh->meshVtxInfos[ i ].pos.z
                    );

                    // Set actual buffer data
					buffer[i].vPos = DirectX::XMFLOAT3(
						currMesh->meshVtxInfos[i].pos.x,
						currMesh->meshVtxInfos[i].pos.y,
						currMesh->meshVtxInfos[i].pos.z
						);
					buffer[i].vNor = DirectX::XMFLOAT3(
						currMesh->meshVtxInfos[i].normal.x,
						currMesh->meshVtxInfos[i].normal.y,
						currMesh->meshVtxInfos[i].normal.z
						);
					buffer[i].vUv = DirectX::XMFLOAT2(
						currMesh->meshVtxInfos[i].uv.x,
						currMesh->meshVtxInfos[i].uv.y
						);

					if (ufmt_model.mboneCount > 0)
					{
						buffer[i].vBWeight.x = static_cast<float>(currMesh->meshVtxInfos[i].ctrlBlendWeights.x);
						buffer[i].vBWeight.y = static_cast<float>(currMesh->meshVtxInfos[i].ctrlBlendWeights.y);
						buffer[i].vBWeight.z = static_cast<float>(currMesh->meshVtxInfos[i].ctrlBlendWeights.z);
						buffer[i].vBWeight.w = static_cast<float>(currMesh->meshVtxInfos[i].ctrlBlendWeights.w);
						buffer[i].vBIdx[0] = static_cast<BYTE>(currMesh->meshVtxInfos[i].ctrlIndices.x);
						buffer[i].vBIdx[1] = static_cast<BYTE>(currMesh->meshVtxInfos[i].ctrlIndices.y);
						buffer[i].vBIdx[2] = static_cast<BYTE>(currMesh->meshVtxInfos[i].ctrlIndices.z);
						buffer[i].vBIdx[3] = static_cast<BYTE>(currMesh->meshVtxInfos[i].ctrlIndices.w);
					}
					else
					{
						buffer[i].vBWeight = DirectX::XMFLOAT4(0, 0, 0, 1);
						buffer[i].vBIdx[0] = static_cast<BYTE>(0);
						buffer[i].vBIdx[1] = static_cast<BYTE>(0);
						buffer[i].vBIdx[2] = static_cast<BYTE>(0);
						buffer[i].vBIdx[3] = static_cast<BYTE>(0);
					}
				}

				//Give the subresource structure a pointer to the vertex data.
				vertexData.pSysMem = &buffer[0];
				vertexData.SysMemPitch = 0;
				vertexData.SysMemSlicePitch = 0;

				//Now create the vertex buffer.
				result = m_device->CreateBuffer(&vertexBufferDesc, &vertexData, &newMesh->GetVertexBuffer());
				UAssert(result == S_OK, "Failed to make vertex buffer!");
                newMesh->SetVertexCount(vertCount);

				/////////////////////////////////////////////////////////////////
				// CREATE INDEX BUFFER //////////////////////////////////////////
				newMesh->SetIndexCount( currMesh->meshVtxInfoCount );

                auto &indexArray = newMesh->GetRawIndices();
				for (unsigned x = 0; x < newMesh->GetIndexCount(); ++x)
					indexArray[x] = x;// currMesh->indices[x];

				D3D11_BUFFER_DESC indexBufferDesc;
				D3D11_SUBRESOURCE_DATA indexData;

				//Set up the description of the static index buffer.
				indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
				indexBufferDesc.ByteWidth = sizeof(unsigned) * newMesh->GetIndexCount();
				indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
				indexBufferDesc.CPUAccessFlags = 0;
				indexBufferDesc.MiscFlags = 0;
				indexBufferDesc.StructureByteStride = 0;

				//Give the subresource structure a pointer to the index data.
				indexData.pSysMem = indexArray.data();
				indexData.SysMemPitch = 0;
				indexData.SysMemSlicePitch = 0;

				//Create the index buffer.
				result = m_device->CreateBuffer(&indexBufferDesc, &indexData, &newMesh->GetIndexBuffer());
				UAssert(result == S_OK, "Failed to make index buffer!");

				m_s2uTable[name] = m_modelCount;
				m_u2mTable[m_modelCount++] = m_modelArray[name];

                m_modelArray[ name ]->AddMesh(newMesh);
			}
		}

		void ModelManager::LoadModel_Ursine(std::string name, std::string fileName)
		{
			UAssert(m_modelArray[name] == nullptr, "Model with name '%' has already been loaded (new source file '%s')", name.c_str(), fileName.c_str());

			std::ifstream input;
			std::vector<AnimationVertex> buffer;

			HANDLE hFile_model = CreateFile(fileName.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
			ufmt_loader::ModelInfo ufmt_model;
			ufmt_loader::AnimInfo ufmt_anim;

			// Serialize in model and animation
			ufmt_model.SerializeIn(hFile_model);
		}

		void ModelManager::LoadLevel_Ursine(std::string name, std::string fileName)
		{
			UAssert(m_modelArray[name] == nullptr, "Model with name '%' has already been loaded (new source file '%s')", name.c_str(), fileName.c_str());

			std::ifstream input;
			std::vector<AnimationVertex> buffer;

			HANDLE hFile = CreateFile(fileName.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
			ufmt_loader::LevelInfo ufmt_lvl;

			// Serialize in model
			ufmt_lvl.SerializeIn(hFile);
            
            // We now have the transforms for the level.

            // For each mesh,

                // Apply inv transform to move from the global model space to the local space of that mesh
                
                // Save that mesh's transformation
		}

        ID3D11Buffer *ModelManager::GetModelVert(std::string name, unsigned index)
        {
            if (m_modelArray[ name ] == nullptr)
                return nullptr;
            return m_modelArray[ name ]->GetMesh(index)->GetVertexBuffer();
        }

        unsigned ModelManager::GetModelVertcount(std::string name, unsigned index)
        {
            if (m_modelArray[ name ] == nullptr)
                return -1;
            return m_modelArray[ name ]->GetMesh(index)->GetVertexCount( );
        }

		unsigned ModelManager::GetModelIndexcount(std::string name, unsigned index)
		{
			if (m_modelArray[name] == nullptr)
				return -1;
            return m_modelArray[ name ]->GetMesh(index)->GetIndexCount( );
		}

        void ModelManager::BindModel(std::string name, unsigned index)
        {


            ModelResource *model = m_modelArray[ name ];

            UAssert(model != nullptr, "Failed to bind model '%s'", name.c_str());

            //map mesh
            unsigned int strides = sizeof(AnimationVertex);
            unsigned int offset = 0;

            m_deviceContext->IASetVertexBuffers(0, 1, &model->GetMesh(index)->GetVertexBuffer(), &strides, &offset);
            m_deviceContext->IASetIndexBuffer(model->GetMesh(index)->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

            m_currentState = m_s2uTable[ name ];
        }

        void ModelManager::BindModel(unsigned ID, unsigned index)
        {


            ModelResource *model = m_u2mTable[ ID ];

            UAssert(model != nullptr, "Failed to bind model ID:%i", ID);

            //map mesh
            unsigned int strides = sizeof(AnimationVertex);
            unsigned int offset = 0;

            m_deviceContext->IASetVertexBuffers(0, 1, &model->GetMesh(index)->GetVertexBuffer(), &strides, &offset);
            m_deviceContext->IASetIndexBuffer(model->GetMesh(index)->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

            m_currentState = ID;
        }

        unsigned ModelManager::GetModelIDByName(std::string name)
        {
            return m_s2uTable[ name ];
        }

        ID3D11Buffer *ModelManager::GetModelVertByID(unsigned ID, unsigned index)
        {
            return m_u2mTable[ ID ]->GetMesh(index)->GetVertexBuffer( );
        }
        unsigned ModelManager::GetModelVertcountByID(unsigned ID, unsigned index)
        {
            return m_u2mTable[ ID ]->GetMesh(index)->GetVertexCount( );
        }

		unsigned ModelManager::GetModelIndexcountByID(unsigned ID, unsigned index)
		{
			return m_u2mTable[ID]->GetMesh(index)->GetIndexCount();
		}

        unsigned ModelManager::GetModelMeshCount(unsigned ID)
        {
            return m_u2mTable[ ID ]->GetMeshCount();
        }

        void ModelManager::Invalidate()
        {
            m_currentState = -1;
        }

    }
}