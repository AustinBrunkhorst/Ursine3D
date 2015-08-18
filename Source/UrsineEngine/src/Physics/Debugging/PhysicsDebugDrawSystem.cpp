/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** PhysicsDebugDrawSystem.cpp
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"
#include "ZoneComponent.h"
#include "Entity.h"
#include "KeyboardManager.h"
#include "WindowManager.h"
#include <GL/glew.h>

namespace Ursine
{
    namespace ECS
    {
        // We want to draw after the render system draws
        PhysicsDebugDrawer::PhysicsDebugDrawer(World *world)
            : EntitySystem(world, -1001)
            , _draw_colliders(false)
            , _draw_aabbs(false)
            //, _draw_grid(false)
        {
            
        }

        PhysicsDebugDrawer::~PhysicsDebugDrawer(void)
        {
            
        }

        void PhysicsDebugDrawer::OnInitialize(void)
        {
            _world->Manager<EntityManager>()->Listener(this)
                .On(EM_ENTITY_ADDED, &PhysicsDebugDrawer::onEntityAdded)
                .On(EM_ENTITY_REMOVED, &PhysicsDebugDrawer::onEntityRemoved);

            _world->Manager<SystemManager>()->Listener(this)
                .On(SM_DRAW, &PhysicsDebugDrawer::onDraw);

            gKeyboardManager->Listener(this)
                .On(KM_KEY_DOWN, &PhysicsDebugDrawer::onKeyDown);

            _simulation = _world->GetEntitySystem(Simulation);
        }

        void PhysicsDebugDrawer::OnRemove(void)
        {
            _world->Manager<EntityManager>()->Listener(this)
                .Off(EM_ENTITY_ADDED, &PhysicsDebugDrawer::onEntityAdded)
                .Off(EM_ENTITY_REMOVED, &PhysicsDebugDrawer::onEntityRemoved);

            _world->Manager<SystemManager>()->Listener(this)
                .Off(SM_DRAW, &PhysicsDebugDrawer::onDraw);

            gKeyboardManager->Listener(this)
                .Off(KM_KEY_DOWN, &PhysicsDebugDrawer::onKeyDown);
        }

        void PhysicsDebugDrawer::onEntityAdded(EVENT_HANDLER(EntityManager))
        {
            EVENT_ATTRS(EntityManager, EntityEventArgs);

            if (args->entity->HasComponent<RigidBody2D>())
            {
                _bodies.push_back(args->entity);
            }
            else if (args->entity->HasComponent<Zone>())
            {
                _zones.push_back(args->entity);
            }
        }

        void PhysicsDebugDrawer::onEntityRemoved(EVENT_HANDLER(EntityManager))
        {
            EVENT_ATTRS(EntityManager, EntityEventArgs);

            if (args->entity->HasComponent<RigidBody2D>())
            {
                auto node = find(_bodies.begin(), _bodies.end(), args->entity);

                if (node != _bodies.end())
                    _bodies.erase(node);
            }
            else if (args->entity->HasComponent<Zone>())
            {
                auto node = find(_bodies.begin(), _bodies.end(), args->entity);

                if (node != _bodies.end())
                    _zones.erase(node);
            }
        }

        void PhysicsDebugDrawer::drawColliders(const std::vector<Collider*> colliders, const Mat3 &mat)
        {
            for (auto collider : colliders)
            {
                if (collider->shape->type == POLYGON)
                    drawPolygon(*static_cast<Polygon*>(collider->shape), collider->is_colliding, mat);
                else
                    drawEllipse(*static_cast<Ellipse*>(collider->shape), collider->is_colliding, mat);

                if (_draw_aabbs)
                    drawAABB(collider->aabb, mat);

                drawColliders(collider->children, mat);
            }
        }

        void PhysicsDebugDrawer::drawPolygon(const Polygon &polygon, bool is_colliding, const Mat3 &mat)
        {
            Vec2 temp[2];

            for(auto edge : polygon.edges)
            {
                temp[0] = mat.TransformPoint(edge.p0);
                temp[1] = mat.TransformPoint(edge.p1);

                if (is_colliding)
                    glColor3f(1.0f, 0.0f, 0.0f);
                else
                    glColor3f(1.0f, 1.0f, 1.0f);

                glVertex2f(temp[0].X(), temp[0].Y());
                glVertex2f(temp[1].X(), temp[1].Y());

                Vec2 mid_p = temp[0] + ((temp[1] - temp[0]) / 2.0f);
                Vec2 mid_plus_norm = mid_p + mat.TransformVector(0.5f * edge.norm);

                glColor3f(0.0f, 1.0f, 0.0f);
                glVertex2f(mid_p.X(), mid_p.Y());
                glVertex2f(mid_plus_norm.X(), mid_plus_norm.Y());
            }
        }

        void PhysicsDebugDrawer::drawEllipse(const Ellipse &ellipse, bool is_colliding, const Mat3 &mat)
        {
            static const float NUM_SEGMENTS = 30;
            static const float INC = Math::PI_2 / NUM_SEGMENTS;

            if (is_colliding)
                glColor3f(1.0f, 0.0f, 0.0f);
            else
                glColor3f(1.0f, 1.0f, 1.0f);

            float theta = 0;
            Vec2 position = ellipse. _position;
            Vec2 temp[2];

            for (int i = 0; i < NUM_SEGMENTS; ++i)
            {
                temp[0] = mat.TransformPoint(ellipse.GetSupport(Mat3(theta).TransformVector({ 1, 0 })));
                theta += INC;
                temp[1] = mat.TransformPoint(ellipse.GetSupport(Mat3(theta).TransformVector({ 1, 0 })));

                glVertex2f(temp[0].X(), temp[0].Y());
                glVertex2f(temp[1].X(), temp[1].Y());
            }

            // draw from inside to outside
            temp[0] = mat.TransformPoint(position);
            position = position + Mat3(ellipse._rotation).TransformVector({ ellipse._dimensions.X(), 0 });
            temp[1] = mat.TransformPoint(position);

            glVertex2f(temp[0].X(), temp[0].Y());
            glVertex2f(temp[1].X(), temp[1].Y());
        }

        void PhysicsDebugDrawer::drawContactPoints(const Mat3 &mat)
        {
            glBegin(GL_POINTS);

            for (auto &point : _contacts)
            {
                Vec2 new_point = mat.TransformPoint(point);

                glVertex2f(new_point.X(), new_point.Y());
            }
        }

        void PhysicsDebugDrawer::drawJoints(const Mat3 &mat)
        {
            const float dist_line_len = 1.0f;

            glColor3f(0.0f, 1.0f, 0.0f);

            Vec2 temp[6];

            for (uint i = 0, size = _simulation->_joints.size(); i < size; ++i)
            {
                DistanceJoint *joint = dynamic_cast<DistanceJoint*>(_simulation->_joints[i]);

                if (joint != nullptr)
                {
                    temp[0] = mat.TransformPoint(joint->_trans_anchor[0]);
                    temp[1] = mat.TransformPoint(joint->_trans_anchor[1]);

                    glVertex2f(temp[0].X(), temp[0].Y());
                    glVertex2f(temp[1].X(), temp[1].Y());

                    temp[2] = joint->_trans_anchor[0];
                    temp[3] = joint->_trans_anchor[1];

                    float distance = Vec2::Distance(temp[2], temp[3]);

                    if (distance > joint->_distance)
                    {
                        // draw the distance lines for the joint
                        Vec2 dir = temp[3] - temp[2];
						dir.Normalize();

                        float diff = distance - joint->_distance;
                        diff /= 2.0f;

                        temp[2] = temp[2] + (dir * diff);
                        temp[3] = temp[3] - (dir * diff);

                        // get the tangent vector
                        dir = Vec2::Cross(1.0f, dir);

                        temp[4] = mat.TransformPoint(temp[2] + (dir * dist_line_len));
                        temp[5] = mat.TransformPoint(temp[2] - (dir * dist_line_len));

                        glVertex2f(temp[4].X(), temp[4].Y());
                        glVertex2f(temp[5].X(), temp[5].Y());

                        temp[4] = mat.TransformPoint(temp[3] + (dir * dist_line_len));
                        temp[5] = mat.TransformPoint(temp[3] - (dir * dist_line_len));

                        glVertex2f(temp[4].X(), temp[4].Y());
                        glVertex2f(temp[5].X(), temp[5].Y());
                    }

                    continue;
                }

                // Hinge Joint Logic
            }
        }

        void PhysicsDebugDrawer::drawRays(const Mat3 &mat)
        {
            const float dt = _world->GetDelta();
            static const float normal_size = 1;

            glColor3f(0.0f, 0.4f, 1.0f);

            Vec2 temp[3];

            auto &ray = _rays.begin();

            while (ray != _rays.end())
            {
                Vec2 dir = ray->input.p2 - ray->input.p1;
                Vec2 end_p = ray->input.p1 + dir * ray->output.fraction;

                temp[0] = mat.TransformPoint(ray->input.p1);
                temp[1] = mat.TransformPoint(end_p);
                temp[2] = mat.TransformPoint(end_p + (ray->output.normal * normal_size));

                glColor3f(0.0f, 0.4f, 1.0f);
                glVertex2f(temp[0].X(), temp[0].Y());
                glVertex2f(temp[1].X(), temp[1].Y());

                glColor3f(1.0f, 1.0f, 1.0f);
                glVertex2f(temp[1].X(), temp[1].Y());
                glVertex2f(temp[2].X(), temp[2].Y());

                ray->time += dt;

                if (ray->time >= RAY_CAST_DEBUG_TIMER)
                    ray = _rays.erase(ray);
                else
                    ++ray;
            }
        }

        void PhysicsDebugDrawer::drawAABB(const AABB &aabb, const Mat3 &mat)
        {
            // Draw the bounding box
            const Vec2 &up_bound = aabb.up_bound;
            const Vec2 &low_bound = aabb.low_bound;
            
            Vec2 temp[4];
            temp[0] = mat.TransformPoint(up_bound);
            temp[1] = mat.TransformPoint({ up_bound.X(), low_bound.Y() });
            temp[2] = mat.TransformPoint(low_bound);
            temp[3] = mat.TransformPoint({ low_bound.X(), up_bound.Y() });

            glColor3f(0.0f, 0.0f, 1.0f);

            for (int i = 0; i < 4; ++i)
            {
                int j = i + 1 == 4 ? 0 : i + 1;
                glVertex2f(temp[i].X(), temp[i].Y());
                glVertex2f(temp[j].X(), temp[j].Y());
            }
        }

        void PhysicsDebugDrawer::drawCenterOfMass(const Vec2 &COM, const Mat3 &mat)
        {
            Vec2 temp_points[4] =
            { mat.TransformPoint(COM + Vec2(0.2f, 0))
            , mat.TransformPoint(COM - Vec2(0.2f, 0))
            , mat.TransformPoint(COM + Vec2(0, 0.2f))
            , mat.TransformPoint(COM - Vec2(0, 0.2f)) };

            glColor3f(0.0f, 0.0f, 1.0f);
            for (int i = 0; i < 4; ++i)
                glVertex2f(temp_points[i].X(), temp_points[i].Y());
        }

        void PhysicsDebugDrawer::drawGrid(Camera2D &camera)
        {
            Vec2 size = gWindowManager->GetSize();
            Vec2 top_l = camera.ScreenToWorld({ 0, 0 }),
                    bot_r = camera.ScreenToWorld(size);
            const Mat3 &matrix = camera.GetWorldToNDC();

            int min_x = (int)top_l.X(),
                max_x = (int)bot_r.X(),
                min_y = (int)bot_r.Y(),
                max_y = (int)top_l.Y();


            glColor3f(0.9f, 0.9f, 0.9f);
            glBegin(GL_LINES);
            glEnable(GL_LINE_SMOOTH);


            Vec2 p0, p1;
            Vec2 temp[2];

            p0.Y() = max_y + 1.0f; p1.Y() = min_y - 1.0f;

            for (int i = min_x; i <= max_x; ++i)
            {
                p0.X() = p1.X() = (float)i;

                temp[0] = matrix.TransformPoint(p0);
                temp[1] = matrix.TransformPoint(p1);

                glVertex2f(temp[0].X(), temp[0].Y());
                glVertex2f(temp[1].X(), temp[1].Y());
            }

            p0.X() = min_x - 1.0f; p1.X() = max_x + 1.0f;

            for (int i = min_y; i <= max_y; ++i)
            {
                p0.Y() = p1.Y() = (float)i;

                temp[0] = matrix.TransformPoint(p0);
                temp[1] = matrix.TransformPoint(p1);

                glVertex2f(temp[0].X(), temp[0].Y());
                glVertex2f(temp[1].X(), temp[1].Y());
            }

            glColor3f(1.0f, 1.0f, 1.0f);
            glEnd();
        }

        void PhysicsDebugDrawer::onDraw(EVENT_HANDLER(SystemManager))
        {
            /*if (_draw_grid)
            {
                drawGrid(_world->GetCamera());
            }*/

            if (_draw_colliders)
            {
                glColor3f(1.0f, 1.0f, 1.0f);

                glDisable(GL_TEXTURE_2D);

                glBegin(GL_LINES);
                glEnable(GL_LINE_SMOOTH);

                const Mat3 &matrix = _world->GetCamera().GetWorldToNDC();

                // draw rigid bodies
                for (auto entity : _bodies)
                {
                    auto body = entity->GetComponent<RigidBody2D>();

                    drawColliders(body->colliders, matrix);

                    drawCenterOfMass(body->GetCenterOfMass(), matrix);

                    if (_draw_aabbs)
                        drawAABB(body->_aabb, matrix);
                }

                drawJoints(matrix);

                drawRays(matrix);

                // draw zones
                glColor3f(0.0f, 1.0f, 1.0f);

                for (auto entity : _zones)
                {
                    auto zone = entity->GetComponent<Zone>();

                    drawAABB(zone->_aabb, matrix);
                }
                glColor3f(1.0f, 1.0f, 1.0f);
                glEnd();

                glPointSize(4.0f);
                glBegin(GL_POINTS);

                // draw contacts
                drawContactPoints(matrix);

                glEnd();
                glPointSize(1.0f);

                glEnable(GL_TEXTURE_2D);

                _contacts.clear();
            }
        }

        void PhysicsDebugDrawer::addContactPoint(const Vec2 &point)
        {
            //_contacts.push_back(point);
        }

        void PhysicsDebugDrawer::onKeyDown(EVENT_HANDLER(KeyboardManager))
        {
            EVENT_ATTRS(KeyboardManager, KeyboardKeyArgs);

            if (args->key == KEY_TILDE)
            {
                if (!_draw_colliders)
                    _draw_colliders = true;
                else if (_draw_colliders && !_draw_aabbs)
                    _draw_aabbs = true;
                else if (_draw_colliders && _draw_aabbs)
                    _draw_aabbs = _draw_colliders = false;
                /*else if (_draw_colliders && _draw_aabbs && !_draw_grid)
                    _draw_grid = true;
                else if (_draw_colliders && _draw_aabbs)
                    _draw_aabbs = _draw_colliders = _draw_grid = false;*/
            }
        }

        void PhysicsDebugDrawer::addRayCast(const RayCastOutput &output, const RayCastInput &input)
        {
            if (_draw_colliders)
                _rays.emplace_back(output, input);
        }
    }
}