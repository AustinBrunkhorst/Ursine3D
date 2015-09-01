#pragma once

#include "Utils.h"

namespace ursine
{
    class LayerID
    {
        uint64 _id;

        friend
        void JsonSerializer::Deserialize(const Json &data, LayerID &out);

    public:
        LayerID(uint64 value);

        uint64 ID(void) const;
    };

    extern template
    Json JsonSerializer::Serialize(LayerID &instance);

    extern template
    void JsonSerializer::Deserialize(const Json &data, LayerID &out);
}