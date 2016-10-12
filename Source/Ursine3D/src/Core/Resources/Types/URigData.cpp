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
        URigData::Bone::Bone(void)
            : name( "N/A" )
            , parent( -1 )
            , numChildren( 0 ) { }

        void URigData::Write(pipeline::ResourceWriter &output)
        {
            
        }

        meta::Type URigData::GetReaderType(void)
        {
            return typeof(URigReader);
        }
    }
}