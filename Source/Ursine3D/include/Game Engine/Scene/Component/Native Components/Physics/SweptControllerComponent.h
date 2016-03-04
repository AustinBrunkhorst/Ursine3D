/* ----------------------------------------------------------------------------
** Team Bear King
** 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SweptControllerComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "Component.h"
#include "RigidbodyComponent.h"
#include "CylinderColliderComponent.h"
#include "SVec3.h"

namespace ursine
{
    namespace ecs
    {
        class SweptController : public Component
        {
            NATIVE_COMPONENT;
            
            friend class SweptControllerSystem;

        public:
            // Flag for enabling and disabling the character controller
            EditorField(
                bool active,
                GetActive,
                SetActive
            );

            // Normal of the world's ground plane.
            // Used to decompose movement into horizontal and vertical pieces.
            // Used to determine what surfaces are walkable.
            EditorField(
                SVec3 worldUp,
                GetWorldUp,
                SetWorldUp
            );

            // If the swept event should also be tracked as standard collision events.
            // i.e. Will send out CollisionStarted/Persisted/Ended events.
            EditorField(
                bool forwardEvents,
                GetForwardEvents,
                SetForwardEvents
            );

            // Constant acceleration against WorldUp when not on ground.
            EditorField(
                float gravity,
                GetGravity,
                SetGravity
            );

            // Instantanious velocity in the WorldUp direction when jump is activated.
            EditorField(
                float jumpSpeed,
                GetJumpSpeed,
                SetJumpSpeed
            );

            // The percentage of upward velocity that should be removed
            // when a jump is cancelled.
            EditorMeta(InputRange(0.0f, 1.0f, 0.01f))
            EditorField(
                float jumpCancelFactor,
                GetJumpCancelFactor,
                SetJumpCancelFactor
            );

            // Maximum speed you can accelerate to in the horizontal direction.
            EditorField(
                float maxMoveSpeed,
                GetMaxMoveSpeed,
                SetMaxMoveSpeed
            );

            // Maximum speed you can fall at.
            EditorField(
                float maxFallSpeed,
                GetMaxFallSpeed,
                SetMaxFallSpeed
            );

            // Increase in movement velocity per second on ground or in air.
            EditorField(
                float groundAcceleration,
                GetGroundAcceleration,
                SetGroundAcceleration
            );

            EditorField(
                float airAcceleration,
                GetAirAcceleration,
                SetAirAcceleration
            );

            // Decrease in movement velocity per second on ground or in air.
            // Not applied against direction of input.
            // Prevents side-slipping when changing directions.
            // Adds to acceleration when moving against current velocity.
            EditorField(
                float groundDeceleration,
                GetGroundDeceleration,
                SetGroundDeceleration
            );

            EditorField(
                float airDeceleration,
                GetAirDeceleration,
                SetAirDeceleration
            );

            // Maximum angle, in degrees, that a surface can be to be considered ground
            // based upon the WorldUp vector.
            // Ground can be walked on and jumped off of.
            // A value of 0 means only flat surfaces are walkable.
            // A value near 90 means almost all surface are walkable.
            EditorMeta(InputRange(0.0f, 90.0f, 0.5f, "{{value.toFixed( 2 )}} deg"))
            EditorField(
                float maxGroundSlope,
                GetMaxGroundSlope,
                SetMaxGroundSlope
            );

            // Maximum angle, in degrees, that a surface can be to be considered a ceiling
            // based upon the WorldUp vector.
            // Used to prevent collide and slide along a ceiling surface.
            // Stops upward velocity when jumping into the ceiling.
            EditorMeta(InputRange(0.0f, 90.0f, 0.5f, "{{value.toFixed( 2 )}} deg"))
            EditorField(
                float maxCeilingSlope,
                GetMaxCeilingSlope,
                SetMaxCeilingSlope
            );

            // Maximum distance that the character will be projected down
            // to maintain connection to the ground.
            // Only takes affect when grounded.
            // If moving over sloped surfaces fast enough, the character can sweep
            // far enough to exceed this distance from the ground in a single update,
            // causing the character to become ungrounded.
            EditorField(
                float groundSnapDistance,
                GetGroundSnapDistance,
                SetGroundSnapDistance
            );

            SweptController(void);
            ~SweptController(void);

            Meta(Disable)
            void OnInitialize(void) override;

            /////////////////////////////////////////////////////////////////
            // Property Getters and Setters
            /////////////////////////////////////////////////////////////////

            bool GetActive(void) const;
            void SetActive(bool active);

            const SVec3 &GetWorldUp(void) const;
            void SetWorldUp(const SVec3 &up);

            bool GetForwardEvents(void) const;
            void SetForwardEvents(bool forward);

            float GetGravity(void) const;
            void SetGravity(float gravity);

            float GetJumpSpeed(void) const;
            void SetJumpSpeed(float jumpSpeed);

            float GetJumpCancelFactor(void) const;
            void SetJumpCancelFactor(float jumpCancelFactor);

            float GetMaxMoveSpeed(void) const;
            void SetMaxMoveSpeed(float maxMoveSpeed);

            float GetMaxFallSpeed(void) const;
            void SetMaxFallSpeed(float maxFallSpeed);

            float GetGroundAcceleration(void) const;
            void SetGroundAcceleration(float groundAcceleration);

            float GetAirAcceleration(void) const;
            void SetAirAcceleration(float airAcceleration);

            float GetGroundDeceleration(void) const;
            void SetGroundDeceleration(float groundDeceleration);

            float GetAirDeceleration(void) const;
            void SetAirDeceleration(float airDeceleration);

            float GetMaxGroundSlope(void) const;
            void SetMaxGroundSlope(float groundSlope);

            float GetMaxCeilingSlope(void) const;
            void SetMaxCeilingSlope(float ceilingSlope);

            float GetGroundSnapDistance(void) const;
            void SetGroundSnapDistance(float snapDist);

            bool GetGrounded(void) const;

            bool GetJumping(void) const;

            // Tell the character controller what movement direction
            // it should move in for this next frame.
            // This is the only function needed to move the controller,
            // and needs to be set per frame.
            void SetMovementDirection(const SVec3 &movementDirection);
            const SVec3 &GetMovementDirection(void) const;

            /////////////////////////////////////////////////////////////////
            // User Manipulation Functions For The Controller
            /////////////////////////////////////////////////////////////////

            // Jump actions should be called before calling Update for that frame.
            // Will only cause the character to jump when grounded.
            // User does not need to check for grounded or anything else before calling.
            void Jump(void);

            // Will cause the character to jump unconditionally.
            // Ideal for jumping while in the air.
            // It is up to the user to determine when this should be called.
            void JumpUnconditionally(void);

            // Will cause the character to jump unconditionally.
            // Overwrites any previous velocity to the jump velocity plus
            // the additional velocity passed in.
            // Ideal for jumping off a wall with some horizontal velocity.
            // It is up to the user to determine when this should be called.
            void JumpDirectionally(const SVec3 &additionalVelocity);

            // Must be called to enable variable jump heights,
            // does not have to be called otherwise.
            // Can be called whenever a jump button is not being pressed
            // or when the jump action should be cancelled for any reason.
            void JumpCancel(void);

            // Scalars for changing the amount of acceleration/deceleration
            // at run time without losing the base values set in the properties.
            // These are reset to 1.0f every frame after computing acceleration/deceleration.
            // Expected modification should occur every frame.
            float GetGroundTraction(void) const;
            void SetGroundTraction(float groundTraction);

            float GetAirTraction(void) const;
            void SetAirTraction(float airTraction);

            void AddImpulse(const SVec3 &impulse);

        private:
            bool m_active;
            
            bool m_forwardEvents;

            // Controller state for being on a ground surface.
            // Determines movement acceleration types.
            // Whether gravity is applied, and if jumping is possible.
            bool m_grounded;

            // Controller state for knowing if a jumping action is active.
            // Used to control variable jump heights.
            bool m_jumping;

            // The velocity that reflects the intended movement.
            // Does not reflect the rigidbody's actual velocity.
            SVec3 m_controllerVelocity;
            
            // The user defined (per frame) desired movement direction.
            SVec3 m_movementDirection;

            SVec3 m_worldUp;
            float m_gravity;
            
            float m_jumpSpeed;
            float m_jumpCancelFactor;

            float m_maxMoveSpeed;
            float m_maxFallSpeed;

            float m_groundAcceleration;
            float m_airAcceleration;

            float m_groundDeceleration;
            float m_airDeceleration;
            
            float m_maxGroundSlope;
            float m_maxCeilingSlope;

            float m_groundSnapDistance;

            float m_groundTraction;
            float m_airTraction;

            float m_epsilon;

            // Component Dependencies
            Handle<Transform> m_transform;
            Handle<Rigidbody> m_rigidbody;
            Handle<CylinderCollider> m_collider;

            // TODO: Cast Filter
            /*
                struct CastFilter
                {
                    Clear();
                    
                    IgnoreChildren : bool;
                    IgnoreCog : Cog;

                    IgnoreDynamic : bool;
                    IgnoreGhost : bool;
                    IgnoreKinematic : bool;
                    IgnoreStatic : bool;

                    bool IsSet(flag);
                    void Set(flag);
                };
            */

            // List of kinematic objects detected during the controller's update.
            // Copied to KinematicContacts at the end of the update.
            std::vector<EntityHandle> m_kinematicPending;

            // List of kinematic objects that the character came into contact with.
            // Used to run sweep with the velocities of kinematic objects so that
            // the character can be moved by them.
            std::vector<EntityHandle> m_kinematicContacts;

            // TODO: This component dispatches:
            
            // Sent for every object that the main sweep resolves against
            // (1) SweptCollision

            // Sent if the character is in contact with the ground
            // (2) GroundSnapCollision

            // Sent when the update call is completed.
            // (3) SweptCompleted

            // Needed in to catch kinematic contacts detected by physics engine.
            // i.e. A platform moving into the character rather than the
            // character moving into a platform.
            void onCollision(EVENT_HANDLER(Entity));

            // Adds kinematic objects to the list that will be resolved next update.
            // Does not add duplicate entries.
            void addIfKinematic(const EntityHandle &entity);


            bool skipResolution(/*TODO: CollisionFilter a, CollisionFilter b*/);

        } Meta(Enable, DisplayName( "SweptController" )) 
          EditorMeta(RequiresComponents( typeof( ursine::ecs::Rigidbody ), typeof( ursine::ecs::CylinderCollider ) ));
    }
}
