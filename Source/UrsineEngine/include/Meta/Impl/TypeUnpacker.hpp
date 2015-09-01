#pragma once

namespace ursine
{
    template<class... Types>
    struct TypeUnpacker { };

    template<>
    struct TypeUnpacker<>
    {
        static void Apply(Type::List &types) { }
    };

    template<class First, class... Types>
    struct TypeUnpacker<First, Types...>
    {
        static void Apply(Type::List &types)
        {
            types.emplace_back( Type::Get<First>( ) );

            TypeUnpacker<Types...>::Apply( types );
        }
    };
}