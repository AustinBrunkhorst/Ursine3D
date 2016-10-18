/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** RigReader.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "RigReader.h"

#include "RigData.h"

namespace ursine
{
    namespace resources
    {
        RigReader::RigReader(void) { }

        ResourceData::Handle RigReader::Read(ResourceReader &input)
        {
            auto rig = std::make_shared<RigData>( );
            size_t boneCount = 0;
            size_t boneMapSize = 0;

            input.Read( rig->name );

            input.Read( boneCount );

            rig->bones.resize( boneCount );

            for (size_t i = 0; i < boneCount; ++i)
            {
                auto &bone = rig->bones[ i ];

                input.Read( bone.localPosition );
                input.Read( bone.localScale );
                input.Read( bone.localRotation );

                input.Read( bone.offset );

                input.Read( bone.parent );
                input.Read( bone.numChildren );

                input.Read( bone.name );
            }

            input.Read( boneMapSize );

            for (size_t i = 0; i < boneMapSize; ++i)
            {
                std::string name;
                uint index;

                input.Read( name );
                input.Read( index );

                rig->boneMap[ name ] = index;
            }

            return rig;
        }
    }
}
