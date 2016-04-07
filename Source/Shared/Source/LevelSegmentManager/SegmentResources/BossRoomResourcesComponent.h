/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** BossRoomResourcesComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include <Component.h>
#include <Array.h>
#include <WorldData.h>

class BossRoomResources : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:

    BossRoomResources(void);

    EditorResourceField(
        ursine::resources::WorldData,
        world,
        GetWorldData,
        SetWorldData
    );

    const ursine::resources::ResourceReference &GetWorldData(void) const;
    void SetWorldData(const ursine::resources::ResourceReference &world);

    std::string bossTopEntityName;
    std::string bossDoorEntityName;
    std::string bossIntroCinCameraName;
    std::string bossTransitionCameraName;

    // Lighting that stays on no matter what
    std::string staticLighting;

    // Lighting for phase 1 and 2
    std::string phase12Lighting;

    // All the different lighting groups for phase 3
    std::string phase3CenterLights;
    std::string phase3LeftLights;
    std::string phase3RightLights;
    std::string phase3BossLights;

    std::string phase3CinematicCamera;
    std::string phase3CinematicFocalPoint;

private:
    ursine::resources::ResourceReference m_worldToMerge;

} Meta(Enable);