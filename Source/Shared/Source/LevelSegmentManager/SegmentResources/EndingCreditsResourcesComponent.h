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

class EndingCreditsResources : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    EndingCreditsResources(void);

    EditorResourceField(
        ursine::resources::WorldData,
        world,
        GetWorldData,
        SetWorldData
    );

    const ursine::resources::ResourceReference &GetWorldData(void) const;
    void SetWorldData(const ursine::resources::ResourceReference &world);

private:
    ursine::resources::ResourceReference m_worldToMerge;

} Meta(Enable);
