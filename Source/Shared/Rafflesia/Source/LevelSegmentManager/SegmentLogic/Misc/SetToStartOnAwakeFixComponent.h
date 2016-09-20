// Jordan does not want to claim ownership of this horrible idea

#pragma once

#include <Component.h>

#include "EntityAnimatorComponent.h"

class SetToStartOnAwakeFix : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    SetToStartOnAwakeFix(void);

    void OnSceneReady(ursine::Scene *scene) override;

} Meta(
    Enable
) EditorMeta(
    RequiresComponents(
        typeof( EntityAnimator )
    )
);
