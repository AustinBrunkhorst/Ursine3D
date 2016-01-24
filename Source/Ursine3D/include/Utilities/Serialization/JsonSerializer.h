/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** JsonSerializer.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "Json.h"

namespace ursine
{
    class JsonSerializer final
    {
    public:
        typedef Json(*Serializer)(void *);
        typedef void(*Deserializer)(const Json &, void *);

        Serializer SerializeFn { nullptr };
        Deserializer DeserializeFn { nullptr };

        JsonSerializer(void) = default;
        JsonSerializer(Serializer serialize, Deserializer deserialize);

        template<typename DataType>
        static Json Serialize(DataType &instance);

        template<typename DataType>
        static void Deserialize(const Json &data, DataType &out);

        // Deserializes the JSON but uses "default_value" if "data" is null.
        template<typename DataType, typename DefaultType>
        static void Deserialize(const Json &data, DataType &out, const DefaultType &default_value);
    };

    ////////////////////////////////////////////////////////////////////////////
    // Standard Serializers
    ////////////////////////////////////////////////////////////////////////////

    extern template
    Json JsonSerializer::Serialize(bool &instance);

    extern template 
    Json JsonSerializer::Serialize(int &instance);

    extern template 
    Json JsonSerializer::Serialize(float &instance);

    extern template 
    Json JsonSerializer::Serialize(double &instance);

    extern template 
    Json JsonSerializer::Serialize(std::string &instance);

   /////////////////////////////////////////////////////////////////////////////
   // Standard Deserializers
   /////////////////////////////////////////////////////////////////////////////

    extern template
    void JsonSerializer::Deserialize(const Json &data, bool &out);

    extern template
    void JsonSerializer::Deserialize(const Json &data, int &out);

    extern template
    void JsonSerializer::Deserialize(const Json &data, float &out);

    extern template
    void JsonSerializer::Deserialize(const Json &data, double &out);

    extern template
    void JsonSerializer::Deserialize(const Json &data, std::string &out);
}

#include "JsonSerializer.hpp"