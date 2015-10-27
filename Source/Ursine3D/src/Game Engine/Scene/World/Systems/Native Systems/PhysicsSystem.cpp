#include "UrsinePrecompiled.h"

#include "PhysicsSystem.h"
#include "RigidbodyComponent.h"
#include "BodyComponent.h"
#include "SphereColliderComponent.h"

namespace ursine
{
    namespace ecs
    {
        ENTITY_SYSTEM_DEFINITION( PhysicsSystem );

        PhysicsSystem::PhysicsSystem(World *world)
            : FilterSystem( world, Filter( ).All<Body, Rigidbody>( ) )
        {
            m_collisionShapes.One<SphereCollider>();
        }

        PhysicsSystem::~PhysicsSystem(void)
        {
            
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
            m_world->Listener( this )
                .On( WORLD_UPDATE, &PhysicsSystem::onUpdate )
                .On( WORLD_ENTITY_COMPONENT_ADDED, &PhysicsSystem::onComponentAdded )
                .On( WORLD_ENTITY_COMPONENT_REMOVED, &PhysicsSystem::onComponentRemoved );
        }

        void PhysicsSystem::OnRemove(void)
        {
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
                    entity->RemoveComponent<Body>( );

                // Add the body to the simulation
                m_simulation.AddRigidbody(
                    &body->m_rigidbody
                );
            }
            else if (component->Is<Body>( ))
            {
                auto body = entity->GetComponent<Body>( );

                m_simulation.RemoveBody(
                    &body->m_body
                );
            }
            else if (component->Is<SphereCollider>( ))
            {
                auto *sphere = entity->GetComponent<SphereCollider>( );
                
                if (!entity->HasComponent<Body>( ) && !entity->HasComponent<Rigidbody>( ))
                    entity->AddComponent<Body>( );

                // Add the collider to the body
                if (entity->HasComponent<Body>( ))
                    entity->GetComponent<Body>( )->m_body.SetCollider( &sphere->m_sphereCollider );
                else if (entity->HasComponent<Rigidbody>( ))
                    entity->GetComponent<Rigidbody>( )->m_rigidbody.SetCollider( &sphere->m_sphereCollider );
            }
        }

        void PhysicsSystem::onComponentRemoved(EVENT_HANDLER(World))
        {
            EVENT_ATTRS(World, ComponentEventArgs);

            auto &entity = args->entity;
            auto &component = args->component;

            if (component->Is<Rigidbody>( ))
            {
                auto body = entity->GetComponent<Rigidbody>( );

                // Add the body to the simulation
                m_simulation.RemoveRigidbody( 
                    &body->m_rigidbody
                );

                // if there are still collision shapes attached to 
                // the entity, add a body for them to use
                if (m_collisionShapes.Matches( entity ))
                    entity->AddComponent<Body>( );
            }
            else if (component->Is<Body>( ))
            {
                auto body = entity->GetComponent<Body>( );
                
                m_simulation.RemoveBody(
                    &body->m_body
                );
            }
            else if (component->Is<SphereCollider>( ))
            {
                auto sphere = entity->GetComponent<SphereCollider>( );

                if (entity->HasComponent<Body>( ))
                    entity->RemoveComponent<Body>( );

                // Remove the collider to the body
                if (entity->HasComponent<Rigidbody>( ))
                    entity->GetComponent<Rigidbody>( )->m_rigidbody.RemoveCollider( );
            }
        }

        void PhysicsSystem::onUpdate(EVENT_HANDLER(World))
        {
            m_simulation.Step(Application::Instance->GetDeltaTime( ));
        }
    }
}
