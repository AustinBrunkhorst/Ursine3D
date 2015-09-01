/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** DistanceJoint.h
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "Joint.h"

namespace Spring
{
    const float STEEL = 1.0f;
    const float ROPE = 0.025f;
    const float SPRING = 0.001f;
    const float RUBBER_BAND = 0.0001f;
    const float BUNGIE_CABLE = 0.00005f;
}

namespace ursine
{
    // Forward declarations
    namespace ecs
    {
        class PhysicsDebugDrawer;
        class RigidBody2D;
    }

    class DistanceJoint : public Joint
    {
        friend class ecs::PhysicsDebugDrawer;

    public:            
        DistanceJoint(ecs::RigidBody2D *body_0, ecs::RigidBody2D *body_1, 
                      float distance, bool max_distance_only = false);

        ~DistanceJoint(void) override;

        // Called before the solve phase
        void PreStep(void) override;

        // Iteratively solve this constraint
        void Solve(float dt) override;

        // Set post solution values
        void PostStep(void) override;

        // Set the distance we're solving for
        DistanceJoint *SetDistance(float distance);

        // Set the anchor that body will rotate around in local coordinates
        DistanceJoint *SetAnchor(uint body_index, const Vec2 &anchor);
            
        DistanceJoint *SetMaxDistanceOnly(bool flag);
            
        DistanceJoint *SetSpring(const float constant);
            
        DistanceJoint *SetRotationLock(uint body_index, float world_rotation);

    private:
        // The bodies' transform components
        ecs::Transform *_trans[2];

        // The distance we solve for
        float _distance;

        // The anchor that the bodies will rotate around
        Vec2 _anchor[2];

        // The transformed anchors in world space (updated each frame)
        // Used for limiting matrix multiplications
        Vec2 _trans_anchor[2];

        // flag saying whether or not the rotation is locked for the two bodies
        bool _rot_locked[2];
        float _trans_rot_anchor[2];
        float _rot_anchor[2];

        // true = allow objects to be inside distance circle but not outside.
        // false = objects are always "_distance" apart.
        bool _max_distance_only;

        float _spring_constant;
    };
}
