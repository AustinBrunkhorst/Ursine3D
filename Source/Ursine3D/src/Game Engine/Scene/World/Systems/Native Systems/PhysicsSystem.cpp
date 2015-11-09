#include "UrsinePrecompiled.h"

#include "PhysicsSystem.h"
#include "RigidbodyComponent.h"
#include "BodyComponent.h"
#include "SphereColliderComponent.h"
#include "BoxColliderComponent.h"
#include "CylinderColliderComponent.h"
#include "CapsuleColliderComponent.h"
#include "ConeColliderComponent.h"
#include "EmptyColliderComponent.h"
#include "GfxAPI.h"

namespace ursine
{
    namespace ecs
    {
        ENTITY_SYSTEM_DEFINITION( PhysicsSystem );

        PhysicsSystem::PhysicsSystem(World *world)
            : FilterSystem( world, Filter( ).One<Body, Rigidbody>( ) )
			, m_debugDrawer( GetCoreSystem( graphics::GfxAPI ) )
        {
            m_collisionShapes.One<
                SphereCollider, 
                BoxCollider, 
                CylinderCollider,
                CapsuleCollider,
                ConeCollider
            >( );

			m_simulation.SetDebugDrawer( &m_debugDrawer );
			m_debugDrawer.setDebugMode(
				physics::DRAW_WIRE_FRAME |
				physics::DRAW_AABB |
				physics::DRAW_CONTACT_POINTS
			);
        }

        bool PhysicsSystem::Raycast(const physics::RaycastInput& input, 
                                    physics::RaycastOutput& output,
                                    physics::RaycastType type, bool debug, float drawDuration)
        {
            bool result = m_simulation.Raycast( input, output, type );

            // debug draw the raycast results
            if (result && debug)
            {
                auto &start = input.start;

                for (size_t i = 0, n = output.hit.size( ); i < n; ++i)
                {
                    auto &hit = output.hit[ i ];
                    auto &norm = output.normal[ i ];

                    // Draw the ray to the hit
                    m_debugSystem->DrawLine( start, hit, Color::Blue, drawDuration );

                    // Draw the normal
                    m_debugSystem->DrawLine( hit, hit + norm * 1.0f, Color::White, drawDuration );

                    // Draw the hit location
                    m_debugSystem->DrawPoint( hit, 10.0f, Color::Cyan, drawDuration );
                }
            }

            return result;
        }

        void PhysicsSystem::Process(Entity* entity)
        {
            auto trans = entity->GetTransform( );

            if (entity->HasComponent<Rigidbody>( ))
            {
                auto body = entity->GetComponent<Rigidbody>( );

                body->m_rigidbody.GetTransform( trans );
            }
            else
            {
                auto body = entity->GetComponent<Body>( );

                body->m_body.GetTransform( trans );
            }
        }

        void PhysicsSystem::OnInitialize(void)
        {
            FilterSystem::OnInitialize( );

            m_debugSystem = m_world->GetEntitySystem( DebugSystem );

            m_world->Listener( this )
                .On( WORLD_UPDATE, &PhysicsSystem::onUpdate )
                .On( WORLD_ENTITY_COMPONENT_ADDED, &PhysicsSystem::onComponentAdded )
                .On( WORLD_ENTITY_COMPONENT_REMOVED, &PhysicsSystem::onComponentRemoved );
        }

        void PhysicsSystem::OnRemove(void)
        {
            FilterSystem::OnRemove( );

            m_world->Listener( this )
                .Off( WORLD_UPDATE, &PhysicsSystem::onUpdate )
                .Off( WORLD_ENTITY_COMPONENT_ADDED, &PhysicsSystem::onComponentAdded )
                .Off( WORLD_ENTITY_COMPONENT_REMOVED, &PhysicsSystem::onComponentRemoved );
        }

        void PhysicsSystem::onComponentAdded(EVENT_HANDLER(World))
        {
            EVENT_ATTRS(World, ComponentEventArgs);
         
            auto &entity = args->entity;
            auto &component = args->component;

            if (component->Is<Rigidbody>( ))
            {
                auto body = entity->GetComponent<Rigidbody>( );

                if (entity->HasComponent<Body>( ))
                {
                    // transfer the collider
                    body->m_rigidbody.SetCollider(
                        entity->GetComponent<Body>( )->m_body.getCollisionShape( ) 
                    );

                    entity->RemoveComponent<Body>( );
                }

                // Add the body to the simulation
                m_simulation.AddRigidbody(
                    &body->m_rigidbody
                );
            }
            else if (component->Is<SphereCollider>( ))
            {
                auto *sphere = entity->GetComponent<SphereCollider>( );

                addCollider( entity, &sphere->m_sphereCollider );
            }
            else if (component->Is<BoxCollider>( ))
            {
                auto *box = entity->GetComponent<BoxCollider>( );

                addCollider( entity, &box->m_boxCollider );
            }
            else if (component->Is<CylinderCollider>( ))
            {
                auto *cylinder = entity->GetComponent<CylinderCollider>( );

                addCollider( entity, &cylinder->m_cylinderCollider );
            }
            else if (component->Is<CapsuleCollider>( ))
            {
                auto *capsule = entity->GetComponent<CapsuleCollider>( );

                addCollider( entity, &capsule->m_capsuleCollider );
            }
            else if (component->Is<ConeCollider>( ))
            {
                auto *cone = entity->GetComponent<ConeCollider>( );

                addCollider( entity, &cone->m_coneCollider );
            }
            else if (component->Is<EmptyCollider>( ))
            {
                auto *empty = entity->GetComponent<EmptyCollider>( );

                addCollider( entity, &empty->m_emptyCollider, true );
            }
        }

        void PhysicsSystem::onComponentRemoved(EVENT_HANDLER(World))
        {
            EVENT_ATTRS(World, ComponentRemovedEventArgs);

            auto &entity = args->entity;
            auto &component = args->component;
            auto &oldTypeMask = args->oldTypeMask;

            if (component->Is<Rigidbody>( ))
            {
                auto rigidbody = static_cast<Rigidbody*>( component );

                // if there are still collision shapes attached to 
                // the entity, add a body for them to use
                if (m_collisionShapes.Matches( oldTypeMask ))
                {
                    // Copy the collider over to the new body
                    auto *body = entity->AddComponent<Body>( );
                    body->m_body.SetCollider(
                        rigidbody->m_rigidbody.GetCollider( )
                    );

                    m_simulation.AddBody( &body->m_body );
                }

                // Add the body to the simulation
                m_simulation.RemoveRigidbody( 
                    &rigidbody->m_rigidbody
                );

                // if there is an empty collider attached, remove it
                if (entity->HasComponent<EmptyCollider>( ))
                    entity->RemoveComponent<EmptyCollider>( );
            }
            else if (component->Is<Body>( ))
            {
                auto body = static_cast<Body*>( component );
                
                m_simulation.RemoveBody(
                    &body->m_body
                );
            }
            else if (m_collisionShapes.Matches( component->GetTypeMask( ) ))
            {
                removeCollider( entity );
            }
        }

        void PhysicsSystem::onUpdate(EVENT_HANDLER(World))
        {
            m_simulation.Step(Application::Instance->GetDeltaTime( ));
        }

        void PhysicsSystem::addCollider(Entity *entity, physics::ColliderBase *collider, bool emptyCollider)
        {
            bool addBody = false;

            if (!entity->HasComponent<Body>( ) && !entity->HasComponent<Rigidbody>( ))
            {
                entity->AddComponent<Body>( );
                addBody = true;
            }

            // Add the collider to the body
            if (entity->HasComponent<Body>( ))
            {
                auto body = entity->GetComponent<Body>( );

                body->m_body.SetCollider( collider );

                if (addBody)
                    m_simulation.AddBody( &body->m_body );
            }
            else if (entity->HasComponent<Rigidbody>( ))
                entity->GetComponent<Rigidbody>( )->m_rigidbody.SetCollider( collider, emptyCollider );
        }

        void PhysicsSystem::removeCollider(Entity *entity)
        {
            if (entity->HasComponent<Body>( ))
                entity->RemoveComponent<Body>( );

            // Add an empty collider
            if (entity->HasComponent<Rigidbody>( ))
                entity->AddComponent<EmptyCollider>( );
        }

    }
}
