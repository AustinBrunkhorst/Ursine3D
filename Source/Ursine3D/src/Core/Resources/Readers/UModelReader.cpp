/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** UModelReader.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "UModelReader.h"

#include "UModelData.h"
#include "UMeshReader.h"

namespace ursine
{
    namespace resources
    {
        // TODO: [J] Remove the "model" concept all together?
        // I.E make everything run off of meshes, and the "Model"
        // generated object would just be a hierarchy of entities
        UModelReader::UModelReader(void) { }

        ResourceData::Handle UModelReader::Read(ResourceReader &input)
        {
            auto output = std::make_shared<UModelData>( );
            size_t numMeshes = 0;
            UMeshReader reader;

            input.Read<size_t>(numMeshes);

            while (numMeshes > 0)
            {
                output->AddMesh(
                    reader.Read( input )
                );
            }
        }
    }
}
