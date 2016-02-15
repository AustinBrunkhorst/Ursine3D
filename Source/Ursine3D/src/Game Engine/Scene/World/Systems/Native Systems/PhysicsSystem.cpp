/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** PhysicsSystem.cpp
**
** Author:
** - Joshua Shlemmer - joshua.shlemmer@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "PhysicsSystem.h"
#include "RigidbodyComponent.h"
#include "BodyComponent.h"
#include "SphereColliderComponent.h"
#include "BoxColliderComponent.h"
#include "CylinderColliderComponent.h"
#include "CapsuleColliderComponent.h"
#include "ConeColliderComponent.h"
#include "ConvexHullColliderComponent.h"
#include "ConvexDecompColliderComponent.h"
#include "BvhTriangleMeshColliderComponent.h"
#include "EmptyColliderComponent.h"
#include "GfxAPI.h"
#include "PhysicsSettingsComponent.h"

namespace ursine
{
    namespace ecs
    {
        ENTITY_SYSTEM_DEFINITION( PhysicsSystem );

        PhysicsSystem::PhysicsSystem(World *world)
            : EntitySystem( world )
			, m_debugDrawer( GetCoreSystem( graphics::GfxAPI ) )
			, m_enableDebugDraw( true )
			, m_playmodeDebugDraw( false )
        {
            m_collisionShapes.One<
                SphereCollider, 
                BoxCollider, 
                CylinderCollider,
                CapsuleCollider,
                ConeCollider,
				ConvexHullCollider,
				ConvexDecompCollider,
				BvhTriangleMeshCollider
            >( );

			m_simulation.SetDebugDrawer( &m_debugDrawer );
        }

        void PhysicsSystem::SetGravity(const SVec3& gravity)
        {
            // We have to wake up all rigidbodies before we set the gravity
            auto entities = m_world->GetEntitiesFromFilter(Filter( ).All<Rigidbody>( ));

            for (auto &entity : entities)
                entity->GetComponent<Rigidbody>( )->SetAwake( );

            m_simulation.SetGravity( gravity );
        }

        SVec3 PhysicsSystem::GetGravity(void) const
        {
            return m_simulation.GetGravity( );
        }

        void PhysicsSystem::SetEnableDebugDraw(bool enable)
        {
            m_enableDebugDraw = enable;

            if (m_enableDebugDraw)
            {
                m_debugDrawer.setDebugMode(
			        physics::DRAW_WIRE_FRAME |
			        physics::DRAW_CONTACT_POINTS
		        );
            }
            else
            {
                m_debugDrawer.setDebugMode(
			        physics::DRAW_NONE
		        );
            }
        }

	    bool PhysicsSystem::GetEnableDebugDraw(void) const
        {
            return m_enableDebugDraw;
        }

		void PhysicsSystem::SetPlaymodeDebugDraw(bool enable)
		{
			m_playmodeDebugDraw = enable;
		}

		bool PhysicsSystem::GetPlaymodeDebugDraw(void) const
		{
			return m_playmodeDebugDraw;
		}

        void PhysicsSystem::ClearContacts(Rigidbody* rigidbody)
        {
            m_simulation.ClearContacts( rigidbody->m_rigidbody );
        }

        bool PhysicsSystem::Raycast(const physics::RaycastInput& input, physics::RaycastOutput& output,
                                    physics::RaycastType type, bool debug, float drawDuration, 
                                    Color color, bool alwaysDrawLine)
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
                    m_debugSystem->DrawLine( start, hit, color, drawDuration );

                    // Draw the normal
                    m_debugSystem->DrawLine( hit, hit + norm * 1.0f, Color::White, drawDuration );

                    // Draw the hit location
                    m_debugSystem->DrawPoint( hit, 10.0f, Color::Cyan, drawDuration );
                }
            }
            else if(debug && alwaysDrawLine)
            {
                m_debugSystem->DrawLine( input.start, input.end, color, drawDuration );
            }

            return result;
        }

		bool PhysicsSystem::Sweep(Rigidbody *body, const SVec3 &velocity, float dt, 
					              physics::SweepOutput &output, physics::SweepType type, bool sorted)
        {
			if (velocity == SVec3::Zero( ) || dt == 0.0f)
				return false;

			auto *bodyBase = &body->m_rigidbody;
	        auto *collider = bodyBase->GetCollider( );

			auto result = m_simulation.Sweep( collider, bodyBase, velocity, dt, output, type, sorted );

			if (m_enableDebugDraw)
			{
				for (size_t i = 0, n = output.hit.size( ); i < n; ++i)
				{
					auto &hit = output.hit[ i ];

					m_debugSystem->DrawPoint( hit, 10.0f, Color::Pink, 0.2f );
				}
			}

			return result;
        }

        void PhysicsSystem::OnInitialize(void)
        {
            m_debugSystem = m_world->GetEntitySystem( DebugSystem );

            m_world->Listener( this )
                .On( WORLD_UPDATE, &PhysicsSystem::onUpdate, 100000 )
                .On( WORLD_ENTITY_COMPONENT_ADDED, &PhysicsSystem::onComponentAdded )
                .On( WORLD_ENTITY_COMPONENT_REMOVED, &PhysicsSystem::onComponentRemoved );

        #if defined(URSINE_WITH_EDITOR)

            m_world->Connect( WORLD_EDITOR_RENDER, this, &PhysicsSystem::onEditorRender, -10000 );

        #endif
        }

        void PhysicsSystem::OnRemove(void)
        {
            m_world->Listener( this )
                .Off( WORLD_UPDATE, &PhysicsSystem::onUpdate )
                .Off( WORLD_ENTITY_COMPONENT_ADDED, &PhysicsSystem::onComponentAdded )
                .Off( WORLD_ENTITY_COMPONENT_REMOVED, &PhysicsSystem::onComponentRemoved );

        #if defined(URSINE_WITH_EDITOR)

            m_world->Disconnect( WORLD_EDITOR_RENDER, this, &PhysicsSystem::onEditorRender );

        #endif
        }

        void PhysicsSystem::OnAfterLoad(void)
        {
            auto levelSettings = m_world->GetSettings( );

            if (!levelSettings->HasComponent<PhysicsSettings>( ))
                levelSettings->AddComponent<PhysicsSettings>( );

			auto physicsSettings = levelSettings->GetComponent<PhysicsSettings>( );

		#ifdef URSINE_WITH_EDITOR

			physicsSettings->SetEnableDebugDraw( true );
			SetEnableDebugDraw( true );

		#else

			physicsSettings->SetEnableDebugDraw( false );
			SetEnableDebugDraw( false );

		#endif
        }

        void PhysicsSystem::onComponentAdded(EVENT_HANDLER(World))
        {
            EVENT_ATTRS(World, ComponentEventArgs);

            auto &entity = args->entity;
            auto &component = args->component;

			// If the user added a collider, remove the others that may exist
			if (m_collisionShapes.Matches( entity ) && 
				m_collisionShapes.Matches( component->GetTypeMask( ) ))
				removeExistingCollider( entity, component->GetTypeID( ) );

            if (component->Is<Rigidbody>( ))
            {
                auto rigidbody = entity->GetComponent<Rigidbody>( );

                if (entity->HasComponent<Body>( ))
                {
                    auto body = entity->GetComponent<Body>( );

                    // transfer the collider
                    rigidbody->m_rigidbody.SetCollider(
                        body->m_body.getCollisionShape( )
                    );

                    // transform the offset
                    rigidbody->m_rigidbody.SetOffset(
                        body->m_body.GetOffset( )
                    );

                    entity->RemoveComponent<Body>( );
                }

                // If the entity does not have a collision shape, add an empty one
                if (!m_collisionShapes.Matches( entity ))
                    entity->AddComponent<EmptyCollider>( );

                // set the transform
                rigidbody->m_rigidbody.SetTransform(
                    entity->GetTransform( )
                );

                // Set the bodies gravity
                rigidbody->m_rigidbody.SetGravity( GetGravity( ) );

                // Add the body to the simulation
                m_simulation.AddRigidbody(
                    &rigidbody->m_rigidbody
                );

                m_rigidbodies.push_back( rigidbody );
            }
            else if (component->Is<Body>( ))
            {
                m_bodies.push_back( reinterpret_cast<Body*>( const_cast<Component*>( args->component ) ) );
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
			else if (component->Is<ConvexHullCollider>( ))
			{
				auto *hull = entity->GetComponent<ConvexHullCollider>( );

				addCollider( entity, &hull->m_convexHullCollider );
			}
			else if (component->Is<ConvexDecompCollider>( ))
			{
				auto *convex = entity->GetComponent<ConvexDecompCollider>( );

				addCollider( entity, &convex->m_convexDecompCollider );
			}
			else if (component->Is<BvhTriangleMeshCollider>( ))
			{
				auto *bvhMesh = entity->GetComponent<BvhTriangleMeshCollider>( );

				addCollider( entity, &bvhMesh->m_bvhTriangleMeshCollider );
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
                if (m_collisionShapes.Matches( oldTypeMask ) && !entity->IsDeleting( ))
                {
                    auto *body = entity->AddComponent<Body>( );

                    // Copy the collider over to the new body
                    body->m_body.SetCollider(
                        rigidbody->m_rigidbody.GetCollider( )
                    );

                    // transform the offset
                    body->m_body.SetOffset(
                        rigidbody->m_rigidbody.GetOffset( )
                    );

                    // set the transform
                    body->m_body.SetTransform(
                        entity->GetTransform( )
                    );

                    m_simulation.AddBody( &body->m_body );
                }

                // Add the body to the simulation
                m_simulation.RemoveRigidbody( 
                    &rigidbody->m_rigidbody
                );

                // if there is an empty collider attached, remove it
                if (entity->HasComponent<EmptyCollider>( ) && !entity->IsDeleting( ))
                    entity->RemoveComponent<EmptyCollider>( );

                // Remove it from our list
                m_rigidbodies.erase( std::find(
                    m_rigidbodies.begin( ),
                    m_rigidbodies.end( ),
                    args->component
                ) );
            }
            else if (component->Is<Body>( ))
            {
                auto body = static_cast<Body*>( component );
                
                m_simulation.RemoveBody(
                    &body->m_body
                );

                // Remove it from our list
                m_bodies.erase( std::find(
                    m_bodies.begin( ),
                    m_bodies.end( ),
                    args->component
                ) );
            }
            else if (m_collisionShapes.Matches( component->GetTypeMask( ) ))
            {
				if (!entity->IsDeleting( ))
					removeCollider( entity );
            }
        }

        void PhysicsSystem::onUpdate(EVENT_HANDLER(World))
        {
            m_simulation.Step( Application::Instance->GetDeltaTime( ) );

            // update all rigidbodies
            for (auto &rigid : m_rigidbodies)
            {
                rigid->m_rigidbody.GetTransform( rigid->GetOwner( )->GetTransform( ) );
            }

            // update all bodies
            for (auto &body : m_bodies)
            {
                body->m_body.GetTransform( body->GetOwner( )->GetTransform( ) );
            }

			// dispatch all collision events for this frame
			m_simulation.DispatchCollisionEvents( );

		#if defined(URSINE_WITH_EDITOR)

			if (m_playmodeDebugDraw)
				m_simulation.DebugDrawSimulation( );

		#endif
        }

    #if defined(URSINE_WITH_EDITOR)

        void PhysicsSystem::onEditorRender(EVENT_HANDLER(World))
        {
            m_simulation.DebugDrawSimulation( );
        }

    #endif

        void PhysicsSystem::addCollider(Entity *entity, physics::ColliderBase *collider, bool emptyCollider)
        {
            bool removeBody = true;

            if (!entity->HasComponent<Body>( ) && !entity->HasComponent<Rigidbody>( ))
            {
                entity->AddComponent<Body>( );
                removeBody = false;
            }

            // Add the collider to the body
            if (entity->HasComponent<Body>( ))
            {
                auto body = entity->GetComponent<Body>( );

                if (removeBody)
                    m_simulation.RemoveBody( &body->m_body );

                body->m_body.SetCollider( collider );

                body->m_body.SetTransform( entity->GetTransform( ) );

                m_simulation.AddBody( &body->m_body );
            }
            else if (entity->HasComponent<Rigidbody>( ))
            {
                auto rigidbody = entity->GetComponent<Rigidbody>();

                if (!emptyCollider)
                    m_simulation.RemoveRigidbody( &rigidbody->m_rigidbody );

                // Assign the collider
                rigidbody->m_rigidbody.SetCollider( collider, emptyCollider );

                if (!emptyCollider)
                    m_simulation.AddRigidbody( &rigidbody->m_rigidbody );
            }

            // Remove the empty collider if it exists
            if (!emptyCollider && entity->HasComponent<EmptyCollider>( ))
                entity->RemoveComponent<EmptyCollider>( );
        }

        void PhysicsSystem::removeCollider(Entity *entity)
        {
            if (entity->HasComponent<Body>( ))
                entity->RemoveComponent<Body>( );

            // Add an empty collider
            if (entity->HasComponent<Rigidbody>( ))
            {
                auto rigidbody = entity->GetComponent<Rigidbody>( );

                ClearContacts( rigidbody );

                entity->AddComponent<EmptyCollider>( );
            }
        }

		void PhysicsSystem::removeExistingCollider(Entity *entity, ComponentTypeID newCollider)
        {
			for (auto *comp : entity->GetComponents( ))
			{
				auto compID = comp->GetTypeID( );

				if (compID != newCollider && m_collisionShapes.Matches( comp->GetTypeMask( ) ))
					entity->RemoveComponent( compID );
			}
        }
    }
}
