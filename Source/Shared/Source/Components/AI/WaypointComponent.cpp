#include "Precompiled.h"

#include "WaypointComponent.h"
//#include "EntityEvent.h"

namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION( Waypoint );


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

        void Waypoint::AddWaypointConnection(const Waypoint* wp)
        {
            if (wp == nullptr)
                return;

            // calculate distance
            float distance = wp->GetOwner()->GetTransform()->GetWorldPosition().DistanceSquared(
                                GetOwner()->GetTransform()->GetWorldPosition()
                            );
            
            // add waypoint to our map of waypoints
            m_connectedWaypoints.emplace(wp, distance);
        }

        void Waypoint::RemoveWaypointConnection(const Waypoint* wp)
        {
            if (wp != nullptr)
            {
                auto search = m_connectedWaypoints.find( wp );

                if (search != m_connectedWaypoints.end())
                    m_connectedWaypoints.erase( search );
            }
        }

        bool Waypoint::HasWaypointConnection(const Waypoint* wp) const
        {
            auto mapIterator = m_connectedWaypoints.find(wp);

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
    }
}
