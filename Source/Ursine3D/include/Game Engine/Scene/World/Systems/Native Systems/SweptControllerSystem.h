/* ---------------------------------------------------------------------------
** Team Bear King
** 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SweptControllerSystem.h
**
** Author:
** - Jordan Ellis - contact@jordanellis.me
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "FilterSystem.h"

namespace ursine
{
	namespace ecs
	{
		class SweptController;
		class PhysicsSystem;

		class SweptControllerSystem : public FilterSystem
		{
			ENTITY_SYSTEM;

		public:
			Meta(DisableNonDynamic)
			SweptControllerSystem(ursine::ecs::World *world);
			~SweptControllerSystem(void);

		private:
			PhysicsSystem *m_physics;

			void Initialize(void) override;

			void Process(const EntityHandle &entity) override;

			// Each frame update, sweepVelocity starts out as the intended movement of the controller (ControllerVelocity).
			// As contact with other geometry is detected during interation, sweepVelocity is continually modified to
			// represent the possible path of motion that is still within the initial direction of motion.
			// The ControllerVelocity is only modified when velocity in a particular direction is not
			// desired for the following frame updates.
			void sweptCollision(SweptController &controller, SVec3 sweptVelocity, float timeLeft, bool kinematic);

			// Sends out script events and invokes tracker events to be sent.
			// Updating the list of kinematic contacts must be done after evets are sent
			// or the list will not have complete information.
			void sweptCompleted(SweptController &controller);

			//////////////////////////////////////////////////
			// Math Helpers
			//////////////////////////////////////////////////

			// A downward cast for snapping the character to the ground, only done when
			// grounded to stay in contact with the ground, unless moving too fast.
			void snapToGround(SweptController &controller);

			// Measures angle between surface normal and WorldUp
			// to determine if the surface is ground.
			static bool isGroundSurface(SweptController &controller, const SVec3 &normal);

			// Measures angle between surface normal and negative worldUp
			// to determine if the surface is a ceiling.
			static bool isCeilingSurface(SweptController &controller, const SVec3 &normal);
			
			// Projects velocity directionally onto the plane defined by the normal.
			// Used to maintain a velocity's length perpendicular to a given axis (direction).
			// Projection is effectively a ray to plane intersection with a plane through the origin.
			static SVec3 skewProjection(SweptController &controller, const SVec3 &velocity, 
								        const SVec3 &direction, const SVec3 &normal);

			// copies all entires over and clears old list for tracking next update.
			static void updateKinematicList(SweptController &controller);

		} Meta(Enable, WhiteListMethods, AutoAddEntitySystem);
	}
}
