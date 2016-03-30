/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** PlayClipOnAnimationFinishComponent.h
**
** Author:
** - JordanEllis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

#include "EntityAnimatorComponent.h"

class PlayClipOnAnimationFinish : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:

    EditorField(
        std::string clipName,
        GetClipName,
        SetClipName
    );

    PlayClipOnAnimationFinish(void);
    ~PlayClipOnAnimationFinish(void);

    void OnSceneReady(ursine::Scene *scene) override;

    const std::string &GetClipName(void) const;
    void SetClipName(const std::string &name);

private:

    std::string m_clipName;

    bool m_unsubscribed;

    void onAnimationFinish(EVENT_HANDLER(EntityAnimator));

} Meta(Enable, RequiresComponents(typeof(EntityAnimator)));
