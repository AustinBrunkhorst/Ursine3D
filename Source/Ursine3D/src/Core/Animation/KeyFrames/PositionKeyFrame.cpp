/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** PositionKeyFrame.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "PositionKeyFrame.h"

namespace ursine
{
    PositionKeyFrame::PositionKeyFrame(void)
        : KeyFrame( 0.0f ) { }

    PositionKeyFrame::PositionKeyFrame(float time, const SVec3 &value)
        : KeyFrame( time )
        , value( value ) { }

    void PositionKeyFrame::Interpolate(KeyFrame *other, float t, ecs::EntityHandle &entity)
    {
        auto otherPos = static_cast<PositionKeyFrame*>( other );

        auto result = value * (1.0f - t) + otherPos->value * t;

        entity->GetTransform( )->SetLocalPosition( result );
    }

    void PositionKeyFrame::SetValue(ecs::EntityHandle &entity)
    {
        entity->GetTransform( )->SetLocalPosition( value );
    }
}
