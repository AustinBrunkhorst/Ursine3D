#include "Precompiled.h"

#include "WaypointComponent.h"
//#include "EntityEvent.h"

namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION( Waypoint );


        bool waypointPair::operator==(Component::Handle<Waypoint> wp) const
        {
            return waypoint == wp;
        }

        Waypoint::Waypoint(void)
            : BaseComponent( )
            , m_cost( 1.0f )
            , m_radius( 1.0f )
            , m_index( static_cast<unsigned>( -1 ) )
            , m_jumpPoint( false )
            , m_autoWPConnect( false )
        {
            m_guid = to_string(GUIDGenerator()());
        }

        void Waypoint::OnInitialize(void)
        {
            Component::OnInitialize();
        }

        float Waypoint::GetRadius()
        {
            return m_radius;
        }

        void Waypoint::SetRadius(float radius)
        {
            m_radius = radius;
        }

        float Waypoint::GetCost(void) const
        {
            return m_cost;
        }

        void Waypoint::SetCost(float cost)
        {
            m_cost = cost;
        }

        bool Waypoint::GetJumpPoint()
        {
            return m_jumpPoint;
        }

        void Waypoint::SetJumpPoint(bool isJumpPoint)
        {
            m_jumpPoint = isJumpPoint;
        }

        bool Waypoint::GetAutoWPConnect()
        {
            return m_autoWPConnect;
        }

        void Waypoint::SetAutoWPConnect(bool isAutoConnecting)
        {
            m_autoWPConnect = isAutoConnecting;
        }

        Vec3 Waypoint::GetPosition() const
        {
            return GetOwner()->GetTransform()->GetWorldPosition();
        }

        void Waypoint::AddWaypointConnection(ursine::ecs::Component::Handle<Waypoint> wp)
        {
            if (!wp)
                return;

            // calculate distance
            float distance = wp->GetOwner()->GetTransform()->GetWorldPosition().DistanceSquared(
                                GetOwner()->GetTransform()->GetWorldPosition()
                            );
            
            // add waypoint to our map of waypoints
            m_connectedWaypoints.Push(waypointPair(distance, wp));
        }

        void Waypoint::RemoveWaypointConnection(ursine::ecs::Component::Handle<Waypoint> wp)
        {
            if ( wp )
            {
                auto search = find_waypoint_pair( wp );

                if ( search != m_connectedWaypoints.end() )
                    m_connectedWaypoints.Remove( search );
            }
        }

        bool Waypoint::HasWaypointConnection(ursine::ecs::Component::Handle<Waypoint> wp)
        {
            auto mapIterator = find_waypoint_pair(wp);

            if (mapIterator == m_connectedWaypoints.end())
            {
                return false;
            }

            return true;
        }

        const waypointConList &Waypoint::GetConnectedWaypoints() const
        {
            return m_connectedWaypoints;
        }

        unsigned Waypoint::GetIndex() const
        {
            return m_index;
        }

        const std::string& Waypoint::GetGUID() const
        {
            return m_guid;
        }

        void Waypoint::SetGUID(const std::string &newGuid)
        {
            m_guid = newGuid;
        }

        waypointConList::Iterator Waypoint::find_waypoint_pair(ursine::ecs::Component::Handle<Waypoint> wp)
        {
            return std::find(
                m_connectedWaypoints.begin(),
                m_connectedWaypoints.end(),
                wp);
        }
    }
}
