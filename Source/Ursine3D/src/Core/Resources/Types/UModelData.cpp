/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** UModelData.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "UModelData.h"
#include "UModelReader.h"

namespace ursine
{
    using namespace graphics;

    namespace resources
    {
        void UModelData::AddMesh(UMeshData::Handle mesh)
        {
            m_meshes.push_back( mesh );
        }

        const UMeshData *UModelData::GetMesh(uint index) const
        {
            return m_meshes[ index ].get( );
        }

        uint UModelData::GetNumMeshes(void) const
        {
            return m_meshes.size( );
        }

        void UModelData::Write(pipeline::ResourceWriter &output)
        {
            // TODO: [J] Write how many meshes there are?
            output.Write( m_meshes.size( ) );

            for (auto &mesh : m_meshes)
            {
                mesh->Write( output );
            }
        }

        // TODO: [J] The ResourceTypeReader class doesn't need to be exist, 
        // just make a "Read" virtual function for the Resource type class
        meta::Type UModelData::GetReaderType(void)
        {
            return typeof(UModelReader);
        }
    }
}
