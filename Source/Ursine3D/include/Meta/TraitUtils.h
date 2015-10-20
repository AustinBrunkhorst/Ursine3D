#pragma once

namespace ursine
{
    template<typename T, typename = void>
    struct TypeOrEnumType
    {
        typedef T type;
    };

    template<typename T>
    struct TypeOrEnumType<T,
        typename std::enable_if<
            std::is_enum<T>::value
        >::type
    >
    {
        typedef typename std::underlying_type<T>::type type;
    };
}
