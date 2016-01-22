/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** MetaTraits.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

namespace ursine
{
    template<typename T>
    class Array;

    namespace meta_traits
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

        ///////////////////////////////////////////////////////////////////////////

        template<typename T>
        struct RemoveArray
        {
            typedef T type;
        };

        template<typename T>
        struct RemoveArray<Array<T>>
        {
            typedef T type;
        };

        ///////////////////////////////////////////////////////////////////////////

        template<typename T>
        struct IsArray
        {
            static const bool value = false;
        };

        template<typename T>
        struct IsArray<Array<T>>
        {
            static const bool value = true;
        };

        ///////////////////////////////////////////////////////////////////////////

        template<typename T>
        using ArrayByValue = Array<T>;

        template<typename T>
        using ArrayByReference = typename std::add_lvalue_reference<Array<T>>::type;
    }
}
