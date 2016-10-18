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
#include "DXErrorHandling.h"
#include "MeshData.h"
#include "ModelReader.h"

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
        {
        }

        void ModelManager::Initialize(ID3D11Device *device, ID3D11DeviceContext *context, std::string filePath)
        {
            m_device = device;
            m_deviceContext = context;

            m_currentState = -1;

            m_loadingModel = false;

            /////////////////////////////////////////////////////////////////////////////
            // GENERATING INTERNAL PS INDEX BUFFER
            {
                auto meshData = std::make_shared<resources::MeshData>( );
                auto *newMesh = new MeshResource( meshData );
                auto modelData = std::make_shared<resources::ModelData>( );
                auto &internalPoint = m_modelCache[ INTERNAL_POINT_INDICES ] = new ModelResource( modelData );

                internalPoint->addMesh( newMesh );
                internalPoint->SetIsLoaded( true );

                meshData->indices.resize( 1024 * 6 );

                for (int x = 0; x < 1024 * 6; ++x)
                    meshData->indices[ x ] = x;

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
                indexData.pSysMem = &meshData->indices[0];
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

                auto meshData = std::make_shared<resources::MeshData>( );
                auto newMesh = new MeshResource( meshData );
                auto modelData = std::make_shared<resources::ModelData>( );
                auto &internalQuad = m_modelCache[ INTERNAL_QUAD ] = new ModelResource( modelData );

                internalQuad->addMesh( newMesh );

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

                uint indexArray[6];

                for (uint x = 0; x < 6; ++x)
                    indexArray[ x ] = x;

                for (int i = 0; i < 6; ++i)
                {
                    auto &vert = temp[i];
                    resources::MeshData::Vertex newVert;

                    newVert.position.Set( vert.vPos.x, vert.vPos.y, vert.vPos.z );
                    newVert.normal.Set( vert.vNor.x, vert.vNor.y, vert.vNor.z );

                    meshData->verts.push_back( newVert );
                }

                meshData->indices = { 0, 1, 2, 3, 4, 5 };

                D3D11_BUFFER_DESC indexBufferDesc;
                D3D11_SUBRESOURCE_DATA indexData;

                //Set up the description of the static index buffer.
                indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
                indexBufferDesc.ByteWidth = sizeof(uint) * 6;
                indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
                indexBufferDesc.CPUAccessFlags = 0;
                indexBufferDesc.MiscFlags = 0;
                indexBufferDesc.StructureByteStride = 0;

                //Give the subresource structure a pointer to the index data.
                indexData.pSysMem = indexArray;
                indexData.SysMemPitch = 0;
                indexData.SysMemSlicePitch = 0;

                //Create the index buffer.
                result = m_device->CreateBuffer( &indexBufferDesc, &indexData, &newMesh->GetIndexBuffer( ) );
                
                UAssert( result == S_OK, 
                    "Failed to make index buffer!"
                );

                internalQuad->SetIsLoaded( true );
            }

            ///////////////////////////////////////////////////////////////////////////////
            //// GENERATING INTERNAL CUBE
            {
                resources::ResourceReader reader( graphics_resources::kCubeJdl, 2676 );

                // load into modelInfo
                resources::ModelReader modelReader;

                auto modelData = modelReader.ReadJDL( reader );

                m_modelCache[ INTERNAL_CUBE ] = new ModelResource( modelData );

                loadModelToGPU( m_modelCache[ INTERNAL_CUBE ] );
            }

            /////////////////////////////////////////////////////////////////////////////
            // GENERATING INTERNAL SPHERE
            {
                resources::ResourceReader reader( graphics_resources::kSphereJdl, 40646 );

                // load into modelInfo
                resources::ModelReader modelInfo;

                auto modelData = modelInfo.ReadJDL( reader );

                m_modelCache[ INTERNAL_SPHERE ] = new ModelResource( modelData );

                loadModelToGPU( m_modelCache[ INTERNAL_SPHERE ] );
            }

            /////////////////////////////////////////////////////////////////////////////
            // GENERATING INTERNAL CONE
            {
                resources::ResourceReader reader( graphics_resources::kConeJdl, 5228 );

                // load into modelInfo
                resources::ModelReader modelInfo;

                auto modelData = modelInfo.ReadJDL( reader );

                m_modelCache[ INTERNAL_CONE ] = new ModelResource( modelData );

                loadModelToGPU( m_modelCache[ INTERNAL_CONE ] );
            }
        }

        void ModelManager::Uninitialize(void)
        {
            m_device = nullptr;
            m_deviceContext = nullptr;

            // delete all model cache
            for (auto &pair : m_modelCache)
            {
                delete pair.second;
            }
        }

        GfxHND ModelManager::CreateModel(const resources::ModelData::Handle &modelData)
        {
            m_loadingModel = true;
            GfxHND handle;

            auto *hnd = HND_RSRCE( handle );

            auto internalID = m_nextModelID++;

            // initialize handle
            hnd->ID_ = SANITY_RESOURCE;
            hnd->Type_ = ID_MODEL;
            hnd->Index_ = internalID;

            auto model = m_modelCache[ hnd->Index_ ] = new ModelResource(modelData);

            m_modelTable.emplace( model->GetName( ), handle );

            /////////////////////////////////////////////////////////
            // GENERATING BONE DATA /////////////////////////////////
            /////////////////////////////////////////////////////////

            unsigned rigIndex = 0;

            // TODO: [J] Do this shit?
            /*if (modelInfo.mboneCount > 0)
                rigIndex = AnimationBuilder::LoadBoneData( modelInfo, modelInfo.name );*/

            m_loadingModel = false;

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

            if (model->GetIsLoaded( ))
                unloadModelFromGPU( model );

            m_modelTable.erase( model->GetName( ) );

            delete m_modelCache[ hnd->Index_ ];

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

            if (model->HasNoReferences( ))
            {
                m_loadingModel = true;
                loadModelToGPU( model );
                m_loadingModel = false;
            }

            model->IncrementReference( );
        }

        void ModelManager::UnloadModel(GfxHND handle)
        {
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

            model->DecrementReference( );

            if (model->HasNoReferences( ))
            {
                m_unloadStack.push( hnd->Index_ );
            }
        }

        void ModelManager::BindModel(unsigned ID, unsigned index, bool indexOnly)
        {
            auto search = m_modelCache.find( ID );

            UAssert( search != m_modelCache.end( ),
                "Failed to bind model.\nID: %i",
                ID
            );

            auto &model = search->second;

            if (model->GetIsLoaded( ) == false)
            {
                UWarning(
                    "Attempted to bind model [index: %i, name: %s], but it isn't loaded on the GPU!",
                    ID, model->GetName( )
                );

                return;
            }

            if (ID == INTERNAL_POINT_INDICES)
            {
                m_deviceContext->IASetVertexBuffers( 0, 0, nullptr, nullptr, nullptr );
                m_deviceContext->IASetIndexBuffer( model->GetMesh( index )->GetIndexBuffer( ), DXGI_FORMAT_R32_UINT, 0 );

                m_currentState = ID;
                return;
            }

            // map mesh
            static const unsigned strides = sizeof( AnimationVertex );
            unsigned int offset = 0;

            m_deviceContext->IASetVertexBuffers( 0, 1, &model->GetMesh( index )->GetVertexBuffer( ), &strides, &offset );
            m_deviceContext->IASetIndexBuffer( model->GetMesh( index )->GetIndexBuffer( ), DXGI_FORMAT_R32_UINT, 0 );

            m_currentState = ID;
        }

        void ModelManager::Invalidate(void)
        {
            m_currentState = -1;
        }

        ModelResource *ModelManager::GetModel(GfxHND handle)
        {
            auto *hnd = HND_RSRCE( handle );

            UAssert( hnd->ID_ == SANITY_RESOURCE, 
                "Attempted to get model with invalid handle!"
            );

            UAssert( hnd->Type_ == ID_MODEL, 
                "Attempted to get model with handle of invalid type!"
            );

            return GetModelIndex( hnd->Index_ );
        }

        ModelResource *ModelManager::GetModelIndex(uint index)
        {
            auto search = m_modelCache.find( index );

            return search == m_modelCache.end( ) ? nullptr : search->second;
        }

        ModelResource *ModelManager::GetModel(const std::string& name)
        {
            auto index = m_modelTable.find(name);

            UAssert(index != m_modelTable.end(), "Failed to find model info with name '%s'!", name.c_str());

            auto *hnd = HND_RSRCE(index->second);

            return m_modelCache[hnd->Index_];
        }

        bool ModelManager::IsLoading() const
        {
            return m_loadingModel;
        }

        void ModelManager::EndFrame(void)
        {
            while (m_unloadStack.size() > 0)
            {
                auto &top = m_unloadStack.top( );

                auto search = m_modelCache.find( top );

                UAssert( search != m_modelCache.end( ),
                    "Failed to unload model.\nID: %i",
                    top
                );

                auto &model = search->second;

                unloadModelFromGPU( model );

                m_unloadStack.pop( );
            }
        }

        void ModelManager::waitForLoading(void) const
        {
            while(m_loadingModel) { }
        }

        void ModelManager::loadModelToGPU(ModelResource *model)
        {
            model->SetIsLoaded( true );

            auto &meshBuffer = model->GetMeshArray( );

            for (auto &mesh : meshBuffer)
            {
                D3D11_BUFFER_DESC vertexBufferDesc;
                D3D11_SUBRESOURCE_DATA vertexData;
                HRESULT result;
                std::vector<AnimationVertex> buffer;

                mesh->CreateVertexBufferData( buffer );

                //Set up the description of the static vertex buffer.
                vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
                vertexBufferDesc.ByteWidth = sizeof(AnimationVertex) * mesh->GetVertexCount( );
                vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
                vertexBufferDesc.CPUAccessFlags = 0;
                vertexBufferDesc.MiscFlags = 0;
                vertexBufferDesc.StructureByteStride = 0;

                //Give the subresource structure a pointer to the vertex data.
                vertexData.pSysMem = &buffer[ 0 ];
                vertexData.SysMemPitch = 0;
                vertexData.SysMemSlicePitch = 0;

                //Now create the vertex buffer.
                result = m_device->CreateBuffer( &vertexBufferDesc, &vertexData, &mesh->GetVertexBuffer( ) );
                UAssert(result == S_OK, "Failed to make vertex buffer! (%s)", DXCore::GetDXErrorMessage(result));

                D3D11_BUFFER_DESC indexBufferDesc;
                D3D11_SUBRESOURCE_DATA indexData;

                //Set up the description of the static index buffer.
                indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
                indexBufferDesc.ByteWidth = sizeof(uint) * mesh->GetIndexCount( );
                indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
                indexBufferDesc.CPUAccessFlags = 0;
                indexBufferDesc.MiscFlags = 0;
                indexBufferDesc.StructureByteStride = 0;

                //Give the subresource structure a pointer to the index data.
                indexData.pSysMem = mesh->GetIndexData( );
                indexData.SysMemPitch = 0;
                indexData.SysMemSlicePitch = 0;

                //Create the index buffer.
                result = m_device->CreateBuffer( &indexBufferDesc, &indexData, &mesh->GetIndexBuffer( ) );
                UAssert(result == S_OK, "Failed to make index buffer! (%s)", DXCore::GetDXErrorMessage(result));
            }
        }

        void ModelManager::unloadModelFromGPU(ModelResource *model)
        {
            auto &meshBuffer = model->GetMeshArray( );

            model->SetIsLoaded( false );

            for (auto &mesh : meshBuffer)
            {
                RELEASE_RESOURCE( mesh->GetIndexBuffer( ) );
                RELEASE_RESOURCE( mesh->GetVertexBuffer( ) );
            }
        }
    }
}
