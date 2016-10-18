/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ModelData.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "ModelData.h"
#include "ModelReader.h"

namespace ursine
{
    using namespace graphics;

    namespace resources
    {
        ModelData::ModelData(void)
          : m_id( 0 ) { }

        graphics::GfxHND ModelData::GetGraphicsHandle(void) const
        {
            return m_id;
        }

        void ModelData::Write(pipeline::ResourceWriter &output)
        {
            output.Write( meshes.size( ) );

            for (auto &mesh : meshes)
            {
                mesh->Write( output );
            }
        }

        // TODO: [J] The ResourceTypeReader class doesn't need to be exist, 
        // just make a "Read" virtual function for the Resource type class
        meta::Type ModelData::GetReaderType(void)
        {
            return typeof(ModelReader);
        }
    }
}
