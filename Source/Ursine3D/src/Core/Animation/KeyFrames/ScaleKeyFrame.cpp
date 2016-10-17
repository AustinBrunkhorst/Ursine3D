/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ScaleKeyFrame.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "ScaleKeyFrame.h"

namespace ursine
{
    ScaleKeyFrame::ScaleKeyFrame(void)
        : KeyFrame( 0.0f ) { }

    ScaleKeyFrame::ScaleKeyFrame(float time, const SVec3& value)
        : KeyFrame( time )
        , value( value ) { }

    void ScaleKeyFrame::Interpolate(KeyFrame *other, float t, ecs::EntityHandle &entity)
    {
        auto otherScale = static_cast<ScaleKeyFrame*>( other );

        auto result = value * (1.0f - t) + otherScale->value * t;

        entity->GetTransform( )->SetLocalScale( result );
    }

    void ScaleKeyFrame::SetValue(ecs::EntityHandle &entity)
    {
        entity->GetTransform( )->SetLocalScale( value );
    }
}
