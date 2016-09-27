/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Model.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "ModelResource.h"

namespace ursine
{
    namespace graphics
    {
        ModelResource::ModelResource(const resources::UModelData::Handle &modelData)
            : m_meshArray( )
            , m_meshMap( )
            , m_onGPU( false )
            , m_referenceCount( 0 )
            , m_modelData( modelData )
        {
            // initialize
            for(uint i = 0, n = m_modelData->GetNumMeshes( ); i < n; ++i)
            {
                auto meshData = m_modelData->GetMesh( i );
                auto meshResource = new MeshResource( meshData );

                meshResource->SetID( i );

                m_meshArray.push_back( meshResource );
                m_meshMap[ meshData->GetName( ) ] = meshResource;
            }
        }

        ModelResource::~ModelResource(void)
        {
            for (auto x : m_meshArray)
                delete x;
        }

        MeshResource *ModelResource::GetMesh(uint index) const
        {
            if(index >= m_meshArray.size())
            {
                return m_meshArray[ 0 ];
            }
            
            return m_meshArray[index];
        }

        MeshResource *ModelResource::GetMeshByName(const std::string &name)
        {
            return m_meshMap[name];
        }

        unsigned ModelResource::GetMeshCount(void) const
        {
            return static_cast<unsigned>(m_meshArray.size());
        }

        const std::vector<MeshResource*> &ModelResource::GetMeshArray(void) const
        {
            return m_meshArray;
        }

        const std::string &ModelResource::GetName(void) const
        {
            return m_modelData->GetName( );
        }

        void ModelResource::IncrementReference(void)
        {
            ++m_referenceCount;
        }

        void ModelResource::DecrementReference(void)
        {
            if(m_referenceCount == 0)
                return;
            --m_referenceCount;
        }

        void ModelResource::NoReference(void)
        {
            m_referenceCount = 0;
        }

        bool ModelResource::HasNoReferences(void) const
        {
            return m_referenceCount == 0;
        }

        bool ModelResource::GetIsLoaded() const
        {
            return m_onGPU;
        }

        void ModelResource::SetIsLoaded(bool isOnGPU)
        {
            m_onGPU = isOnGPU;
        }

        ModelResource::ModelResource(void)
            : m_onGPU( false )
            , m_referenceCount( 0 ) { }

        void ModelResource::addMesh(MeshResource *mesh)
        {
            m_meshArray.push_back( mesh );
            m_meshMap[ mesh->GetName( ) ] = mesh;
        }
    }
}