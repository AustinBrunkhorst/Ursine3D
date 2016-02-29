/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EntityAnimatorClipComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

struct EntityKeyFrame
{
    EntityKeyFrame(void)
        : delta( 0.0f )
        , positionKey( false )
        , scaleKey( false )
        , rotationKey( false )
        , ease( ursine::ease::Type::Linear ) { }

    bool positionKey;
    bool scaleKey;
    bool rotationKey;

    ursine::SVec3 position;
    ursine::SVec3 scale;
    ursine::SQuat rotation;

    float delta;

    ursine::ease::Type ease;

} Meta(Enable, EnableArrayType);

class EntityAnimatorClip : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    EntityAnimatorClip(void);

    std::string clipName;

    bool looping;

    ursine::Array<EntityKeyFrame> keyFrames;

} Meta(Enable, HiddenInSelector);
