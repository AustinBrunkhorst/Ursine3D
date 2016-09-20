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

namespace ursine
{
    using namespace graphics;

    namespace resources
    {
        UModelData::UModelData(const UModelData& other)
            : m_onGPU( false )
        {
            
        }

        UModelData::~UModelData(void)
        {
            for (auto &mesh : m_meshes)
            {
                delete mesh;
            }

            m_meshes.clear( );
        }

        void UModelData::AddMesh(Mesh *mesh)
        {
            m_meshes.push_back( mesh );
        }

        Mesh *UModelData::GetMesh(uint index) const
        {
            return m_meshes[ index ];
        }

        void UModelData::Write(pipeline::ResourceWriter &output)
        {
            // TODO: [J] Implement this
        }

        meta::Type UModelData::GetReaderType(void)
        {
            return typeof(UModelReader);
        }
    }
}
