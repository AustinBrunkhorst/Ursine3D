/*---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** FaceBoneTowardsOrigin.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
**-------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

class FaceBoneTowardsOrigin : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    FaceBoneTowardsOrigin(void);

    void OnSceneReady(ursine::Scene *scene) override;

private:
    void onAnimationManipulation(EVENT_HANDLER(ursine::ecs::Entity));

} Meta(Enable);
