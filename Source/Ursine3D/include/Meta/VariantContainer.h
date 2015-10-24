/* ----------------------------------------------------------------------------
** Team Bear King
** © 201x DigiPen Institute of Technology, All Rights Reserved.
**
** VariantContainer.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
** --------------------------------------------------------------------------*/

#pragma once

#include "VariantBase.h"

#include "TraitUtils.h"

#define DEFAULT_TYPE_HANDLER(typeName)                                              \
    template<typename U = T>                                                        \
    typeName get##typeName(                                                         \
        typename std::enable_if<                                                    \
            !std::is_convertible<typename TypeOrEnumType<U>::type, typeName>::value \
        >::type* = nullptr                                                          \
    ) const;                                                                        \
    template<typename U = T>                                                        \
    typeName get##typeName(                                                         \
        typename std::enable_if<                                                    \
            std::is_convertible<typename TypeOrEnumType<U>::type, typeName>::value  \
        >::type* = nullptr                                                          \
    ) const;                                                                        \

namespace ursine
{
    namespace meta
    {
        template<typename T>
        class VariantContainer : public VariantBase
        {
        public:
            VariantContainer(const T &value);
            VariantContainer(const T &&value);

            Type GetType(void) const override;

            void *GetPtr(void) const override;

            int ToInt(void) const override;
            bool ToBool(void) const override;
            float ToFloat(void) const override;
            double ToDouble(void) const override;
            std::string ToString(void) const override;

            VariantBase *Clone(void) const override;

        private:
            friend class Variant;

            T m_value;

            VariantContainer &operator=(const VariantContainer &rhs) = delete;

            DEFAULT_TYPE_HANDLER( int );
            DEFAULT_TYPE_HANDLER( bool );
            DEFAULT_TYPE_HANDLER( float );
            DEFAULT_TYPE_HANDLER( double );

            template<typename U = T>             
            std::string getString(
                typename std::enable_if<                 
                    !std::is_arithmetic<U>::value 
                >::type* = nullptr                       
            ) const;          

            template<typename U = T>                     
            std::string getString(
                typename std::enable_if<                 
                    std::is_arithmetic<U>::value
                >::type* = nullptr                       
            ) const;                                           
        };
    }
}

#undef DEFAULT_TYPE_HANDLER

#include "Impl/VariantContainer.hpp"
#include "Impl/VariantContainerStandardTypes.hpp"