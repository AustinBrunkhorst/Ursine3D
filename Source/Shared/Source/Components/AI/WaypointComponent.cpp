#include "Precompiled.h"

#include "WaypointComponent.h"
//#include "EntityEvent.h"

namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION( Waypoint );


        bool Waypoint::waypointPair::operator==(const Handle<Waypoint> wp) const
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

        void Waypoint::AddWaypointConnection(const Handle<Waypoint> wp)
        {
            if (!wp)
                return;

            // calculate distance
            float distance = wp->GetOwner()->GetTransform()->GetWorldPosition().DistanceSquared(
                                GetOwner()->GetTransform()->GetWorldPosition()
                            );
            
            // add waypoint to our map of waypoints
            m_connectedWaypoints.push_back(waypointPair(distance, wp));
        }

        void Waypoint::RemoveWaypointConnection(const Handle<Waypoint> wp)
        {
            if ( wp )
            {
                auto search = find_waypoint_pair( wp );

                if ( search != m_connectedWaypoints.end() )
                    m_connectedWaypoints.erase( search );
            }
        }

        bool Waypoint::HasWaypointConnection(const Handle<Waypoint> wp) const
        {
            auto mapIterator = find_waypoint_pair(wp);

            if (mapIterator == m_connectedWaypoints.end())
            {
                return false;
            }

            return true;
        }

        const Waypoint::waypointConList &Waypoint::GetConnectedWaypoints() const
        {
            return m_connectedWaypoints;
        }

        unsigned Waypoint::GetIndex() const
        {
            return m_index;
        }

        Waypoint::waypointConList::const_iterator Waypoint::find_waypoint_pair(const Handle<Waypoint> wp) const
        {
            return std::find(
                m_connectedWaypoints.begin(),
                m_connectedWaypoints.end(),
                wp);
        }
    }
}
