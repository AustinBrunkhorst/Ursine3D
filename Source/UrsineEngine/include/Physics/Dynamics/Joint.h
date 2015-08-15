/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Joint.h
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

namespace Ursine
{
    // Forward declarations
    namespace ECS
    {
        class Simulation;
        class RigidBody2D;
    }

    class Joint
    {
        friend class ECS::Simulation;

    public:
        virtual ~Joint(void) { }

        virtual void PreStep(void) { }

        virtual void Solve(float dt) = 0;

        virtual void PostStep(void) { }

    protected:
        // Two bodies being connected
        ECS::RigidBody2D *_body[2];
    };
}