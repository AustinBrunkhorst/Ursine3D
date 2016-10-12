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
        graphics::GfxHND UModelData::GetGraphicsHandle(void) const
        {
            return m_id;
        }

        void UModelData::Write(pipeline::ResourceWriter &output)
        {
            output.Write( meshes.size( ) );

            for (auto &mesh : meshes)
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
