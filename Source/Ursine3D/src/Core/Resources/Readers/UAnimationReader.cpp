/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** UAnimationReader.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "UAnimationReader.h"

#include "UAnimationData.h"

namespace ursine
{
    namespace resources
    {
        UAnimationReader::UAnimationReader(void) { }

        ResourceData::Handle UAnimationReader::Read(ResourceReader &input)
        {
            auto anim = std::make_shared<UAnimationData>( );

            input.Read( anim->name );
            input.Read( anim->duration );

            size_t size;

            input.Read( size );

            anim->lanes.resize( size );

            for (uint i = 0; i < size; ++i)
            {
                anim->lanes[ i ].Read( input );
            }

            return anim;
        }
    }
}
