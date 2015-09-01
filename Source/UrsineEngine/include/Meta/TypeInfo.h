#pragma once

#include "TypeConfig.h"

namespace ursine
{
    namespace meta
    {
        struct TypeData;

        template<typename T>
        struct TypeInfo
        {
            static TypeID ID;
            static bool Defined;

            static void Register(TypeID id, TypeData &data, bool beingDefined);

        private:
            template<typename U = T>
            static void applyTrivialAttributes(TypeData &data, typename std::enable_if< !std::is_trivial<U>::value >::type* = nullptr);

            template<typename U = T>
            static void applyTrivialAttributes(TypeData &data, typename std::enable_if< std::is_trivial<U>::value >::type* = nullptr);
        };
    }
}

#include "Impl/TypeInfo.hpp"