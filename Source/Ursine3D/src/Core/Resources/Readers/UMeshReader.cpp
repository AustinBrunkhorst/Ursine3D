/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** UMeshReader.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "UMeshReader.h"
#include "UMeshData.h"

namespace ursine
{
    namespace resources
    {
        UMeshData::Handle UMeshReader::Read(ResourceReader &input)
        {
            auto mesh = std::make_shared<UMeshData>( );
            size_t numVerts, numIndices, numNormals, numTangents, numBitangents;

            input.Read(numVerts);

            mesh->verts.resize( numVerts );

            for (auto &vert : mesh->verts)
            {
                input.Read( vert.X( ) );
                input.Read( vert.Y( ) );
                input.Read( vert.Z( ) );
            }

            input.Read( numIndices );

            mesh->indices.resize( numIndices );

            for (auto &index : mesh->indices)
            {
                input.Read( index );
            }

            input.Read( numNormals );

            mesh->normals.resize( numNormals );

            for (auto &normal : mesh->normals)
            {
                input.Read( normal.X( ) );
                input.Read( normal.Y( ) );
                input.Read( normal.Z( ) );
            }

            input.Read( numTangents );

            mesh->tangents.resize( numTangents );

            for (auto &tangent : mesh->tangents)
            {
                input.Read( tangent.X( ) );
                input.Read( tangent.Y( ) );
                input.Read( tangent.Z( ) );
            }

            input.Read( numBitangents );

            mesh->bitangents.resize( numBitangents );

            for (auto &bi : mesh->bitangents)
            {
                input.Read( bi.X( ) );
                input.Read( bi.Y( ) );
                input.Read( bi.Z( ) );
            }

            return mesh;
        }
    }
}
