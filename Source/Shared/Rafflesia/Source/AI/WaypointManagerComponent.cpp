/* ----------------------------------------------------------------------------
** Team Bear King
** 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** WaypointManagerComponent.cpp
**
** Author:
** - Joshua Shlemmer - joshua.shlemmer@digipen.edu
**
** Contributors:
** --------------------------------------------------------------------------*/
#include "Precompiled.h"

#include "WaypointManagerComponent.h"
#include <Game Engine/Scene/Entity/EntityEvent.h>
using namespace ursine;
using namespace ecs;

NATIVE_COMPONENT_DEFINITION(WaypointManager);


WaypointManager::WaypointManager() : BaseComponent( )
{
}

void WaypointManager::OnSerialize(ursine::Json::object& output) const
{
    // WaypointList haswaypoints
    Json::array waypointList;

    // each waypoint has object for position and object for guid
    auto allWaypoints = GetOwner()->GetComponentsInChildren<Waypoint>();

    for (auto wp : allWaypoints)
    {
        Json::object wpObject;

        createObjectFromWaypoint(*wp, wpObject);

        waypointList.push_back(wpObject);
    }

    output["WaypointList"] = waypointList;
}

void WaypointManager::OnDeserialize(const ursine::Json& input)
{
    m_deserialized.clear();

    auto waypointList = input["WaypointList"];

    if (waypointList.is_null())
        return;

    auto waypoints = waypointList.array_items();

    // first we have to create all of the waypoints
    for (auto JSONwaypoint : waypoints)
    {
        auto guid = JSONwaypoint["GUID"];

        auto &guidArray = JSONwaypoint["Connections"].array_items();

        //spawnWaypoint(position.array_items(), guid.string_value());
        auto &deserializedObj = m_deserialized[guid.string_value()];

        for (auto connectedGuid : guidArray)
        {
            deserializedObj.GuidsOfConnectedWPs.push_back(connectedGuid.string_value());
        }
    }
    return;
}

void WaypointManager::OnSceneReady(ursine::Scene* scene)
{
    // now we have to connect them
    /*
    for (auto JSONwaypoint : waypoints)
    {
    auto &connections = JSONwaypoint["Connections"];//.object_items;

    for (auto connection : connections.array_items())
    {
    m_loadedWaypoints[JSONwaypoint["GUID"].string_value()]
    ->AddWaypointConnection(m_loadedWaypoints[connection.string_value()]);
    }
    }*/

    GetOwner()->Listener(this)
        .On(ENTITY_HIERARCHY_SERIALIZED, &WaypointManager::onHierarchySerialized);


}

const ursine::resources::ResourceReference& WaypointManager::GetWaypointArchetype() const
{
    return m_waypointArchetypeReference;
}

void WaypointManager::SetWaypointArchetype(const ursine::resources::ResourceReference& archetype)
{
    m_waypointArchetypeReference = archetype;
}

void WaypointManager::createObjectFromWaypoint(const ursine::ecs::Waypoint& wp, Json::object &objectOut)
{
    objectOut["GUID"] = wp.GetGUID();

    Json::array connections;

    for (auto wpPair : wp.GetConnectedWaypoints())
    {
        connections.push_back(wpPair.waypoint->GetGUID());
    }

    objectOut["Connections"] = connections;
}

void WaypointManager::onHierarchySerialized(EVENT_HANDLER(Entity))
{
    //ENTITY_HIERARCHY_SERIALIZED
    EVENT_ATTRS(Entity, EntityEventArgs);

    // get all waypoints in our children
    auto childWaypoints = GetOwner()->GetComponentsInChildren<Waypoint>();

    for (auto *childWP : childWaypoints)
    {
        //place them in a map based off of guid

        m_deserialized[childWP->GetGUID()].waypoint = childWP;
    }

    for (auto &wpPair : m_deserialized)
    {
        for (std::string &connection : wpPair.second.GuidsOfConnectedWPs)
        {
            auto wp = m_deserialized[connection].waypoint;
            wpPair.second.waypoint->AddWaypointConnection(wp);
        }
    }
}

void WaypointManager::spawnWaypoint(const ursine::Json::array &position, const std::string& guid)
{
    // create new waypoint in the world
    auto waypointEntity = this->GetOwner()->GetWorld()->CreateEntityFromArchetype(m_waypointArchetypeReference);

    // it is going to generate a new guid, replace it with the correct one
    if (waypointEntity)
    {
        waypointEntity->GetComponent<Waypoint>()->SetGUID(guid);
    }
}
