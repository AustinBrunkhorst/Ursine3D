/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Filter.hpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/


#include "ComponentRegistrar.h"

namespace ursine
{
    namespace ecs
    {
        namespace
        {
            // definition
            template<class... Types>
            struct MaskUnpack {};

            // base case
            template<>
            struct MaskUnpack<>
            {
                static ComponentTypeMask Apply(void)
                {
                    return 0;
                }
            };

            // all other cases
            template<class First, class... Types>
            struct MaskUnpack<First, Types...>
            {
                static ComponentTypeMask Apply(void)
                {
                    static_assert(std::is_base_of<Component, First>::value,
                        "Not all types are components!");

                    return GetComponentMask(First) | MaskUnpack<Types...>::Apply();
                }
            };
        }

        inline ComponentTypeMask Filter::OneMask(void) const
        {
            return _mask_one;
        }

        inline ComponentTypeMask Filter::ExcludeMask(void) const
        {
            return _mask_exclude;
        }

        inline ComponentTypeMask Filter::ContainsMask(void) const
        {
            return _mask_contains;
        }

        template<class... Types>
        Filter &Filter::All(void)
        {
            _mask_contains |= MaskUnpack<Types...>::Apply();

            return *this;
        }

        template<class... Types>
        Filter &Filter::Exclude(void)
        {
            _mask_exclude |= MaskUnpack<Types...>::Apply();

            return *this;
        }

        template<class... Types>
        Filter &Filter::One(void)
        {
            _mask_one |= MaskUnpack<Types...>::Apply();

            return *this;
        }
    }
}