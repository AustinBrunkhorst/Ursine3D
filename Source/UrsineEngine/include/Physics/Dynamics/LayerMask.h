#pragma once

#include "LayerID.h"

namespace ursine
{
    class LayerMask
    {
        uint64 _mask;

    public:
        LayerMask(/* Include Everything */);
        LayerMask(const std::vector<LayerID> &layers);

        uint64 Mask(void) const;

        bool Contains(const LayerID &id) const;
    };

    extern template
    Json JsonSerializer::Serialize(LayerMask &instance);

    extern template
    void JsonSerializer::Deserialize(const Json &data, LayerMask &out);
}