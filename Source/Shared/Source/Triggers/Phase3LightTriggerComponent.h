/* ----------------------------------------------------------------------------
** Team Bear King
** 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** Phase3LightTriggerComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** --------------------------------------------------------------------------*/

#pragma once

#include <Component.h>
#include <GhostComponent.h>
#include <WorldEvent.h>

#include "LightingGroupComponent.h"
#include "EntityAnimatorComponent.h"

class EntityAnimator;

enum Phase3LightingEvent
{
    BOTH_SIDES_TRIGGERED = ursine::ecs::WORLD_EVENT_USER + 50
};

class Phase3LightTrigger : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:

    EditorField(
        std::string leftSideLights,
        GetLeftSideLights,
        SetLeftSideLights
    );

    EditorField(
        std::string rightSideLights,
        GetRightSideLights,
        SetRightSideLights
    );
    
    Phase3LightTrigger(void);
    ~Phase3LightTrigger(void);

    const std::string &GetLeftSideLights(void) const;
    void SetLeftSideLights(const std::string &entity);

    const std::string &GetRightSideLights(void) const;
    void SetRightSideLights(const std::string &entity);

    void OnSceneReady(ursine::Scene *scene) override;

private:

    std::string m_left;
    std::string m_right;

    LightingGroup *m_leftGroup;
    LightingGroup *m_rightGroup;

    bool m_animated;

    void onUpdate(EVENT_HANDLER(ursine::ecs::World));

    void onAnimationFinished(EVENT_HANDLER(EntityAnimator));

} Meta(Enable, RequiresComponents(typeof(ursine::ecs::Ghost), typeof(EntityAnimator)));
