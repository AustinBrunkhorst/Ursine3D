/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Collider.cpp
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

namespace ursine
{
    Collider::Collider()
        : shape(nullptr)
        , is_colliding(false)
    {

    }

    Collider::Collider(const Collider &other)
        : is_colliding(other.is_colliding)
        , transform(other.transform)
    {
        if (other.shape != nullptr)
            shape = other.shape->Clone();
        else
            shape = nullptr;
    }

    Collider &Collider::operator=(const Collider &rhs)
    {
        shape = rhs.shape->Clone();
        is_colliding = rhs.is_colliding;

        return (*this);
    }

    Collider::~Collider()
    {
        if (shape != nullptr)
            delete shape;
    }

    uint Collider::FindIndex(const Vec2 &normal) const
    {
        if (shape->type == POLYGON)
        {
            Polygon *poly = static_cast<Polygon*>(shape);

            uint i = 0;
            for (auto &edge : poly->edges)
            {
                if (Vec2::SameDirection(normal, edge.norm))
                    return i;
                
                ++i;
            }
        }

        return 0;
    }

    template<>
    Json JsonSerializer::Serialize(Collider *&collider)
    {
        Json::object object;

        // Serialize shape
        if (collider->shape->type == POLYGON)
            object["mesh"] = Json(static_cast<Polygon*>(collider->shape)->GetName());

        // position, scale, and rotation
        auto &transform = collider->transform;

        Json::object position;
        position["x"] 
            = Json(static_cast<double>(transform.LocalPosition().X()));
        position["y"]
            = Json(static_cast<double>(transform.LocalPosition().Y()));
        object["position"] = position;

        Json::object scale;
        scale["x"]
            = Json(static_cast<double>(transform.LocalScale().X()));
        scale["y"]
            = Json(static_cast<double>(transform.LocalScale().Y()));
        object["scale"] = scale;

        Json::object rotation;
        rotation["z"]
            = Json(static_cast<double>(transform.LocalRotation2D()));
        object["rotation"] = rotation;

        return Json(object);
    }

    template<>
    Json JsonSerializer::Serialize(Colliders &colliders)
    {
        Json::array items;

        for (auto &collider : colliders)
            items.push_back(Serialize(collider));

        return Json(items);
    }

    // Forward declaration
    template<>
    void JsonSerializer::Deserialize(const Json &data, Colliders &colliders);

    template<>
    void JsonSerializer::Deserialize(const Json &data, Collider * &collider)
    {
        static std::string path = "Assets/Meshes/";

        ecs::Transform &transform = collider->transform;

        const Json &pos = data["position"];

        transform.SetPosition({
            static_cast<float>(pos["x"].number_value()),
            static_cast<float>(pos["y"].number_value()),
            0});

        const Json &scale = data["scale"];
        float x = static_cast<float>(scale["x"].number_value());
        float y = static_cast<float>(scale["y"].number_value());

        UAssert(!Math::IsZero(x) && !Math::IsZero(y),
            "Scale components cannot be zero on colliders.");

        transform.SetScale({
            Math::IsZero(x) ? 1 : x,
            Math::IsZero(y) ? 1 : y,
            0});

        const Json &rot = data["rotation"];

        transform.SetRotation2D(static_cast<float>(rot["z"].number_value()));

        auto items = data.object_items();

        auto itr = items.find("mesh");

        // polygon
        if (itr != items.end())
        {
            Polygon *poly = new Polygon();
            collider->shape = poly;
            poly->Set(path + data["mesh"].string_value());
        }
        else
        {
            Ellipse *ellipse = new Ellipse();
            collider->shape = ellipse;
            ellipse->Set(transform.RootRotation2D(),
                         { x, y },
                         Vec2(transform.LocalPosition()));
        }

        auto &children = data["children"];

        if (children.is_array())
        {
            Deserialize<Colliders>(children, collider->children);
        }
    }

    template<>
    void JsonSerializer::Deserialize(const Json &data, Colliders &colliders)
    {
        for (auto &item : data.array_items())
        {
            Collider *collider = new Collider();

            Deserialize<Collider *>(item, collider);

            colliders.push_back(collider);
        }
    }
}