/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Filter.hpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

namespace ursine
{
    namespace ecs
    {
        namespace
        {
            // definition
            template<class... Types>
            struct MaskUnpack { };

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
                    static_assert( std::is_base_of<Component, First>::value,
                        "Not all types are components!" );

                    return GetComponentMask( First ) | MaskUnpack<Types...>::Apply( );
                }
            };
        }

        inline ComponentTypeMask Filter::OneMask(void) const
        {
            return m_maskOne;
        }

        inline ComponentTypeMask Filter::ExcludeMask(void) const
        {
            return m_maskExclude;
        }

        inline ComponentTypeMask Filter::ContainsMask(void) const
        {
            return m_maskContains;
        }

        template<class... Types>
        Filter &Filter::All(void)
        {
            m_maskContains |= MaskUnpack<Types...>::Apply( );

            return *this;
        }

        template<class... Types>
        Filter &Filter::Exclude(void)
        {
            m_maskExclude |= MaskUnpack<Types...>::Apply( );

            return *this;
        }

        template<class... Types>
        Filter &Filter::One(void)
        {
            m_maskOne |= MaskUnpack<Types...>::Apply( );

            return *this;
        }
    }
}