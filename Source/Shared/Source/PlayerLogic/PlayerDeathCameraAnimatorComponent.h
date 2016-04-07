/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** PlayerDeathCameraAnimatorComponent.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

#include "EntityAnimatorComponent.h"

#include <CameraComponent.h>

class PlayerDeathCameraAnimator : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    PlayerDeathCameraAnimator(void);
    
    void AnimateDeath(void);
    void AnimateRevival(void);

    std::string fpsCam;

    float outTime;
    ursine::ease::Type outEase;

    float inTime;
    ursine::ease::Type inEase;

private:
    void onAnimationFinished(EVENT_HANDLER(EntityAnimator));

} Meta(
    Enable,
    RequiresComponents(
        typeof( EntityAnimator ),
        typeof( ursine::ecs::Camera )
    )
);
