/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
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
        ModelResource::ModelResource(void)
            : m_rootNode(nullptr)
            , m_meshArray( )
            , m_meshMap( )
        {
        }

        ModelResource::~ModelResource(void)
        {
            for (auto *x : m_meshArray)
                delete x;
        }

        void ModelResource::AddMesh(Mesh *newMesh)
        {
            if (m_meshArray.size( ) == 0)
                m_rootNode = newMesh;

            m_meshMap[ newMesh->GetName( ) ] = newMesh;
            m_meshArray.push_back( newMesh );
        }

        Mesh *ModelResource::GetMesh(const unsigned index)
        {
            UAssert( index < m_meshArray.size( ), "Tried to get mesh that does not exist!" );
            return m_meshArray[ index ];
        }

        Mesh *ModelResource::GetMesh(const std::string & name)
        {
            return m_meshMap[ name ];
        }

        unsigned ModelResource::GetMeshCount(void) const
        {
            return static_cast<unsigned>( m_meshArray.size(  ) );
        }

        const std::vector<Mesh*> &ModelResource::GetMeshArray(void) const
        {
            return m_meshArray;
        }
    }
}