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
        void UModelData::AddMesh(const UMeshData::Handle &mesh)
        {
            m_meshes.push_back( mesh );
        }

        const UMeshData::Handle &UModelData::GetMesh(uint index) const
        {
            return m_meshes[ index ];
        }

        uint UModelData::GetNumMeshes(void) const
        {
            return static_cast<uint>( m_meshes.size( ) );
        }

        void UModelData::SetName(const std::string &name)
        {
            m_name = name;
        }

        const std::string &UModelData::GetName(void) const
        {
            return m_name;
        }

        graphics::GfxHND UModelData::GetGraphicsHandle(void) const
        {
            return m_id;
        }

        void UModelData::Write(pipeline::ResourceWriter &output)
        {
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
