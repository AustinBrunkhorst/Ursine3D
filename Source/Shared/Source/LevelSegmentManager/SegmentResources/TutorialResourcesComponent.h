/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** TutorialResourcesComponent.h
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
#include <ArchetypeData.h>

class TutorialResources : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:

    TutorialResources(void);

    EditorResourceField(
        ursine::resources::WorldData,
        world,
        GetWorldData,
        SetWorldData
    );

    EditorResourceField(
        ursine::resources::ArchetypeData,
        vineArchetype,
        GetVineArchetype,
        SetVineArchetype
    );

    const ursine::resources::ResourceReference &GetWorldData(void) const;
    void SetWorldData(const ursine::resources::ResourceReference &world);

    const ursine::resources::ResourceReference &GetVineArchetype(void) const;
    void SetVineArchetype(const ursine::resources::ResourceReference &vine);

private:
    ursine::resources::ResourceReference m_worldToMerge;
    ursine::resources::ResourceReference m_vine;

} Meta(Enable);
