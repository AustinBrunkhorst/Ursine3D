/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** UMeshData.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "UMeshData.h"
#include "UMeshReader.h"

namespace ursine
{
    namespace resources
    {
        void UMeshData::SetName(const std::string &name)
        {
            m_name = name;
        }

        const std::string &UMeshData::GetName(void) const
        {
            return m_name;
        }

        void UMeshData::Write(pipeline::ResourceWriter &output)
        {
            output.Write( verts.size( ) );

            for (auto &vert : verts)
            {
                output.Write( vert.X( ) );
                output.Write( vert.Y( ) );
                output.Write( vert.Z( ) );
            }

            output.Write( indices.size( ) );

            for (auto &index : indices)
            {
                output.Write( index );
            }

            output.Write( normals.size( ) );

            for (auto &norm : normals)
            {
                output.Write( norm.X( ) );
                output.Write( norm.Y( ) );
                output.Write( norm.Z( ) );
            }

            output.Write( tangents.size( ) );

            for (auto &tang : tangents)
            {
                output.Write( tang.X( ) );
                output.Write( tang.Y( ) );
                output.Write( tang.Z( ) );
            }

            output.Write( bitangents.size( ) );

            for (auto &bi : bitangents)
            {
                output.Write( bi.X( ) );
                output.Write( bi.Y( ) );
                output.Write( bi.Z( ) );
            }
        }

        meta::Type UMeshData::GetReaderType(void)
        {
            return typeof(UMeshReader);
        }

    }
}