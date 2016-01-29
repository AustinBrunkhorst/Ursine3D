/* ----------------------------------------------------------------------------
** Team Bear King
** © 201x DigiPen Institute of Technology, All Rights Reserved.
**
** TypeConfig.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
** --------------------------------------------------------------------------*/

#pragma once

#include <type_traits>

// converts the type name into a meta::Type instance
#define typeof(expr)                                                  \
	ursine::meta::Type( 					                          \
		ursine::meta::TypeInfo<                                       \
			ursine::meta::CleanedType<                                \
                typename ursine::meta_traits::RemoveArray<expr>::type \
            >                                                         \
		>::ID,                                                        \
        ursine::meta_traits::IsArray<expr>::value                     \
	)                                                                 \

namespace ursine
{
    namespace meta
    {
        template<typename T>
        using CleanedType = 
        	typename std::remove_cv< 
        		typename std::remove_reference< T >::type 
    		>::type;

        typedef unsigned TypeID;
    }
}