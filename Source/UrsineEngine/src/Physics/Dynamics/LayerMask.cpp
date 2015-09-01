#include <UrsinePrecompiled.h>

namespace ursine
{
    LayerMask::LayerMask(void)
    {
        // set all bits to one
        _mask = -1;
    }

    LayerMask::LayerMask(const std::vector<LayerID> &layers)
        : _mask(0)
    {
        // Set all of the bit flags in the mask based on the provided layer IDs
        for (auto &layer : layers)
            _mask |= uint64(1) << layer.ID();
    }

    uint64 LayerMask::Mask(void) const
    {
        return _mask;
    }

    bool LayerMask::Contains(const LayerID &id) const
    {
        uint64 id_mask = 0;
        id_mask |= uint64(1) << id.ID();

        return utils::IsFlagSet(_mask, id_mask);
    }

    template <>
    Json JsonSerializer::Serialize(LayerMask &instance)
    {
        Json::array mask_array;
        uint64 mask = instance.Mask();

        for (uint64 i = 0; i < 64; ++i)
        {
            if (utils::IsFlagSet(mask, 1 << i))
            {
                // push i
                mask_array.push_back(Json(static_cast<int>(i)));
            }
        }

        return { mask_array };
    }

    template<>
    void JsonSerializer::Deserialize(const Json &data, LayerMask &out)
    {
        UAssert(data.is_array(), "You must create an array of layer IDs ([0, 1, 3])");

        std::vector<LayerID> ids;

        for (auto &item : data.array_items())
        {
            ids.emplace_back(item.int_value());
        }

        if (ids.size())
            out = LayerMask(ids);
        else
            out = LayerMask();
    }
}