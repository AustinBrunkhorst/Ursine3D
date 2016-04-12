/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** WaypointManagerComponent.h
**
** Author:
** - Joshua Shlemmer - joshua.shlemmer@digipen.edu
**
** Contributors:
** --------------------------------------------------------------------------*/
#pragma once
#include <Component.h>
#include <ArchetypeData.h>
#include "WaypointComponent.h"

struct DeserializedWaypoint
{
    ursine::ecs::Waypoint *waypoint;

    std::vector<std::string> GuidsOfConnectedWPs;
};

class WaypointManager : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

    friend ursine::ecs::Waypoint;
public:

    //EditorField(
    //    ursine::resources::ResourceReference
    //    waypointArchetype,
    //    GetWaypointArchetype,
    //    SetWaypointArchetype
    //    );
    EditorResourceField(
        ursine::resources::ArchetypeData,
        waypointArchetype,
        GetWaypointArchetype,
        SetWaypointArchetype
        );

    WaypointManager(void);

    void OnSerialize(ursine::Json::object &output) const override;

    void OnDeserialize(const ursine::Json &input) override;

    virtual void OnSceneReady(ursine::Scene *scene) override;

    const ursine::resources::ResourceReference &GetWaypointArchetype(void) const;

    void SetWaypointArchetype(const ursine::resources::ResourceReference &archetype);

    static void WaypointManager::createObjectFromWaypoint(const ursine::ecs::Waypoint& wp, ursine::Json::object &objectOut);

private:
    void onHierarchySerialized(EVENT_HANDLER(Entity));

    void spawnWaypoint(const ursine::Json::array &position, const std::string& guid);

    //Meta(Disable)
    //std::unordered_map<std::string, ursine::ecs::Waypoint *> m_loadedWaypoints;

    Meta(Disable)
    std::unordered_map<std::string, DeserializedWaypoint> m_deserialized;

    Meta(Enable)
    ursine::resources::ResourceReference m_waypointArchetypeReference;

}Meta(Enable, DisplayName("WaypointManager"))
//EditorMeta(DisableComponentRemoval)
;
