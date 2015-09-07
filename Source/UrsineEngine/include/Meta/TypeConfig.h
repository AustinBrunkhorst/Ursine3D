#pragma once

#include <type_traits>

#define typeof(type) ursine::meta::Type( ursine::meta::TypeInfo< ursine::meta::CleanedType< type > >::ID )

namespace ursine
{
    namespace meta
    {
        template<typename T>
        using CleanedType = typename std::remove_cv< typename std::remove_reference< T >::type >::type;

        typedef unsigned TypeID;
    }
}