/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** URigData.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "URigData.h"

#include "URigReader.h"

namespace ursine
{
    namespace resources
    {
        void URigData::Write(pipeline::ResourceWriter &output)
        {
            output.Write( name );

            output.Write( bones.size( ) );

            for (auto &bone : bones)
            {
                output.Write( bone.localPosition );
                output.Write( bone.localScale );
                output.Write( bone.localRotation );

                output.Write( bone.offset );

                output.Write( bone.parent );
                output.Write( bone.numChildren );

                output.Write( bone.name );
            }

            output.Write( boneMap.size( ) );

            for (auto &pair : boneMap)
            {
                output.Write( pair.first );
                output.Write( pair.second );
            }
        }

        meta::Type URigData::GetReaderType(void)
        {
            return typeof(URigReader);
        }
    }
}