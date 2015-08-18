/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** PhysicsDebugDrawSystem.h
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

////////////////////////////////////////////////////////////////////////////////
#ifndef PHYSICS_DEBUG_DRAW_SYSTEM_H
#define PHYSICS_DEBUG_DRAW_SYSTEM_H
////////////////////////////////////////////////////////////////////////////////

#include "FilterSystem.h"
#include "EntityManager.h"

namespace Ursine
{
    // Forward declaration
    class Polygon;
    class Ellipse;

    namespace ECS
    {
        // amount of time the ray is drawn on screen
        #define RAY_CAST_DEBUG_TIMER 2.0f

        // Forward declaration
        class Simulation;

        struct RayCastDebugInfo
        {
            RayCastDebugInfo(const RayCastOutput &output, const RayCastInput &input)
                : output(output)
                , input(input)
                , time(0.0f) 
            { 

            }

            RayCastOutput output;
            RayCastInput input;
            float time;
        };

        class PhysicsDebugDrawer 
            : public EntitySystem
            , public Utils::NoCopy
        {
            bool _draw_colliders;
            bool _draw_aabbs;
            //bool _draw_grid;

            void onEntityAdded(EVENT_HANDLER(EntityManager));
            void onEntityRemoved(EVENT_HANDLER(EntityManager));

            void onDraw(EVENT_HANDLER(SystemManager));

            void onKeyDown(EVENT_HANDLER(KeyboardManager));

            Simulation *_simulation;

            std::vector<Entity *> _bodies;
            std::vector<Entity *> _zones;
            std::vector<Vec2> _contacts;

            std::vector<RayCastDebugInfo> _rays;

        public:
            PhysicsDebugDrawer(World *world);
            ~PhysicsDebugDrawer(void);

            void drawColliders(Colliders colliders, const Mat3 &mat);
            void drawPolygon(const Polygon &polygon, bool is_colliding, const Mat3 &mat);
            void drawEllipse(const Ellipse &ellipse, bool is_colliding, const Mat3 &mat);
            void drawContactPoints(const Mat3 &mat);
            void drawJoints(const Mat3 &mat);
            void drawRays(const Mat3 &mat);
            void drawAABB(const AABB &aabb, const Mat3 &mat);
            void drawCenterOfMass(const Vec2 &COM, const Mat3 &mat);
            void drawGrid(Camera2D &camera);

            void addContactPoint(const Vec2 &point);
            void addRayCast(const RayCastOutput &output, const RayCastInput &input);

            void OnInitialize(void) override;
            void OnRemove(void) override;
        };
    }
}

////////////////////////////////////////////////////////////////////////////////
#endif // !PHYSICS_DEBUG_DRAW_SYSTEM_H
////////////////////////////////////////////////////////////////////////////////

#ifdef REGISTER_SYSTEMS

#ifndef PHYSICS_DEBUG_DRAW_REGISTERED
#define PHYSICS_DEBUG_DRAW_REGISTERED

namespace Ursine
{
    namespace ECS
    {
        RegisterEntitySystem(PhysicsDebugDrawer)
    }
}

#endif

#endif
