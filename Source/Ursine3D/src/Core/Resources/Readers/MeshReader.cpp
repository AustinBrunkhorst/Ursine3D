/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** MeshReader.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "MeshReader.h"
#include "MeshData.h"

namespace ursine
{
    namespace resources
    {
        MeshData::Handle MeshReader::Read(ResourceReader &input)
        {
            auto mesh = std::make_shared<MeshData>( );
            std::string meshName;
            size_t numVerts, numIndices;

            input.Read( meshName );

            mesh->SetName( meshName );

            input.Read( numVerts );

            mesh->verts.resize( numVerts );

            for (auto &vert : mesh->verts)
            {
                input.Read( vert.position.X( ) );
                input.Read( vert.position.Y( ) );
                input.Read( vert.position.Z( ) );

                input.Read( vert.normal.X( ) );
                input.Read( vert.normal.Y( ) );
                input.Read( vert.normal.Z( ) );

                input.Read( vert.tangent.X( ) );
                input.Read( vert.tangent.Y( ) );
                input.Read( vert.tangent.Z( ) );

                input.Read( vert.bitangent.X( ) );
                input.Read( vert.bitangent.Y( ) );
                input.Read( vert.bitangent.Z( ) );

                input.Read( vert.uv.X( ) );
                input.Read( vert.uv.Y( ) );

                input.Read( vert.boneIndices[ 0 ] );
                input.Read( vert.boneIndices[ 1 ] );
                input.Read( vert.boneIndices[ 2 ] );
                input.Read( vert.boneIndices[ 3 ] );

                input.Read( vert.boneWeights.X( ) );
                input.Read( vert.boneWeights.Y( ) );
                input.Read( vert.boneWeights.Z( ) );
                input.Read( vert.boneWeights.W( ) );
            }

            input.Read( numIndices );

            mesh->indices.resize( numIndices );

            for (auto &index : mesh->indices)
            {
                input.Read( index );
            }

            return mesh;
        }
    }
}
