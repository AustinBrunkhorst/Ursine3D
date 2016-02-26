/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EntityAnimatorGroupComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

class EntityAnimatorGroup : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:

    EditorButton(
        playGroupAnimators,
        "Play Group Animators"
    );

    EditorButton(
        stopGroupAnimators,
        "Stop Group Animators"
    );

    EntityAnimatorGroup(void);

    void PlayGroupAnimators(void);
    void StopGroupAnimators(void);

} Meta(Enable);
