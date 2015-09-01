/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SimulationSystem.cpp
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

namespace ursine
{
    namespace ecs
    {
        Simulation::Simulation(World *world)
            : EntitySystem(world, SIMULATION_PRIORITY)
            , _dt(0.0f)
        {

        }

        Simulation::~Simulation(void)
        {

        }

        void Simulation::OnInitialize()
        {
            _world->Manager<EntityManager>()->Listener(this)
                .On(EM_COMPONENT_ADDED, &Simulation::onComponentAdded)
                .On(EM_COMPONENT_REMOVED, &Simulation::onComponentRemoved);

            _world->Manager<SystemManager>()->Listener(this)
                .On(SM_UPDATE, &Simulation::onUpdate);

            _debug_drawer = _world->GetEntitySystem(PhysicsDebugDrawer);
        }

        void Simulation::OnRemove()
        {
            _world->Manager<EntityManager>()->Listener(this)
                .Off(EM_COMPONENT_ADDED, &Simulation::onComponentAdded)
                .Off(EM_COMPONENT_REMOVED, &Simulation::onComponentRemoved);

            _world->Manager<SystemManager>()->Listener(this)
                .Off(SM_UPDATE, &Simulation::onUpdate);
        }

        DistanceJoint *Simulation::AddDistanceJoint(RigidBody2D *body_0, RigidBody2D *body_1,
                                                    float distance, bool max_distance_only)
        {
            _joints.emplace_back(new DistanceJoint(body_0, body_1, distance, max_distance_only));
            
            return reinterpret_cast<DistanceJoint*>(_joints.back());
        }

        void Simulation::RemoveJoint(RigidBody2D *body_0, RigidBody2D *body_1)
        {
            auto &joint = _joints.begin();

            while (joint != _joints.end())
            {
                if (((*joint)->_body[0] == body_0 && (*joint)->_body[1] == body_1) ||
                    ((*joint)->_body[0] == body_1 && (*joint)->_body[1] == body_0))
                {
                    delete *joint;
                    joint = _joints.erase(joint);
                }
                else
                {
                    ++joint;
                }
            }
        }

        void Simulation::RemoveJoints(RigidBody2D *body)
        {
            auto &joint = _joints.begin();

            while (joint != _joints.end())
            {
                if ((*joint)->_body[0] == body || (*joint)->_body[1] == body)
                {
                    delete *joint;
                    joint = _joints.erase(joint);
                }
                else
                {
                    ++joint;
                }
            }
        }

        void Simulation::onComponentAdded(EVENT_HANDLER(EntityManager))
        {
            EVENT_ATTRS(EntityManager, ComponentEventArgs);

            // if the component added is a rigid body, add it to the vector
            if (args->component->Is<RigidBody2D>())
            {
                _bodies.push_back(args->entity->GetComponent<RigidBody2D>());
            }
        }

        void Simulation::onComponentRemoved(EVENT_HANDLER(EntityManager))
        {
            EVENT_ATTRS(EntityManager, ComponentEventArgs);

            // if the component removed is a rigid body, remove it from the vector
            if (args->component->Is<RigidBody2D>())
            {
                auto body = args->entity->GetComponent<RigidBody2D>();

                auto itr = std::find(_bodies.begin(), _bodies.end(), body);
                
                if (itr != _bodies.end())
                _bodies.erase(
                    std::find(_bodies.begin(), 
                              _bodies.end(), 
                              body)
                    );

                auto id = args->entity->GetUniqueID();
                std::vector<ManifoldKey> remove_queue;

                for (auto it : _manifolds)
                {
                    auto &key = it.first;

                    if (key.a == id || key.b == id)
                        remove_queue.push_back(key);
                }
                
                for (auto key : remove_queue)
                    _manifolds.erase(key);

                RemoveJoints(body);
            }
        }

        void Simulation::generateManifolds(void)
        {
            // set collision to false initially
            URSINE_TODO("Physics: These are needless iterations, find a way around this.");
            for (uint i = 0, size = _bodies.size();
                i < size; ++i)
            {
                Collision::SetCollisionFalse(_bodies[i]->colliders);
            }

            for (uint i = 0, size = _bodies.size();
                i < size; ++i)
            {
                RigidBody2D *a = _bodies[i];

                for (uint j = i + 1; j < size; ++j)
                {
                    RigidBody2D *b = _bodies[j];

                    // Continue if both are part of an ignored pair
                    if (GetIgnoreCollision(a, b))
                        continue;

                    // Continue if one objects collision mask matches the others id
                    if (!a->layer_mask.Contains(b->layer) &&
                        !b->layer_mask.Contains(a->layer))
                        continue;

                    // Continue if both are either kinematic or static bodies
                    if (a->_inv_mass == 0.0f && b->_inv_mass == 0.0f)
                    {
                        // Check to see if we need to use the kinematic solver
                        if (a->body_type == BODY_TYPE_KINEMATIC_SOLVE ||
                            b->body_type == BODY_TYPE_KINEMATIC_SOLVE)
                        {
                            // generate a manifold for this pair
                            KinematicManifold manifold(a, b);

                            // if there are contacts
                            if (manifold.contacts.size())
                            {
                                // tell the manifold to positionally corect them since
                                // one or more objects are kinematic solvers
                                manifold.Solve();
                            }
                        }

                        // continue onto the next body pair and throw away the
                        // manifold we created above, since it's not being iterated over
                        continue;
                    }

                    Manifold new_manifold(a, b);
                    ManifoldKey new_key(
                        a->GetOwner()->GetUniqueID(), 
                        b->GetOwner()->GetUniqueID()
                    );
                    
                    auto it = _manifolds.find(new_key);

                    // Contact between the two bodies was found
                    if (new_manifold.contacts.size())
                    {
                        if (it != _manifolds.end())
                        {
                            // Send collision persisted event
                            Collision::SendEvents(a, b, new_manifold, COLLISION_PERSISTED);

                            it->second.Update(new_manifold.contacts);
                        }
                        else
                        {
                            // Send collision started event
                            Collision::SendEvents(a, b, new_manifold, COLLISION_STARTED);

                            _manifolds.insert(ManifoldPair(new_key, new_manifold));
                        }

                        // Send contact points to the physics debug drawer
                        for (auto &contact : new_manifold.contacts)
                        {
                            _debug_drawer->addContactPoint(contact.point);
                        }
                    }
                    else
                    {
                        // The manifold is contained within the map
                        if (it != _manifolds.end())
                        {
                            // Send collision ended event
                            Collision::SendEvents(a, b, new_manifold, COLLISION_ENDED);

                            // Erase the key value pair
                            _manifolds.erase(it);
                        }
                    }
                }
            }
        }

        void Simulation::integrateForces(void)
        {
            for (uint i = 0, size = _bodies.size();
                 i < size; ++i)
            {
                RigidBody2D *body = _bodies[i];

                if (body->body_type != BODY_TYPE_DYNAMIC)
                    continue;

                body->SetVelocity(
                    body->_velocity + 
                    _dt * body->_inv_mass * body->_force
                );

                body->SetAngularVelocity(
                    body->_angular_velocity + 
                    _dt * body->_inv_inertia * body->_torque
                );
            }
        }

        void Simulation::preStep(void)
        {
            float inv_dt = _dt == 0.0f ? 0.0f : 1.0f / _dt;

            for (auto &it : _manifolds)
            {
                if (it.second.a->body_type != BODY_TYPE_GHOST &&
                    it.second.b->body_type != BODY_TYPE_GHOST)
                    it.second.PreStep(inv_dt);
            }

            for (auto &it : _joints)
            {
                it->PreStep();
            }
        }

        void Simulation::applyImpulse()
        {
            for (int i = 0; i < IMPULSE_ITERATIONS; ++i)
            {
                for (auto &it : _manifolds)
                {
                    if (it.second.a->body_type != BODY_TYPE_GHOST &&
                        it.second.b->body_type != BODY_TYPE_GHOST)
                        it.second.ApplyImpulse();
                }

                for (auto &it : _joints)
                {
                    it->Solve(_dt);
                }
            }
        }

        void Simulation::integrateVelocities(void)
        {
            for (uint i = 0, size = _bodies.size(); i < size; ++i)
            {
                RigidBody2D *body = _bodies[i];
                Transform *trans = body->_transform;

                if (body->body_type == BODY_TYPE_STATIC)
                {
                    // Update colliders
                    body->UpdateColliders();
                    continue;
                }

                // Apply velocities
                if (!body->_pos_locked)
                    trans->AddPosition(SVec3(_dt * body->_velocity, 0));

                if (!body->_rot_locked)
                    trans->AddRotation2D(_dt * body->_angular_velocity);

                // Update colliders
                body->UpdateColliders();

                // Reset force and torque
                body->_force.Set(0.0f, 0.0f);
                body->_torque = 0.0f;
            }
        }

        void Simulation::postStep(void)
        {
            for (auto &it : _joints)
            {
                it->PostStep();
            }
        }

        void Simulation::onUpdate(EVENT_HANDLER(SystemManager))
        {
            // Determine dt
            _dt = math::Min(DT, _world->GetDelta());

            // Step (1) Collision detection
            generateManifolds();

            // Step (2) Integrate forces
            integrateForces();

            // Step (3) Pre-step
            preStep();

            // Step (4) Apply impulse
            applyImpulse();

            // Step (5) Integrate velocities
            integrateVelocities();

            // Step (6) Post Step
            postStep();
        }

        bool Simulation::RayCast(RayCastOutput &output, const RayCastInput &input, 
                                 const LayerMask &mask)
        {
            return RayCast(output, input, mask, [=](RigidBody2D *body) { return true; });
        }

        bool Simulation::RegionQuery(const AABB &region, std::vector<Entity *> &entities)
        {
            bool result = false;
            RayCastInput top;
            RayCastInput bot;
            RayCastInput left;
            RayCastInput right;
            RayCastOutput out;

            top.p1 = { region.low_bound.X(), region.up_bound.Y() };
            top.p2 = { region.up_bound.X(), region.up_bound.Y() };
            top.max_fraction = 1.0f;

            bot.p1 = { region.low_bound.X(), region.low_bound.Y() };
            bot.p2 = { region.up_bound.X(), region.low_bound.Y() };
            bot.max_fraction = 1.0f;

            left.p1 = { region.low_bound.X(), region.up_bound.Y() };
            left.p2 = { region.low_bound.X(), region.low_bound.Y() };
            left.max_fraction = 1.0f;
            
            right.p1 = { region.up_bound.X(), region.up_bound.Y() };
            right.p2 = { region.up_bound.X(), region.low_bound.Y() };
            right.max_fraction = 1.0f;

            // iterate through all bodies
            for (unsigned i = 0, size = _bodies.size(); i < size; ++i)
            {
                // if the body's AABB intersects the regions aabb
                if (_bodies[i]->_aabb.Overlap(region))
                {
                    if (ColliderRayCast(_bodies[i]->colliders, out, top) ||
                        ColliderRayCast(_bodies[i]->colliders, out, bot) ||
                        ColliderRayCast(_bodies[i]->colliders, out, left) ||
                        ColliderRayCast(_bodies[i]->colliders, out, right))
                    {
                        entities.push_back(_bodies[i]->GetOwner());
                        result = true;
                    }
                }
            }
            
            return result;
        }

        void Simulation::SetIgnoreCollision(RigidBody2D *body_0, RigidBody2D *body_1, bool flag)
        {
            EntityUniqueID id_0, id_1;

            getIDs(body_0, body_1, id_0, id_1);

            IDVec &vec = _ignore_map[id_0];

            auto &it = std::find(vec.begin(), vec.end(), id_1);

            if (it != vec.end() && !flag)
                vec.erase(it);
            else if (it == vec.end() && flag)
                vec.push_back(id_1);
        }

        bool Simulation::GetIgnoreCollision(RigidBody2D *body_0, RigidBody2D *body_1)
        {
            EntityUniqueID id_0, id_1;

            getIDs(body_0, body_1, id_0, id_1);

            auto &find = _ignore_map.find(id_0);

            // if the map contains the first id
            if (find != _ignore_map.end())
            {
                const IDVec &vec = _ignore_map[id_0];

                auto &it = std::find(vec.begin(), vec.end(), id_1);

                if (it != vec.end())
                    return true;

                return false;
            }
            else
                return false;
        }

        void Simulation::getIDs(RigidBody2D *body_0, RigidBody2D *body_1, 
                                EntityUniqueID &id_0, EntityUniqueID &id_1)
        {
            id_0 = body_0->GetOwner()->GetUniqueID();
            id_1 = body_1->GetOwner()->GetUniqueID();

            if (id_0 > id_1)
                utils::Swap(id_0, id_1);
        }

    }
}