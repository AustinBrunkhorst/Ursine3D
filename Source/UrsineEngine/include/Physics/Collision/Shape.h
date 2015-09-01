/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Shape.h
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "AABB.h"
#include "RayCast.h"

namespace ursine
{
    enum ShapeType
    {
        POLYGON,
        ELLIPSE
    };

    class Shape
    {
    public:
        virtual ~Shape(void) {};

        virtual Shape *Clone(void) const = 0;

        // Set the shape's properties 
        // (mesh name for polygons, radius for circles)
        template<typename T>
        void Set(T) = 0;

        // return the encumpasing AABB of the shape
        virtual AABB TransformShape(ecs::Transform &transform) = 0;

        // return a support point in the specified direction
        virtual Vec2 GetSupport(const Vec2 &direction) const = 0;

        // compute the mass of this shape
        virtual void ComputeMass(ecs::Transform &transform, Vec2 &center,
                                 float &area, float &inertia) = 0;

        virtual bool RayCast(RayCastOutput &output, const RayCastInput &input) = 0;

        ShapeType type;
    };
}
