/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ZoneComponent.cpp
** - Logic Zone Component
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
**
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"
#include "ZoneComponent.h"
#include "Entity.h"

namespace ursine
{
    namespace ecs
    {
        Zone::Zone()
            : BaseComponent()
            , width(1)
            , height(1)
        {
        }

        Zone::~Zone()
        {
        }

        Zone::Zone(const Zone &other)
            : BaseComponent()
            , width(other.width)
            , height(other.height)
            , _aabb(other._aabb)
            , _transform(other._transform)
            , type(other.type)
        {
        }

        const Zone &Zone::operator=(const Zone &rhs)
        {
            width = rhs.width;
            height = rhs.height;
            _aabb = rhs._aabb;

            return (*this);
        }

        void Zone::OnInitialize()
        {
            _transform = GetOwner()->GetTransform();
            UpdateAABB();
        }

        void Zone::SetWidth(float Width)
        {
            width = Width;
            UpdateAABB();
        }

        void Zone::SetHeight(float Height)
        {
            height = Height;
            UpdateAABB();
        }

        float Zone::GetWidth(void)
        {
            return width * _transform->ScaleWC().X();
        }

        float Zone::GetHeight(void)
        {
            return height * _transform->ScaleWC().Y();
        }

        bool Zone::Contains(AABB const& aabb)
        {
            return _aabb.Overlap(aabb);
        }

        void Zone::UpdateAABB(void)
        {
            Vec2 center = Vec2(_transform->PositionWC());

            float w = GetWidth()
                , h = GetHeight();

            _aabb.low_bound.Set(center.X() - w, center.Y() - h);
            _aabb.up_bound.Set(center.X() + w, center.Y() + h);
        }
    }

    template<>
    Json JsonSerializer::Serialize(ZoneType &instance)
    {
        return Json(static_cast<int>(instance));
    }

    template<>
    void JsonSerializer::Deserialize(const Json &data, ZoneType &out)
    {
        out = static_cast<ZoneType>(data.int_value());
    }
}