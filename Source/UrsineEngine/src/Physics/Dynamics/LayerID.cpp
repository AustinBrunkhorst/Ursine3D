#include <UrsinePrecompiled.h>

namespace ursine
{
    LayerID::LayerID(uint64 value)
        : _id(value)
    {
        UAssert(_id < 64, "There are only 64 layers");
    }

    uint64 LayerID::ID(void) const
    {
        return _id;
    }

    template <>
    Json JsonSerializer::Serialize(LayerID &instance)
    {
        return { static_cast<int>(instance.ID()) };
    }

    template <>
    void JsonSerializer::Deserialize(const Json &data, LayerID &out)
    {
        UAssert(data.is_number(), "The layer id needs to be represented as an integer.")

        out._id = data.int_value();
    }
}