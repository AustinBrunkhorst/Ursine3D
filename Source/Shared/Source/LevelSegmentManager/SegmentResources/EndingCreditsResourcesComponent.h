/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EndingCreditsResourcesComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

#include <WorldData.h>

#include "LevelSegmentManagerComponent.h"

class EndingCreditsResources : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    EditorResourceField(
        ursine::resources::WorldData,
        world,
        GetWorldData,
        SetWorldData
    );

    EndingCreditsResources(void);
    ~EndingCreditsResources(void);

    void OnSceneReady(ursine::Scene *scene) override;

    ursine::Array<std::string> phase5Text;
    float phase5TextDuration;

    ursine::Array<std::string> creditsText;
    float creditsTextDuration;

    const ursine::resources::ResourceReference &GetWorldData(void) const;
    void SetWorldData(const ursine::resources::ResourceReference &world);

private:
    ursine::resources::ResourceReference m_worldToMerge;

    void onSegmentChange(EVENT_HANDLER(LevelSegmentManager));

} Meta(
    Enable
) EditorMeta(
    RequiresComponents(
        typeof(LevelSegmentManager)
    )
);
