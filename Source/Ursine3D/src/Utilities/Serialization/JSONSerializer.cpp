/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** JSONSerializer.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "JsonSerializer.h"

namespace ursine
{
    JsonSerializer::JsonSerializer(Serializer serialize, Deserializer deserialize)
        : SerializeFn(serialize)
        , DeserializeFn(deserialize) {}

    ////////////////////////////////////////////////////////////////////////////
    // Integral Serializers
    ////////////////////////////////////////////////////////////////////////////

    template<>
    Json JsonSerializer::Serialize(bool &instance)
    {
        return Json(instance);
    }

    template<>
    Json JsonSerializer::Serialize(int &instance)
    {
        return Json(instance);
    }

    template<>
    Json JsonSerializer::Serialize(float &instance)
    {
        return Json(static_cast<double>(instance));
    }

    template<>
    Json JsonSerializer::Serialize(double &instance)
    {
        return Json(instance);
    }

    template<>
    Json JsonSerializer::Serialize(std::string &instance)
    {
        return Json(instance);
    }

    ////////////////////////////////////////////////////////////////////////////
    // Integral Deserializers
    ////////////////////////////////////////////////////////////////////////////

    template<>
    void JsonSerializer::Deserialize(const Json &data, bool &out)
    {
        out = data.bool_value();
    }

    template<>
    void JsonSerializer::Deserialize(const Json &data, int &out)
    {
        out = data.int_value();
    }

    template<>
    void JsonSerializer::Deserialize(const Json &data, float &out)
    {
        out = static_cast<float>(data.number_value());
    }

    template<>
    void JsonSerializer::Deserialize(const Json &data, double &out)
    {
        out = data.number_value();
    }

    template<>
    void JsonSerializer::Deserialize(const Json &data, std::string &out)
    {
        out = data.string_value();
    }
}
