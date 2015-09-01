#include "Type.h"
#include "TypeUnpacker.hpp"

namespace std
{
    template<>
    struct hash<ursine::InvokableSignature>
    {
        size_t operator()(const ursine::InvokableSignature &signature) const
        {
            size_t seed = 0;

            for (auto &type : signature)
                seed ^= type.GetID( ) + 0x9e3779b9 + (seed << 6) + (seed >> 2);

            return seed;
        }
    };
}

namespace ursine
{
    template<typename ...Types>
    InvokableSignature Invokable::CreateSignature(void)
    {
        static InvokableSignature signature;

        static auto initial = true;

        if (initial)
        {
            TypeUnpacker<Types...>::Apply( signature );

            initial = false;
        }

        return signature;
    }
}