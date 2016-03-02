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
#include "CFBXLoader.h"
#include "DXErrorHandling.h"

#include "InternalResourceByteCode.h"

namespace ursine
{
    namespace graphics
    {
        ModelManager::ModelManager()
            : m_device( nullptr )
            , m_deviceContext( nullptr )
            , m_modelCount( INTERNAL_GEOMETRY_COUNT )
            , m_currentState( -1 )
            , m_modelCache( { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr } )
            , m_modelInfoCache( { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr } )
            , m_animeCount( 0 )
            , m_animeInfoCache( { } )
        {
        }

        void ModelManager::Initialize(ID3D11Device *device, ID3D11DeviceContext *context, std::string filePath)
        {
            m_device = device;
            m_deviceContext = context;

            m_modelCount = 0;
            m_currentState = -1;

            m_animeCount = 0;

            AnimationBuilder::InitializeStaticData();

            /////////////////////////////////////////////////////////////////////////////
            // GENERATING INTERNAL PS INDEX BUFFER
            {
                m_modelCache[ INTERNAL_POINT_INDICES ] = new ModelResource( );
                auto *newMesh = new Mesh( );
                m_modelCache[ INTERNAL_POINT_INDICES ]->AddMesh( newMesh );

                unsigned indices[ 1024 * 6 ];
                unsigned indexArray[ 6 ] = { 0, 1, 2, 1, 2, 3 };

                for (int x = 0; x < 1024 * 6; ++x)
                {
                    indices[ x ] = x;
                }

                D3D11_BUFFER_DESC indexBufferDesc;
                D3D11_SUBRESOURCE_DATA indexData;

                //Set up the description of the static index buffer.
                indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
                indexBufferDesc.ByteWidth = sizeof(unsigned) * 1024 * 6;
                indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
                indexBufferDesc.CPUAccessFlags = 0;
                indexBufferDesc.MiscFlags = 0;
                indexBufferDesc.StructureByteStride = 0;

                //Give the subresource structure a pointer to the index data.
                indexData.pSysMem = indices;
                indexData.SysMemPitch = 0;
                indexData.SysMemSlicePitch = 0;

                //Create the index buffer.
                HRESULT result = m_device->CreateBuffer(&indexBufferDesc, &indexData, &newMesh->GetIndexBuffer());
                UAssert(result == S_OK, "Failed to make index buffer!");
            }

            /////////////////////////////////////////////////////////////////////////////
            // GENERATING INTERNAL QUAD FOR RENDERING
            {
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

                m_modelCache[ INTERNAL_QUAD ] = new ModelResource( );
                auto *newMesh = new Mesh( );
                m_modelCache[ INTERNAL_QUAD ]->AddMesh(newMesh);

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
                indexData.pSysMem = indexArray.data( );
                indexData.SysMemPitch = 0;
                indexData.SysMemSlicePitch = 0;

                //Create the index buffer.
                result = m_device->CreateBuffer(&indexBufferDesc, &indexData, &newMesh->GetIndexBuffer( ));
                UAssert(result == S_OK, "Failed to make index buffer!");
            }

            /////////////////////////////////////////////////////////////////////////////
            // GENERATING INTERNAL CUBE
            {
                m_modelCache[ INTERNAL_CUBE ] = new ModelResource( );
                auto *newMesh = new Mesh( );
                m_modelCache[ INTERNAL_CUBE ]->AddMesh( newMesh );

                // load into modelInfo
                ufmt_loader::ModelInfo modelInfo;

                // load into model
              /*  InitializeModel(
                    &modelInfo, 
                    m_modelCache[ INTERNAL_CUBE ]
                );*/
            }

            /////////////////////////////////////////////////////////////////////////////
            // GENERATING INTERNAL SPHERE
            {
                m_modelCache[ INTERNAL_SPHERE ] = new ModelResource( );
                auto *newMesh = new Mesh( );
                m_modelCache[ INTERNAL_SPHERE ]->AddMesh(newMesh);

                graphics_resources::kSphereJdl;

                // somehow load into modelInfo
                ufmt_loader::ModelInfo modelInfo;

                // load into model
               // InitializeModel(&modelInfo, m_modelCache[ INTERNAL_SPHERE ]);
            }

            /////////////////////////////////////////////////////////////////////////////
            // GENERATING INTERNAL CONE
            {
                m_modelCache[ INTERNAL_CONE ] = new ModelResource( );
                auto *newMesh = new Mesh( );
                m_modelCache[ INTERNAL_CONE ]->AddMesh(newMesh);

                graphics_resources::kConeJdl;

                // somehow load into modelInfo
                ufmt_loader::ModelInfo modelInfo;

                // load into model
                // InitializeModel( &modelInfo, m_modelCache[ INTERNAL_CONE ] );
            }
        }

        void ModelManager::Uninitialize()
        {
            for (auto &x : m_modelCache)
            {
                delete x;
            }

            for (auto &x : m_animeArray)
            {
                if (x.second == nullptr)
                    continue;

                delete x.second;
            }

            m_device = nullptr;
            m_deviceContext = nullptr;
        }

        void ModelManager::InitializeModel(ufmt_loader::ModelInfo* modelInfo, ModelResource* modelresource)
        {
            // for each mesh
            for (uint mesh_idx = 0; mesh_idx < modelInfo->mmeshCount; ++mesh_idx)
            {
                // create a new mesh
                Mesh *newMesh = new Mesh();
                newMesh->SetID(mesh_idx);

                /////////////////////////////////////////////////////////////////
                // ALLOCATE MODEL ///////////////////////////////////////////////                

                ufmt_loader::MeshInfo* currMesh = &modelInfo->mMeshInfoVec[ mesh_idx ];

                newMesh->SetName(currMesh->name);

                uint vertCount = currMesh->meshVtxInfoCount;
                newMesh->SetVertexCount(vertCount);
                auto &meshVertArray = newMesh->GetRawVertices();

                //Give the subresource structure a pointer to the vertex data. - need layout_type to determine if static or skinned
                //can do this with skincount
                std::vector<AnimationVertex> &buffer = newMesh->GetRawModelData();
                buffer.resize(vertCount);
                for (size_t i = 0; i < vertCount; ++i)
                {
                    // update raw verts for physics
                    meshVertArray[ i ] = Vec3(
                        currMesh->meshVtxInfos[ i ].pos.x,
                        currMesh->meshVtxInfos[ i ].pos.y,
                        currMesh->meshVtxInfos[ i ].pos.z
                        );

                    // transform these points from their global model space into their local space
                    SVec4 tempPosition = SVec4(
                        currMesh->meshVtxInfos[ i ].pos.x,
                        currMesh->meshVtxInfos[ i ].pos.y,
                        currMesh->meshVtxInfos[ i ].pos.z,
                        1.0f
                        );

                    // Set data
                    buffer[ i ].vPos = DirectX::XMFLOAT3(
                        tempPosition.ToD3D().x,
                        tempPosition.ToD3D().y,
                        tempPosition.ToD3D().z
                        );

                    buffer[ i ].vNor = DirectX::XMFLOAT3(
                        currMesh->meshVtxInfos[ i ].normal.x,
                        currMesh->meshVtxInfos[ i ].normal.y,
                        currMesh->meshVtxInfos[ i ].normal.z
                        );
                    buffer[ i ].vUv = DirectX::XMFLOAT2(
                        currMesh->meshVtxInfos[ i ].uv.x,
                        currMesh->meshVtxInfos[ i ].uv.y
                        );

                    if (modelInfo->mboneCount > 0)
                    {
                        buffer[ i ].vBWeight.x = static_cast<float>(currMesh->meshVtxInfos[ i ].ctrlBlendWeights.x);
                        buffer[ i ].vBWeight.y = static_cast<float>(currMesh->meshVtxInfos[ i ].ctrlBlendWeights.y);
                        buffer[ i ].vBWeight.z = static_cast<float>(currMesh->meshVtxInfos[ i ].ctrlBlendWeights.z);
                        buffer[ i ].vBWeight.w = static_cast<float>(currMesh->meshVtxInfos[ i ].ctrlBlendWeights.w);
                        buffer[ i ].vBIdx[ 0 ] = static_cast<BYTE>(currMesh->meshVtxInfos[ i ].ctrlIndices.x);
                        buffer[ i ].vBIdx[ 1 ] = static_cast<BYTE>(currMesh->meshVtxInfos[ i ].ctrlIndices.y);
                        buffer[ i ].vBIdx[ 2 ] = static_cast<BYTE>(currMesh->meshVtxInfos[ i ].ctrlIndices.z);
                        buffer[ i ].vBIdx[ 3 ] = static_cast<BYTE>(currMesh->meshVtxInfos[ i ].ctrlIndices.w);
                    }
                    else
                    {
                        buffer[ i ].vBWeight = DirectX::XMFLOAT4(0, 0, 0, 1);
                        buffer[ i ].vBIdx[ 0 ] = static_cast<BYTE>(0);
                        buffer[ i ].vBIdx[ 1 ] = static_cast<BYTE>(0);
                        buffer[ i ].vBIdx[ 2 ] = static_cast<BYTE>(0);
                        buffer[ i ].vBIdx[ 3 ] = static_cast<BYTE>(0);
                    }
                }

                newMesh->SetVertexCount(vertCount);

                /////////////////////////////////////////////////////////////////
                // CREATE INDEX BUFFER //////////////////////////////////////////
                newMesh->SetIndexCount(currMesh->meshVtxIdxCount);

                auto &indexArray = newMesh->GetRawIndices();
                for (unsigned x = 0; x < newMesh->GetIndexCount(); ++x)
                    indexArray[ x ] = currMesh->meshVtxIndices[ x ];

                modelresource->AddMesh(newMesh);
            }

            for (auto &x : modelInfo->mMeshLvVec)
                modelresource->AddMesh2Tree(x);
            
            for (auto &x : modelInfo->mRigLvVec)
                modelresource->AddRig2Tree(x);
        }

        GfxHND ModelManager::CreateModel(ufmt_loader::ModelInfo *modelInfo)
        {
            // if this model already exists and we are updating it, release the prior resource, keep old ID
            size_t internalID;

            GfxHND handle;
            _RESOURCEHND *id = HND_RSRCE(handle);

            internalID = m_modelCache.size();
            m_modelCache.push_back( new ModelResource( ) );
            m_modelInfoCache.push_back( new ufmt_loader::ModelInfo( ) );
            *(m_modelInfoCache[ internalID ]) = *modelInfo;

            /////////////////////////////////////////////////////////
            // GENERATING BONE DATA /////////////////////////////////
            /////////////////////////////////////////////////////////
            unsigned rigIndex = 0;
            if (modelInfo->mboneCount > 0)
                rigIndex = AnimationBuilder::LoadBoneData(*modelInfo, modelInfo->name);

            // load it up into CPU memory
            InitializeModel(
                modelInfo,
                m_modelCache[ internalID ]
            );

            // initialize handle
            id->ID_ = SANITY_RESOURCE;
            id->Type_ = ID_MODEL;
            id->Index_ = static_cast<unsigned>(internalID);

            return handle;
        }

        void ModelManager::DestroyModel(GfxHND &handle)
        {
            int id;

            _RESOURCEHND *hnd = HND_RSRCE(handle);
            UAssert(hnd->ID_ == SANITY_RESOURCE, "Attempted to get model with invalid handle!");
            UAssert(hnd->Type_ == ID_MODEL, "Attempted to get model with handle of invalid type!");

            id = hnd->Index_;

            ModelResource *model = m_modelCache[ id ];
            ufmt_loader::ModelInfo *modelInfo = m_modelInfoCache[ id ];

            if (model->GetIsLoaded())
                unloadModelFromGPU(model);

            delete model;
            delete modelInfo;

            handle = 0;
        }

        void ModelManager::LoadModel(GfxHND handle)
        {
            int id;
            _RESOURCEHND *hnd = HND_RSRCE(handle);
            UAssert(hnd->ID_ == SANITY_RESOURCE, "Attempted to get model with invalid handle!");
            UAssert(hnd->Type_ == ID_MODEL, "Attempted to get model with handle of invalid type!");

            id = hnd->Index_;

            if (m_modelCache[ id ]->HasNoReferences())
                loadModelToGPU(m_modelCache[ id ]);

            m_modelCache[ id ]->IncrementReference();
        }

        void ModelManager::UnloadModel(GfxHND handle)
        {
            int id;
            _RESOURCEHND *hnd = HND_RSRCE(handle);
            UAssert(hnd->ID_ == SANITY_RESOURCE, "Attempted to get model with invalid handle!");
            UAssert(hnd->Type_ == ID_MODEL, "Attempted to get model with handle of invalid type!");

            id = hnd->Index_;

            m_modelCache[ id ]->DecrementReference();

            if (m_modelCache[ id ]->HasNoReferences())
                unloadModelFromGPU(m_modelCache[ id ]);
        }

        ufmt_loader::ModelInfo *ModelManager::GetModelInfo(GfxHND handle)
        {
            int id;
            _RESOURCEHND *hnd = HND_RSRCE(handle);
            UAssert(hnd->ID_ == SANITY_RESOURCE, "Attempted to get model with invalid handle!");
            UAssert(hnd->Type_ == ID_MODEL, "Attempted to get model with handle of invalid type!");

            id = hnd->Index_;

            return m_modelInfoCache[ id ];
        }

        void ModelManager::BindModel(unsigned ID, unsigned index, bool indexOnly)
        {
            ModelResource *model = m_modelCache[ ID ];

            UAssert(model != nullptr, "Failed to bind model ID:%i", ID);

            if (ID == INTERNAL_POINT_INDICES)
            {
                m_deviceContext->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
                m_deviceContext->IASetIndexBuffer(model->GetMesh(index)->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

                m_currentState = ID;
                return;
            }

            //map mesh
            unsigned int strides = sizeof(AnimationVertex);
            unsigned int offset = 0;

            m_deviceContext->IASetVertexBuffers(0, 1, &model->GetMesh(index)->GetVertexBuffer(), &strides, &offset);
            m_deviceContext->IASetIndexBuffer(model->GetMesh(index)->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

            m_currentState = ID;
        }

        unsigned ModelManager::GetModelIndexcountByID(unsigned ID, unsigned index)
        {
            return m_modelCache[ ID ]->GetMesh( index )->GetIndexCount( );
        }

        unsigned ModelManager::GetModelMeshCount(unsigned ID)
        {
            return m_modelCache[ ID ]->GetMeshCount();
        }

        void ModelManager::Invalidate()
        {
            m_currentState = -1;
        }

        ModelResource *ModelManager::GetModel(const unsigned ID)
        {
            return m_modelCache[ ID ];
        }

        void ModelManager::loadModelToGPU(ModelResource *model)
        {
            auto &meshBuffer = model->GetMeshArray();

            for (auto &mesh : meshBuffer)
            {
                D3D11_BUFFER_DESC vertexBufferDesc;
                D3D11_SUBRESOURCE_DATA vertexData;
                HRESULT result;

                //Set up the description of the static vertex buffer.
                vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
                vertexBufferDesc.ByteWidth = sizeof(AnimationVertex) * mesh->GetVertexCount();
                vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
                vertexBufferDesc.CPUAccessFlags = 0;
                vertexBufferDesc.MiscFlags = 0;
                vertexBufferDesc.StructureByteStride = 0;

                //Give the subresource structure a pointer to the vertex data.
                vertexData.pSysMem = mesh->GetRawModelData().data();
                vertexData.SysMemPitch = 0;
                vertexData.SysMemSlicePitch = 0;

                //Now create the vertex buffer.
                result = m_device->CreateBuffer(&vertexBufferDesc, &vertexData, &mesh->GetVertexBuffer());
                UAssert(result == S_OK, "Failed to make vertex buffer! (%s)", DXCore::GetDXErrorMessage(result));

                D3D11_BUFFER_DESC indexBufferDesc;
                D3D11_SUBRESOURCE_DATA indexData;

                //Set up the description of the static index buffer.
                indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
                indexBufferDesc.ByteWidth = sizeof(unsigned) * mesh->GetIndexCount();
                indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
                indexBufferDesc.CPUAccessFlags = 0;
                indexBufferDesc.MiscFlags = 0;
                indexBufferDesc.StructureByteStride = 0;

                //Give the subresource structure a pointer to the index data.
                indexData.pSysMem = mesh->GetRawIndices().data();
                indexData.SysMemPitch = 0;
                indexData.SysMemSlicePitch = 0;

                //Create the index buffer.
                result = m_device->CreateBuffer(&indexBufferDesc, &indexData, &mesh->GetIndexBuffer());
                UAssert(result == S_OK, "Failed to make index buffer! (%s)", DXCore::GetDXErrorMessage(result));
            }
        }

        void ModelManager::unloadModelFromGPU(ModelResource *model)
        {
            auto &meshBuffer = model->GetMeshArray();

            for (auto &mesh : meshBuffer)
            {
                RELEASE_RESOURCE(mesh->GetIndexBuffer());
                RELEASE_RESOURCE(mesh->GetVertexBuffer());
            }
        }

        // creating a anime 
        GfxHND ModelManager::CreateAnimation(ufmt_loader::AnimInfo *animeInfo)
        {
            // if this model already exists and we are updating it, release the prior resource, keep old ID
            size_t internalID;

            GfxHND handle;
            _RESOURCEHND *id = HND_RSRCE(handle);

            internalID = m_animeInfoCache.size();
            m_animeInfoCache.push_back( new ufmt_loader::AnimInfo() );
            *(m_animeInfoCache[internalID]) = *animeInfo;

            // initialize handle
            id->ID_ = SANITY_RESOURCE;
            id->Type_ = ID_ANIMATION;
            id->Index_ = static_cast<unsigned>(internalID);

            return handle;
        }

        void ModelManager::DestroyAnimation(GfxHND &handle)
        {
            int id;

            _RESOURCEHND *hnd = HND_RSRCE(handle);
            UAssert(hnd->ID_ == SANITY_RESOURCE, "Attempted to get animation with invalid handle!");
            UAssert(hnd->Type_ == ID_ANIMATION, "Attempted to get animation with handle of invalid type!");

            id = hnd->Index_;

            ufmt_loader::AnimInfo *animeInfo = m_animeInfoCache[ id ];
            
            //delete[  ] anime->m_binaryData;
            //model->m_binarySize = 0;
            //model->m_referenceCount = 0;
            //
            //if (model->GetIsLoaded())
            //    unloadModelFromGPU(model);
            
            delete animeInfo;

            handle = 0;
        }

        ufmt_loader::AnimInfo *ModelManager::GeAnimeInfo(GfxHND handle)
        {
            int id;

            _RESOURCEHND *hnd = HND_RSRCE(handle);
            UAssert(hnd->ID_ == SANITY_RESOURCE, "Attempted to get animation with invalid handle!");
            UAssert(hnd->Type_ == ID_ANIMATION, "Attempted to get animation with handle of invalid type!");

            id = hnd->Index_;

            return m_animeInfoCache[ id ];
        }
    }
}