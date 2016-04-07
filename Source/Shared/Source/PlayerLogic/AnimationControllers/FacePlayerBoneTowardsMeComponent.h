/* ----------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** FacePlayerBoneTowardsMeComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** --------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

class FaceBoneTowardsTarget;

class FacePlayerBoneTowardsMe : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    FacePlayerBoneTowardsMe(void);
    ~FacePlayerBoneTowardsMe(void);

    void OnSceneReady(ursine::Scene *scene) override;

private:
    std::vector<FaceBoneTowardsTarget *> m_targetFinder;

    void onUpdate(EVENT_HANDLER(ursine::ecs::World));

} Meta( Enable );
