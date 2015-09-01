#pragma once

#include <vector>
#include <unordered_map>

namespace ursine
{
    namespace meta
    {
        class Type;

        typedef std::vector<Type> InvokableSignature;

        template<typename T>
        using InvokableOverloadMap = std::unordered_multimap<InvokableSignature, T>;
    }
}