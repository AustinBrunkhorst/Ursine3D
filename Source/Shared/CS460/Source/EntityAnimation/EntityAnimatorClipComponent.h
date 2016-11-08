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
        : positionKey( false )
        , scaleKey( false )
        , rotationKey( false )
        , delta( 0.0f )
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

    ursine::Array<float> interpolationLookup;

} Meta(Enable) EditorMeta(HiddenInSelector);
