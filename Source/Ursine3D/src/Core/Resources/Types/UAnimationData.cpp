/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** UAnimationData.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "UAnimationData.h"

#include "UAnimationReader.h"
#include "RigComponent.h"

namespace ursine
{
    namespace resources
    {
        UAnimationData::UAnimationData(void)
            : name( "N/A" )
            , duration( 0.0f ) { }

        void UAnimationData::Evaluate(ecs::Rig *rig, float time)
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

        void UAnimationData::Write(pipeline::ResourceWriter &output)
        {
            output.Write( name );
            output.Write( duration );

            output.Write( lanes.size( ) );

            for (auto &lane : lanes)
            {
                lane.Write( output );
            }
        }

        meta::Type UAnimationData::GetReaderType(void)
        {
            return typeof(UAnimationReader);
        }
    }
}
