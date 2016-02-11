/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Randomizer.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "Randomizer.h"

namespace ursine
{
    Randomizer::Randomizer(void)
        : m_distributor(0.0f, 0.0f)
    {

    }

    Randomizer::Randomizer(float min, float max)
        : m_distributor(min, max)
    {

    }

    float Randomizer::GetValue(void)
    {
        return m_distributor(m_engine);
    }

    float Randomizer::GetMin(void) const
    {
        return m_distributor.min();
    }

    void Randomizer::SetMin(float min)
    {
        m_distributor = std::uniform_real_distribution<float>(min, m_distributor.max());
    }

    float Randomizer::GetMax(void) const
    {
        return m_distributor.max();
    }

    void Randomizer::SetMax(float max)
    {
        m_distributor = std::uniform_real_distribution<float>(m_distributor.min(), max);
    }

    template<>
    Json JsonSerializer::Serialize(Randomizer &instance)
    {
        Json::object object;

        object["min"] = Json(instance.GetMin());
        object["max"] = Json(instance.GetMax());

        return Json(object);
    }

    template<>
    void JsonSerializer::Deserialize(const Json &data, Randomizer &out)
    {
        out.SetMin(static_cast<float>(data["min"].number_value()));
        out.SetMax(static_cast<float>(data["max"].number_value()));
    }
}