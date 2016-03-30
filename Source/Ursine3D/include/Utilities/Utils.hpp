/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Utils.hpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

namespace ursine
{
    namespace utils
    {
        template<typename A, typename B>
        inline bool IsFlagSet(A value, B flag)
        {
            return (value & flag) == flag;
        }

        template<typename A, typename B>
        inline void FlagSet(A &value, B flag)
        {
            value |= flag;
        }

        template<typename A, typename B>
        inline void FlagUnset(A &value, B flag)
        {
            value &= ~flag;
        }

        template<typename A>
        inline A FlagNegateAll(A value)
        {
            return ~value;
        }

        template<typename A>
        constexpr A IndexToMask(A value)
        {
            return static_cast<A>( 1 ) << value;
        }

        template<typename ElementType, typename...Elements>
        constexpr auto MakeArray(Elements &&...elements)
            -> std::array<ElementType, sizeof...(Elements)>
        {
            return { { std::forward<Elements>( elements )... } };
        }

        template<typename Container, class Predicate>
        void InsertionSort(Container &v, Predicate predicate)
        {
            for (auto i = v.begin( ); i != v.end( ); ++i)
            {
                std::rotate( std::upper_bound( v.begin( ), i, *i, predicate ), i, i + 1 );
            }
        }

        template<typename Container, class T, class Predicate>
        void InsertionSort(Container &v, const T &item, Predicate predicate)
        {
            if (v.empty( ))
            {
                v.push_back( item );
            }
            else
            {
                v.insert( std::upper_bound( v.begin( ), v.end( ), item, predicate ), item );
            }
        }
    }
}
