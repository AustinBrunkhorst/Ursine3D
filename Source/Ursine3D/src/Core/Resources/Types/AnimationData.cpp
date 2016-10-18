/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** AnimationData.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "AnimationData.h"

#include "AnimationReader.h"
#include "RigComponent.h"

namespace ursine
{
    namespace resources
    {
        AnimationData::AnimationData(void)
            : name( "NULL" )
            , duration( 0.0f ) { }

        void AnimationData::Evaluate(ecs::Rig *rig, float time)
        {
            for (auto &lane : lanes)
            {
                auto entity = rig->GetBoneEntity( lane.boneIndex );

                if (!entity)
                {
                    continue;
                }

                lane.Evaluate( entity, time );
            }
        }

        void AnimationData::Write(pipeline::ResourceWriter &output)
        {
            output.Write( name );
            output.Write( duration );

            output.Write( lanes.size( ) );

            for (auto &lane : lanes)
            {
                lane.Write( output );
            }
        }

        meta::Type AnimationData::GetReaderType(void)
        {
            return typeof(AnimationReader);
        }
    }
}
