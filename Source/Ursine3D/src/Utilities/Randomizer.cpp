#include "UrsinePrecompiled.h"

#include "Randomizer.h"

namespace ursine
{
    Randomizer::Randomizer(void)
        : _min(0)
        , _max(0)
    {

    }

    Randomizer::Randomizer(float min, float max)
        : _min(min)
        , _max(max)
    {

    }

    float Randomizer::GetValue(void) const
    {
        return math::Rand(_min, _max);
    }

    float Randomizer::GetMin(void) const
    {
        return _min;
    }

    void Randomizer::SetMin(float min)
    {
        _min = min;
    }

    float Randomizer::GetMax(void) const
    {
        return _max;
    }

    void Randomizer::SetMax(float max)
    {
        _max = max;
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