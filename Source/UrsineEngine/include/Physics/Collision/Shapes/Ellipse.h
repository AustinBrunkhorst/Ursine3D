/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Ellipse.h
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "Shape.h"

namespace Ursine
{
    // Forward declarations
    namespace ECS
    {
        class PhysicsDebugDrawer;
    }

    class Ellipse : public Shape
    {
        friend class ECS::PhysicsDebugDrawer;

    public:
        Ellipse(void);
        Ellipse(const Ellipse &other);
        ~Ellipse(void) override;

        Ellipse *Clone(void) const override;

        void Set(float rotation, 
                    const Vec2 &dimensions,
                    const Vec2 &position);

        AABB TransformShape(ECS::Transform &transform) override;

        Vec2 GetSupport(const Vec2 &direction) const override;

        void ComputeMass(ECS::Transform &transform, Vec2 &center,
                         float &area, float &inertia);

        bool RayCast(RayCastOutput &output, const RayCastInput &input);

    private:
        // rotation, width and hight, and position of the ellipse
        float _rotation;
        Vec2 _dimensions;
        Vec2 _position;

        // stored cosine and sine of the rotation
        float _c, _s;
    };
}
