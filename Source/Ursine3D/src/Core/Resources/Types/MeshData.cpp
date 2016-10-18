/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** MeshData.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "MeshData.h"
#include "MeshReader.h"

namespace ursine
{
    namespace resources
    {
        MeshData::Vertex::Vertex(void)
            : boneIndices{ static_cast<uint>( -1 ) } { }

        void MeshData::SetName(const std::string &name)
        {
            m_name = name;
        }

        const std::string &MeshData::GetName(void) const
        {
            return m_name;
        }

        void MeshData::Write(pipeline::ResourceWriter &output)
        {
            output.Write( m_name );
            output.Write( verts.size( ) );

            for (auto &vert : verts)
            {
                output.Write( vert.position.X( ) );
                output.Write( vert.position.Y( ) );
                output.Write( vert.position.Z( ) );

                output.Write( vert.normal.X( ) );
                output.Write( vert.normal.Y( ) );
                output.Write( vert.normal.Z( ) );

                output.Write( vert.tangent.X( ) );
                output.Write( vert.tangent.Y( ) );
                output.Write( vert.tangent.Z( ) );

                output.Write( vert.bitangent.X( ) );
                output.Write( vert.bitangent.Y( ) );
                output.Write( vert.bitangent.Z( ) );

                output.Write( vert.uv.X( ) );
                output.Write( vert.uv.Y( ) );

                output.Write( vert.boneIndices[ 0 ] );
                output.Write( vert.boneIndices[ 1 ] );
                output.Write( vert.boneIndices[ 2 ] );
                output.Write( vert.boneIndices[ 3 ] );

                output.Write( vert.boneWeights.X( ) );
                output.Write( vert.boneWeights.Y( ) );
                output.Write( vert.boneWeights.Z( ) );
                output.Write( vert.boneWeights.W( ) );
            }

            output.Write( indices.size( ) );

            for (auto &index : indices)
            {
                output.Write( index );
            }
        }

        meta::Type MeshData::GetReaderType(void)
        {
            return typeof(MeshReader);
        }

    }
}