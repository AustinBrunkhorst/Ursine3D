/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** WaypointComponent.h
**
** Author:
** - Joshua Shlemmer - joshua.shlemmer@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once
#include <unordered_map>

#include "Component.h"
#include "Vec3.h"

namespace ursine
{
    namespace ecs
    {
        class Waypoint;

        struct waypointPair
        {
            waypointPair(float wpCost, ursine::ecs::Component::Handle<ursine::ecs::Waypoint> wp) : cost(wpCost), waypoint(wp) { }

            bool operator==(const ursine::ecs::Component::Handle<ursine::ecs::Waypoint> wp) const;

            float cost;

            ursine::ecs::Component::Handle<ursine::ecs::Waypoint> waypoint;
        };

        typedef ursine::Array<waypointPair> waypointConList;

        class Waypoint : public Component
        {
            NATIVE_COMPONENT;

            friend class WaypointSystem;

        public:

            //void OnSerialize(Json &output) override;

            EditorField(
                float Radius,
                GetRadius,
                SetRadius
            );

            EditorField(
                float Cost,
                GetCost,
                SetCost
            );

            EditorField(
                bool JumpPoint,
                GetJumpPoint,
                SetJumpPoint
            );

            EditorField(
                bool AutoConnectWaypoints,
                GetAutoWPConnect,
                SetAutoWPConnect
            );

            Meta(Enable)
            Waypoint(void);
            //~Waypoint(void);

            //Meta(Disable)
            void OnInitialize(void) override;

            ////////////////////////////////////////////////////////////////////
            // Editor Variables Get/Set
            ////////////////////////////////////////////////////////////////////
            Meta(Enable)
            float GetRadius(void);

            Meta(Enable)
            void SetRadius(float radius);


            Meta(Enable)
            float GetCost(void) const;

            Meta(Enable)
            void SetCost(float cost);


            Meta(Enable)
            bool GetJumpPoint(void);

            Meta(Enable)
            void SetJumpPoint(bool isJumpPoint);


            Meta(Enable)
            bool GetAutoWPConnect(void);

            Meta(Enable)
            void SetAutoWPConnect(bool isAutoConnecting);


            Meta(Enable)
            Vec3 GetPosition(void) const;

            ////////////////////////////////////////////////////////////////////
            // Functions for waypoint connections
            ////////////////////////////////////////////////////////////////////
            Meta(Enable)
            void Waypoint::AddWaypointConnection(ursine::ecs::Component::Handle<ursine::ecs::Waypoint> wp);
            
            Meta(Enable)
            void RemoveWaypointConnection(ursine::ecs::Component::Handle<ursine::ecs::Waypoint> wp);

            Meta(Enable)
            bool HasWaypointConnection(ursine::ecs::Component::Handle<ursine::ecs::Waypoint> wp);

            Meta(Disable)
            const waypointConList &GetConnectedWaypoints(void) const;

            Meta(Disable)
            unsigned GetIndex(void) const;

            Meta(Enable)
            const std::string &GetGUID(void) const;

            Meta(Enable)
            void SetGUID(const std::string &newGuid);

            Meta(Disable)
            waypointConList m_connectedWaypoints;

            // this is used to identify for serialization
            Meta(Enable)
            std::string m_guid;
        private:

            waypointConList::Iterator Waypoint::find_waypoint_pair(ursine::ecs::Component::Handle<Waypoint> wp);

            ursine::Array<boost::uuids::uuid> m_connectedGuids;



            float m_cost;

            float m_radius;

            unsigned m_index;

            bool m_jumpPoint;

            bool m_autoWPConnect;

        } Meta(Enable, WhiteListMethods, DisplayName( "Waypoint" ));

    }
}
