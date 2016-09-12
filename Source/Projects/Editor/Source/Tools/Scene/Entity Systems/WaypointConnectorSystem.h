/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** WaypointConnectorSystem.h
**
** Author:
** - Joshua Shlemmer - joshua.shlemmer@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <EntitySystem.h>
#include <WaypointComponent.h>
#include <KeyboardManager.h>
#include <GfxAPI.h>
#include <Vec4.h>


namespace ursine
{
    namespace ecs
    {
        class WaypointConnectorSystem : public ursine::ecs::EntitySystem
        {
            ENTITY_SYSTEM;

        public:
            enum Mode
            {
                MODE_UNIDIRECTIONAL,
                MODE_BIDIRECTIONAL,
                MODE_JUMP,
                MODE_INACTIVE
            };

            WaypointConnectorSystem(ursine::ecs::World *world);

            void SetMode(Mode newMode);

            void SetShowLines(bool draw);

            void OnSceneReady(Scene *scene) override;

        private:

            void OnInitialize(void) override;
            void OnRemove(void) override;

            void onComponentAdded(EVENT_HANDLER(World));
            void onComponentRemoved(EVENT_HANDLER(World));

            void onUpdate(EVENT_HANDLER(World));

            void onKeyUp(EVENT_HANDLER(World));
            void onKeyDown(EVENT_HANDLER(World));

            void makeConnection(Waypoint *endWP);

            void drawConnection(SVec3 start, SVec3 end);

            bool hasStartAlready(void);

            void grabExistingConnections(void);

            std::list<Waypoint *> m_connectedWaypoints;

            Vec4 m_lineColor;

            Waypoint *m_startWP;

            KeyboardManager *m_keyboardManager;

            graphics::GfxAPI *m_gfx;

            Mode m_mode;

            bool m_showLines;

        } Meta(Enable, AutoAddEntitySystem);
    }
}