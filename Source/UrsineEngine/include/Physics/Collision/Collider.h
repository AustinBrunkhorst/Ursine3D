/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Collider.h
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include <vector>
#include "Vector2.h"
#include "TransformComponent.h"

namespace Ursine
{
    // Forward declarations
    class Collider;
    class Shape;

    typedef std::vector<Collider*> Colliders;

    class Collider
    {
    public:
        Collider();
        Collider(const Collider &other);
        ~Collider();
        Collider &operator=(const Collider &rhs);

        // Find the index of the incident
        // edge based on the nomal
        uint FindIndex(const Vector2 &normal) const;

        bool is_colliding;

        // The shape of the collider
        Shape *shape;

        ECS::Transform transform;

        // The bounding AABB around this collider
        AABB aabb;

        Colliders children;
    };

    extern template
    Json JsonSerializer::Serialize(Collider * &instance);

    extern template
    Json JsonSerializer::Serialize(Colliders &colliders);

    extern template
    void JsonSerializer::Deserialize(const Json &data, Collider * &out);

    extern template
    void JsonSerializer::Deserialize(const Json &data, Colliders &colliders);
}