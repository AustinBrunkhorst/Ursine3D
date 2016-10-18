/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** AnimationReader.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "AnimationReader.h"

#include "AnimationData.h"

namespace ursine
{
    namespace resources
    {
        AnimationReader::AnimationReader(void) { }

        ResourceData::Handle AnimationReader::Read(ResourceReader &input)
        {
            auto anim = std::make_shared<AnimationData>( );

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
