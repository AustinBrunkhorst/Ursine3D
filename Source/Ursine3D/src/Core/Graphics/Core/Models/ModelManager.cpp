﻿/* ----------------------------------------------------------------------------
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
#include "AnimationBuilder.h"
#include "DXErrorHandling.h"

#include "InternalResourceByteCode.h"

#include <d3d11.h>

namespace ursine
{
    namespace graphics
    {
        ModelManager::ModelManager(void)
            : m_device( nullptr )
            , m_deviceContext( nullptr )
            , m_nextModelID( INTERNAL_GEOMETRY_COUNT )
            , m_currentState( -1 )
            , m_modelCache( )
            , m_modelInfoCache( )
            , m_nextAnimationID( 0 )
            , m_animeInfoCache( { } )
        {
        }

        void ModelManager::Initialize(ID3D11Device *device, ID3D11DeviceContext *context, std::string filePath)
        {
            m_device = device;
            m_deviceContext = context;

            m_currentState = -1;

            AnimationBuilder::InitializeStaticData( );

            /////////////////////////////////////////////////////////////////////////////
            // GENERATING INTERNAL PS INDEX BUFFER
            {
                auto *newMesh = new Mesh( );

                auto &internalPoint = m_modelCache[ INTERNAL_POINT_INDICES ];

                internalPoint.AddMesh( newMesh );
                internalPoint.SetIsLoaded( true );

                unsigned indices[ 1024 * 6 ];
                unsigned indexArray[ 6 ] = { 0, 1, 2, 1, 2, 3 };

                for (int x = 0; x < 1024 * 6; ++x)
                    indices[ x ] = x;

                D3D11_BUFFER_DESC indexBufferDesc;
                D3D11_SUBRESOURCE_DATA indexData;

                // set up the description of the static index buffer
                indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
                indexBufferDesc.ByteWidth = sizeof(unsigned) * 1024 * 6;
                indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
                indexBufferDesc.CPUAccessFlags = 0;
                indexBufferDesc.MiscFlags = 0;
                indexBufferDesc.StructureByteStride = 0;

                // give the subresource structure a pointer to the index data
                indexData.pSysMem = indices;
                indexData.SysMemPitch = 0;
                indexData.SysMemSlicePitch = 0;

                // create the index buffer
                auto result = m_device->CreateBuffer( &indexBufferDesc, &indexData, &newMesh->GetIndexBuffer( ) );

                UAssert( result == S_OK, 
                    "Failed to make index buffer!"
                );
            }

            /////////////////////////////////////////////////////////////////////////////
            // GENERATING INTERNAL QUAD FOR RENDERING
            {
                std::vector<AnimationVertex> temp;
                temp.resize( 6 );

                temp[ 0 ].vPos = DirectX::XMFLOAT3( -0.5, -0.5, 0.5 );
                temp[ 0 ].vUv = DirectX::XMFLOAT2( 1, 1 );
                temp[ 0 ].vNor = DirectX::XMFLOAT3( 0, 0, 0 );

                temp[ 1 ].vPos = DirectX::XMFLOAT3( 0.5, -0.5, 0.5 );
                temp[ 1 ].vUv = DirectX::XMFLOAT2( 0, 1 );
                temp[ 1 ].vNor = DirectX::XMFLOAT3( 0, 0, 0 );

                temp[ 2 ].vPos = DirectX::XMFLOAT3( 0.5, 0.5, 0.5 );
                temp[ 2 ].vUv = DirectX::XMFLOAT2( 0, 0 );
                temp[ 2 ].vNor = DirectX::XMFLOAT3( 0, 0, 0 );

                temp[ 3 ].vPos = DirectX::XMFLOAT3( 0.5, 0.5, 0.5 );
                temp[ 3 ].vUv = DirectX::XMFLOAT2( 0, 0 );
                temp[ 3 ].vNor = DirectX::XMFLOAT3( 0, 0, 0 );

                temp[ 4 ].vPos = DirectX::XMFLOAT3( -0.5, 0.5, 0.5 );
                temp[ 4 ].vUv = DirectX::XMFLOAT2( 1, 0 );
                temp[ 4 ].vNor = DirectX::XMFLOAT3( 0, 0, 0 );

                temp[ 5 ].vPos = DirectX::XMFLOAT3( -0.5, -0.5, 0.5 );
                temp[ 5 ].vUv = DirectX::XMFLOAT2( 1, 1 );
                temp[ 5 ].vNor = DirectX::XMFLOAT3( 0, 0, 0 );

                auto *newMesh = new Mesh( );

                auto &internalQuad = m_modelCache[ INTERNAL_QUAD ];

                internalQuad.AddMesh( newMesh );

                D3D11_BUFFER_DESC vertexBufferDesc;
                D3D11_SUBRESOURCE_DATA vertexData;
                HRESULT result;

                // set up the description of the static vertex buffer
                vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
                vertexBufferDesc.ByteWidth = sizeof(AnimationVertex) * 6;
                vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
                vertexBufferDesc.CPUAccessFlags = 0;
                vertexBufferDesc.MiscFlags = 0;
                vertexBufferDesc.StructureByteStride = 0;

                // give the subresource structure a pointer to the vertex data

                vertexData.pSysMem = &temp[ 0 ];

                vertexData.SysMemPitch = 0;
                vertexData.SysMemSlicePitch = 0;

                // now create the vertex buffer
                result = m_device->CreateBuffer( &vertexBufferDesc, &vertexData, &newMesh->GetVertexBuffer( ) );

                UAssert( result == S_OK, 
                    "Failed to make vertex buffer!"
                );

                newMesh->SetVertexCount( 6 );
                //newMesh->SetNormalCount( 6 );
                //newMesh->SetBinormalCount( 6 );
                //newMesh->SetTangentCount( 6 );

                newMesh->SetIndexCount( 6 );
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
                result = m_device->CreateBuffer( &indexBufferDesc, &indexData, &newMesh->GetIndexBuffer( ) );
                
                UAssert( result == S_OK, 
                    "Failed to make index buffer!"
                );

                internalQuad.SetIsLoaded( true );
            }

            ///////////////////////////////////////////////////////////////////////////////
            //// GENERATING INTERNAL CUBE
            {
                resources::ResourceReader reader( graphics_resources::kCubeJdl, 2676 );

                // load into modelInfo
                ufmt_loader::ModelInfo modelInfo;

                modelInfo.Read( reader );

                auto &internalCube = m_modelCache[ INTERNAL_CUBE ];

                InitializeModel( modelInfo, internalCube );

                loadModelToGPU( internalCube );
            }

            /////////////////////////////////////////////////////////////////////////////
            // GENERATING INTERNAL SPHERE
            {
                resources::ResourceReader reader( graphics_resources::kSphereJdl, 40646 );

                // load into modelInfo
                ufmt_loader::ModelInfo modelInfo;

                modelInfo.Read( reader );

                auto &internalSphere = m_modelCache[ INTERNAL_SPHERE ];

                InitializeModel( modelInfo, internalSphere );

                loadModelToGPU( internalSphere );
            }

            /////////////////////////////////////////////////////////////////////////////
            // GENERATING INTERNAL CONE
            {
                resources::ResourceReader reader( graphics_resources::kConeJdl, 5228 );

                // load into modelInfo
                ufmt_loader::ModelInfo modelInfo;

                modelInfo.Read( reader );

                auto &internalCone = m_modelCache[ INTERNAL_CONE ];

                InitializeModel( modelInfo, internalCone );

                loadModelToGPU( internalCone );
            }
        }

        void ModelManager::Uninitialize(void)
        {
            m_device = nullptr;
            m_deviceContext = nullptr;
        }

        bool ModelManager::CheckModelExistence(const std::string &modelName)
        {
            for (auto &iter : m_modelInfoCache)
            {
                if (iter.second.name == modelName)
                    return true;
            }
            return false;
        }

        GfxHND ModelManager::CreateModel(const ufmt_loader::ModelInfo &modelInfo)
        {
            GfxHND handle;

            auto *hnd = HND_RSRCE( handle );

            auto internalID = m_nextModelID++;

            auto &cachedModel = 
                m_modelCache.emplace( internalID, ModelResource { } ).first->second;

            m_modelInfoCache.emplace( internalID, modelInfo );

            // set in lookup table
            m_modelInfoTable.emplace( modelInfo.name, &m_modelInfoCache[ internalID ] );

            /////////////////////////////////////////////////////////
            // GENERATING BONE DATA /////////////////////////////////
            /////////////////////////////////////////////////////////

            unsigned rigIndex = 0;

            if (modelInfo.mboneCount > 0)
                rigIndex = AnimationBuilder::LoadBoneData( modelInfo, modelInfo.name );

            // load it up into CPU memory
            InitializeModel( modelInfo, cachedModel );

            // initialize handle
            hnd->ID_ = SANITY_RESOURCE;
            hnd->Type_ = ID_MODEL;
            hnd->Index_ = internalID;

            return handle;
        }

        void ModelManager::DestroyModel(GfxHND &handle)
        {
            auto *hnd = HND_RSRCE( handle );

            UAssert( hnd->ID_ == SANITY_RESOURCE, 
                "Attempted to get model with invalid handle!"
            );

            UAssert( hnd->Type_ == ID_MODEL, 
                "Attempted to get model with handle of invalid type!"
            );

            auto search = m_modelCache.find( hnd->Index_ );

            UAssert( search != m_modelCache.end( ),
                "Failed to destroy model.\nID: %i",
                hnd->Index_
            );

            auto &model = search->second;

            if (model.GetIsLoaded( ))
                unloadModelFromGPU( model );

            m_modelCache.erase( search );

            handle = 0;
        }

        void ModelManager::LoadModel(GfxHND handle)
        {
            auto *hnd = HND_RSRCE( handle );

            UAssert( hnd->ID_ == SANITY_RESOURCE, 
                "Attempted to get model with invalid handle!"
            );

            UAssert( hnd->Type_ == ID_MODEL, 
                "Attempted to get model with handle of invalid type!"
            );

            auto search = m_modelCache.find( hnd->Index_ );

            UAssert( search != m_modelCache.end( ),
                "Failed to load model.\nID: %i",
                hnd->Index_
            );

            auto &model = search->second;

            if (model.HasNoReferences( ))
                loadModelToGPU( model );

            model.IncrementReference( );
        }

        void ModelManager::UnloadModel(GfxHND handle)
        {
            URSINE_TODO( "Fix this." );
            return;
            auto *hnd = HND_RSRCE( handle );

            if (handle != 0)
            {
                UAssert( hnd->ID_ == SANITY_RESOURCE, 
                    "Attempted to get model with invalid handle!" 
                );

                UAssert( hnd->Type_ == ID_MODEL,
                    "Attempted to get model with handle of invalid type!" 
                );
            }

            if (hnd->Index_ < INTERNAL_GEOMETRY_COUNT)
                return;

            auto search = m_modelCache.find( hnd->Index_ );

            UAssert( search != m_modelCache.end( ),
                "Failed to unload model.\nID: %i",
                hnd->Index_
            );

            auto &model = search->second;

            model.DecrementReference( );

            if (model.HasNoReferences( ))
                unloadModelFromGPU( model );
        }

        void ModelManager::BindModel(unsigned ID, unsigned index, bool indexOnly)
        {
            auto search = m_modelCache.find( ID );

            UAssert( search != m_modelCache.end( ),
                "Failed to bind model.\nID: %i",
                ID
            );

            auto &model = search->second;

            UAssert( model.GetIsLoaded( ) == true, 
                "Attempted to bind model %i, but it isn't loaded on the GPU!", 
                ID
            );

            if (ID == INTERNAL_POINT_INDICES)
            {
                m_deviceContext->IASetVertexBuffers( 0, 0, nullptr, nullptr, nullptr );
                m_deviceContext->IASetIndexBuffer( model.GetMesh( index )->GetIndexBuffer( ), DXGI_FORMAT_R32_UINT, 0 );

                m_currentState = ID;
                return;
            }

            // map mesh
            static const unsigned strides = sizeof( AnimationVertex );
            unsigned int offset = 0;

            m_deviceContext->IASetVertexBuffers( 0, 1, &model.GetMesh( index )->GetVertexBuffer( ), &strides, &offset );
            m_deviceContext->IASetIndexBuffer( model.GetMesh( index )->GetIndexBuffer( ), DXGI_FORMAT_R32_UINT, 0 );

            m_currentState = ID;
        }

        unsigned ModelManager::GetModelIndexcountByID(unsigned ID, unsigned index)
        {
            return m_modelCache[ ID ].GetMesh( index )->GetIndexCount( );
        }

        unsigned ModelManager::GetModelMeshCount(unsigned ID)
        {
            return m_modelCache[ ID ].GetMeshCount( );
        }

        void ModelManager::Invalidate(void)
        {
            m_currentState = -1;
        }

        // animation
        bool ModelManager::CheckAnimExistence(const std::string &animeName)
        {
            for (auto &iter : m_animeInfoCache)
            {
                if (iter.second.name == animeName)
                    return true;
            }
            return false;
        }

        GfxHND ModelManager::CreateAnimation(const ufmt_loader::AnimInfo &animeInfo)
        {
            GfxHND handle;

            auto *hnd = HND_RSRCE( handle );

            auto internalID = m_nextAnimationID++;

            m_animeInfoCache.emplace( internalID, animeInfo );

            // initialize handle
            hnd->ID_ = SANITY_RESOURCE;
            hnd->Type_ = ID_ANIMATION;
            hnd->Index_ = internalID;

            return handle;
        }

        void ModelManager::DestroyAnimation(GfxHND &handle)
        {
            auto *hnd = HND_RSRCE( handle );

            UAssert( hnd->ID_ == SANITY_RESOURCE, 
                "Attempted to get animation with invalid handle!"
            );

            UAssert( hnd->Type_ == ID_ANIMATION, 
                "Attempted to get animation with handle of invalid type!"
            );

            m_animeInfoCache.erase( hnd->Index_ );

            handle = 0;
        }

        ufmt_loader::ModelInfo *ModelManager::GetModelInfo(GfxHND handle)
        {
            auto *hnd = HND_RSRCE( handle );

            UAssert( hnd->ID_ == SANITY_RESOURCE, 
                "Attempted to get model with invalid handle!"
            );

            UAssert( hnd->Type_ == ID_MODEL, 
                "Attempted to get model with handle of invalid type!"
            );

            auto search = m_modelInfoCache.find( hnd->Index_ );

            return search == m_modelInfoCache.end( ) ? nullptr : &search->second;
        }

        ufmt_loader::AnimInfo *ModelManager::GeAnimeInfo(GfxHND handle)
        {
            auto *hnd = HND_RSRCE( handle );

            UAssert( hnd->ID_ == SANITY_RESOURCE, 
                "Attempted to get animation with invalid handle!"
            );

            UAssert( hnd->Type_ == ID_ANIMATION, 
                "Attempted to get animation with handle of invalid type!"
            );

            auto search = m_animeInfoCache.find( hnd->Index_ );

            return search == m_animeInfoCache.end( ) ? nullptr : &search->second;
        }

        ModelResource *ModelManager::GetModel(const unsigned ID)
        {
            return &m_modelCache[ ID ];
        }

        ufmt_loader::ModelInfo* ModelManager::GetModelInfoByName(const std::string& name)
        {
            auto modelInfo = m_modelInfoTable.find( name );

            UAssert( modelInfo != m_modelInfoTable.end( ), "Failed to find model info with name '%s'!", name.c_str( ) );

            return modelInfo->second;
        }

        // private methods
        void ModelManager::InitializeModel(const ufmt_loader::ModelInfo &modelInfo, ModelResource &modelResource)
        {
            // for each mesh
            for (uint mesh_idx = 0; mesh_idx < modelInfo.mmeshCount; ++mesh_idx)
            {
                // create a new mesh
                Mesh *newMesh = new Mesh( );
                newMesh->SetID( mesh_idx );

                /////////////////////////////////////////////////////////////////
                // ALLOCATE MODEL ///////////////////////////////////////////////                

                ufmt_loader::MeshInfo currMesh = modelInfo.mMeshInfoVec[ mesh_idx ];

                newMesh->SetName( currMesh.name );

                uint vertCount = currMesh.meshVtxInfoCount;
                newMesh->SetVertexCount( vertCount );
                //newMesh->SetNormalCount( vertCount );
                //newMesh->SetBinormalCount( vertCount );
                //newMesh->SetTangentCount( vertCount );
                auto &meshVertArray = newMesh->GetRawVertices( );
                //auto &meshNormArray = newMesh->GetRawNormals( );
                //auto &meshBinormArray = newMesh->GetRawBinormals();
                //auto &meshTangentArray = newMesh->GetRawTangent();

                //Give the subresource structure a pointer to the vertex data. - need layout_type to determine if static or skinned
                //can do this with skincount
                std::vector<AnimationVertex> &buffer = newMesh->GetRawModelData( );
                buffer.resize( vertCount );
                for (size_t i = 0; i < vertCount; ++i)
                {
                    // update raw verts for physics
                    meshVertArray[ i ] = Vec3(
                        currMesh.meshVtxInfos[ i ].pos.x,
                        currMesh.meshVtxInfos[ i ].pos.y,
                        currMesh.meshVtxInfos[ i ].pos.z
                    );

                    //meshNormArray[i] = Vec3(
                    //    currMesh.meshVtxInfos[i].normal.x,
                    //    currMesh.meshVtxInfos[i].normal.y,
                    //    currMesh.meshVtxInfos[i].normal.z
                    //    );
                    //
                    //meshBinormArray[i] = Vec3(
                    //    currMesh.meshVtxInfos[i].binormal.x,
                    //    currMesh.meshVtxInfos[i].binormal.y,
                    //    currMesh.meshVtxInfos[i].binormal.z
                    //    );
                    //
                    //meshTangentArray[i] = Vec3(
                    //    currMesh.meshVtxInfos[i].tangent.x,
                    //    currMesh.meshVtxInfos[i].tangent.y,
                    //    currMesh.meshVtxInfos[i].tangent.z
                    //    );

                    // transform these points from their global model space into their local space
                    SVec4 tempPosition = SVec4(
                        currMesh.meshVtxInfos[ i ].pos.x,
                        currMesh.meshVtxInfos[ i ].pos.y,
                        currMesh.meshVtxInfos[ i ].pos.z,
                        1.0f
                    );

                    // Set data
                    buffer[ i ].vPos = DirectX::XMFLOAT3(
                        tempPosition.ToD3D( ).x,
                        tempPosition.ToD3D( ).y,
                        tempPosition.ToD3D( ).z
                    );

                    buffer[ i ].vNor = DirectX::XMFLOAT3(
                        currMesh.meshVtxInfos[ i ].normal.x,
                        currMesh.meshVtxInfos[ i ].normal.y,
                        currMesh.meshVtxInfos[ i ].normal.z
                    );

                    buffer[ i ].vTan = DirectX::XMFLOAT3(
                        currMesh.meshVtxInfos[ i ].tangent.x,
                        currMesh.meshVtxInfos[ i ].tangent.y,
                        currMesh.meshVtxInfos[ i ].tangent.z
                    );

                    buffer[ i ].vUv = DirectX::XMFLOAT2(
                        currMesh.meshVtxInfos[ i ].uv.x,
                        currMesh.meshVtxInfos[ i ].uv.y
                    );

                    if (modelInfo.mboneCount > 0)
                    {
                        buffer[ i ].vBWeight.x = static_cast<float>( currMesh.meshVtxInfos[ i ].ctrlBlendWeights.x );
                        buffer[ i ].vBWeight.y = static_cast<float>( currMesh.meshVtxInfos[ i ].ctrlBlendWeights.y );
                        buffer[ i ].vBWeight.z = static_cast<float>( currMesh.meshVtxInfos[ i ].ctrlBlendWeights.z );
                        buffer[ i ].vBWeight.w = static_cast<float>( currMesh.meshVtxInfos[ i ].ctrlBlendWeights.w );
                        buffer[ i ].vBIdx[ 0 ] = static_cast<BYTE>( currMesh.meshVtxInfos[ i ].ctrlIndices.x );
                        buffer[ i ].vBIdx[ 1 ] = static_cast<BYTE>( currMesh.meshVtxInfos[ i ].ctrlIndices.y );
                        buffer[ i ].vBIdx[ 2 ] = static_cast<BYTE>( currMesh.meshVtxInfos[ i ].ctrlIndices.z );
                        buffer[ i ].vBIdx[ 3 ] = static_cast<BYTE>( currMesh.meshVtxInfos[ i ].ctrlIndices.w );
                    }
                    else
                    {
                        buffer[ i ].vBWeight = DirectX::XMFLOAT4( 0, 0, 0, 1 );
                        buffer[ i ].vBIdx[ 0 ] = static_cast<BYTE>( 0 );
                        buffer[ i ].vBIdx[ 1 ] = static_cast<BYTE>( 0 );
                        buffer[ i ].vBIdx[ 2 ] = static_cast<BYTE>( 0 );
                        buffer[ i ].vBIdx[ 3 ] = static_cast<BYTE>( 0 );
                    }
                }

                newMesh->SetVertexCount( vertCount );
                //newMesh->SetNormalCount( vertCount );
                //newMesh->SetBinormalCount( vertCount );
                //newMesh->SetTangentCount( vertCount );

                /////////////////////////////////////////////////////////////////
                // CREATE INDEX BUFFER //////////////////////////////////////////
                newMesh->SetIndexCount( currMesh.meshVtxIdxCount );

                auto &indexArray = newMesh->GetRawIndices( );
                for (unsigned x = 0; x < newMesh->GetIndexCount( ); ++x)
                    indexArray[ x ] = currMesh.meshVtxIndices[ x ];

                modelResource.AddMesh( newMesh );
            }
        }

        void ModelManager::loadModelToGPU(ModelResource &model)
        {
            model.SetIsLoaded( true );

            auto &meshBuffer = model.GetMeshArray( );

            for (auto &mesh : meshBuffer)
            {
                D3D11_BUFFER_DESC vertexBufferDesc;
                D3D11_SUBRESOURCE_DATA vertexData;
                HRESULT result;

                //Set up the description of the static vertex buffer.
                vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
                vertexBufferDesc.ByteWidth = sizeof(AnimationVertex) * mesh->GetVertexCount( );
                vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
                vertexBufferDesc.CPUAccessFlags = 0;
                vertexBufferDesc.MiscFlags = 0;
                vertexBufferDesc.StructureByteStride = 0;

                //Give the subresource structure a pointer to the vertex data.
                vertexData.pSysMem = mesh->GetRawModelData( ).data( );
                vertexData.SysMemPitch = 0;
                vertexData.SysMemSlicePitch = 0;

                //Now create the vertex buffer.
                result = m_device->CreateBuffer( &vertexBufferDesc, &vertexData, &mesh->GetVertexBuffer( ) );
                UAssert(result == S_OK, "Failed to make vertex buffer! (%s)", DXCore::GetDXErrorMessage(result));

                D3D11_BUFFER_DESC indexBufferDesc;
                D3D11_SUBRESOURCE_DATA indexData;

                //Set up the description of the static index buffer.
                indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
                indexBufferDesc.ByteWidth = sizeof(unsigned) * mesh->GetIndexCount( );
                indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
                indexBufferDesc.CPUAccessFlags = 0;
                indexBufferDesc.MiscFlags = 0;
                indexBufferDesc.StructureByteStride = 0;

                //Give the subresource structure a pointer to the index data.
                indexData.pSysMem = mesh->GetRawIndices( ).data( );
                indexData.SysMemPitch = 0;
                indexData.SysMemSlicePitch = 0;

                //Create the index buffer.
                result = m_device->CreateBuffer( &indexBufferDesc, &indexData, &mesh->GetIndexBuffer( ) );
                UAssert(result == S_OK, "Failed to make index buffer! (%s)", DXCore::GetDXErrorMessage(result));
            }
        }

        void ModelManager::unloadModelFromGPU(ModelResource &model)
        {
            auto &meshBuffer = model.GetMeshArray( );

            model.SetIsLoaded( false );

            for (auto &mesh : meshBuffer)
            {
                RELEASE_RESOURCE( mesh->GetIndexBuffer( ) );
                RELEASE_RESOURCE( mesh->GetVertexBuffer( ) );
            }
        }
    }
}
