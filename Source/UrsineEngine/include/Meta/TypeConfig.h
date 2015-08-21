#pragma once

#include <type_traits>

namespace Ursine
{
    template<typename T>
    using CleanedType = typename std::remove_cv< typename std::remove_reference< T >::type >::type;

    typedef unsigned TypeID;
}