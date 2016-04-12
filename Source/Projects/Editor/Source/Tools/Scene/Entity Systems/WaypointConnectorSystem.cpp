#include "Precompiled.h"

#include "WaypointConnectorSystem.h"

#include <SelectedComponent.h>
#include <KeyboardManager.h>
#include <AIMovementControllerComponent.h>

namespace ursine
{
    namespace ecs
    {

        ENTITY_SYSTEM_DEFINITION(WaypointConnectorSystem);

        WaypointConnectorSystem::WaypointConnectorSystem(ursine::ecs::World *world)
            : EntitySystem(world)
            , m_startWP(nullptr)
            , m_keyboardManager(GetCoreSystem(KeyboardManager))
            , m_gfx(GetCoreSystem(graphics::GfxAPI))
            , m_mode(MODE_INACTIVE)
            , m_showLines(true)
        {

        }

        void WaypointConnectorSystem::SetMode(Mode newMode)
        {
            m_mode = newMode;
        }

        void WaypointConnectorSystem::SetShowLines(bool draw)
        {
            m_showLines = draw;
        }

        void WaypointConnectorSystem::OnInitialize()
        {
            //m_keyboardManager = GetCoreSystem(KeyboardManager);

            m_world->Listener(this)
                .On(WORLD_ENTITY_COMPONENT_ADDED, &WaypointConnectorSystem::onComponentAdded)
                .On(WORLD_ENTITY_COMPONENT_REMOVED, &WaypointConnectorSystem::onComponentRemoved)
                .On(WORLD_EDITOR_UPDATE, &WaypointConnectorSystem::onUpdate);

            m_keyboardManager->Listener(this)
                .On(KM_KEY_DOWN, &WaypointConnectorSystem::onKeyDown)
                .On(KM_KEY_UP, &WaypointConnectorSystem::onKeyUp);

            m_lineColor = Vec4(0.0f, 1.0f, 0.3f, 1.0f);

            //we need to check for any pre-existing connections on the level
            //grabExistingConnections();
        }

        void WaypointConnectorSystem::OnSceneReady(Scene *scene)
        {
            grabExistingConnections();
        }

        void WaypointConnectorSystem::OnRemove()
        {
            m_world->Listener(this)
                .Off(WORLD_ENTITY_COMPONENT_ADDED, &WaypointConnectorSystem::onComponentAdded)
                .Off(WORLD_ENTITY_COMPONENT_REMOVED, &WaypointConnectorSystem::onComponentRemoved)
                .Off(WORLD_EDITOR_UPDATE, &WaypointConnectorSystem::onUpdate);

            m_keyboardManager->Listener(this)
                .Off(KM_KEY_DOWN, &WaypointConnectorSystem::onKeyDown)
                .Off(KM_KEY_UP, &WaypointConnectorSystem::onKeyUp);
        }

        void WaypointConnectorSystem::onComponentAdded(EVENT_HANDLER(World))
        {
            EVENT_ATTRS(World, ComponentEventArgs);

            if (args->component->Is<Selected>() && m_mode != MODE_INACTIVE)
            {
                auto *selected = static_cast<Selected *>( const_cast<Component*>( args->component ) );
                
                if (selected->GetOwner()->HasComponent<Waypoint>())
                {
                    auto *newWP = selected->GetOwner()->GetComponent<Waypoint>();
                    if (m_startWP == nullptr)
                    {
                        m_startWP = newWP;
                    }
                    else if(m_startWP != newWP)
                    {
                        makeConnection(newWP);
                    }
                }
                //else
                //{
                //    m_mode = MODE_INACTIVE;
                //}
            }
            else if(args->component->Is<Selected>())
            {
                auto kineControl = m_world->GetEntitiesFromFilter(Filter().All<AIMovementController>());


                if (kineControl.size() > 0)
                {
                    Vec3 dir = Vec3(args->entity->GetTransform( )->GetWorldPosition( )
                                - kineControl[0]->GetTransform( )->GetWorldPosition( ));

                    kineControl[0]->GetComponent<AIMovementController>()->SetTargetDirection( dir );
                }
            }
        }

        void WaypointConnectorSystem::onComponentRemoved(EVENT_HANDLER(World))
        {
            EVENT_ATTRS(World, ComponentEventArgs);

            if (args->component->Is<Waypoint>())
            {
                if (m_startWP == static_cast<Waypoint *>(const_cast<Component*>(args->component)))
                {
                    if (!m_connectedWaypoints.empty())
                    {
                        m_connectedWaypoints.remove(m_startWP);
                    }

                    m_startWP = nullptr;
                }
            }
        }

        void WaypointConnectorSystem::onUpdate(EVENT_HANDLER(World))
        {
            if (m_connectedWaypoints.empty())
            {
                grabExistingConnections();
            }

            if (m_showLines)
            {
                // go through list of stored connections and draw them
                for (auto wp : m_connectedWaypoints)
                {
                    for (auto conWP : wp->GetConnectedWaypoints())
                    {
                        drawConnection(wp->GetOwner()->GetTransform()->GetWorldPosition()
                            , conWP.waypoint->GetOwner()->GetTransform()->GetWorldPosition());
                    }
                }
            }
        }

        void WaypointConnectorSystem::onKeyUp(EVENT_HANDLER(World))
        {
            EVENT_ATTRS(KeyboardManager, KeyboardKeyArgs);

            //shift is for doing jump points
            /*
            if (args->key == KEY_SHIFT)
            {
                m_mode = MODE_INACTIVE;
            }
            */
        }

        void WaypointConnectorSystem::onKeyDown(EVENT_HANDLER(World))
        {
            EVENT_ATTRS(KeyboardManager, KeyboardKeyArgs);

            //shift is for doing jump points
            /*
            if (args->key == KEY_SHIFT)
            {
                m_mode = MODE_BIDIRECTIONAL;
            }
            */
        }

        void WaypointConnectorSystem::makeConnection(Waypoint* endWP)
        {
            // just return out if we have already added this connection
            if (m_startWP->HasWaypointConnection(endWP))
            {
                return;
            }
            if (!hasStartAlready())
            {
                m_connectedWaypoints.push_back(m_startWP);
            }

            m_startWP->AddWaypointConnection(endWP);

            if (m_mode == MODE_BIDIRECTIONAL)
            {
                endWP->AddWaypointConnection(m_startWP);
            }
            m_startWP = nullptr;
        }

        void WaypointConnectorSystem::drawConnection(SVec3 start, SVec3 end)
        {
            //use set overdraw if I need everything to be above the objects10
            m_gfx->DrawingMgr.SetOverdraw(true);
            m_gfx->DrawingMgr.SetColor(m_lineColor);
            m_gfx->DrawingMgr.DrawLine(start, end);
            m_gfx->DrawingMgr.SetOverdraw(false);
        }

        bool WaypointConnectorSystem::hasStartAlready()
        {
            for (auto wp : m_connectedWaypoints)
            {
                if (wp == m_startWP)
                {
                    return true;
                }
            }
            return false;
        }

        void WaypointConnectorSystem::grabExistingConnections(void)
        {
            Filter filter;

            auto waypointEntities = m_world->GetEntitiesFromFilter(filter.All<Waypoint>());

            for (auto waypointEntity : waypointEntities)
            {
                auto wp = waypointEntity->GetComponent<Waypoint>();

                // creating one
                auto guid = GUIDGenerator( )( );

                // string representation
                auto guidString = to_string( guid );

                auto fromString = GUIDStringGenerator( )( guidString );
                

                auto connections = wp->GetConnectedWaypoints();

                for (auto connection : connections)
                {
                    m_connectedWaypoints.push_back(wp);
                }
            }
        }

    }
}
