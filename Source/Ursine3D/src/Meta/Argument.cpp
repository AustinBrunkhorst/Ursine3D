/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Argument.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "Type.h"

#include "Argument.h"

#include "Variant.h"

namespace ursine
{
    namespace meta
    {
        Argument::Argument(void)
            : m_type( { Type::Invalid } )
            , m_data( nullptr ) { }

        Argument::Argument(const Argument &rhs) 
            : m_type( rhs.m_type )
            , m_data( rhs.m_data ) { }

        Argument::Argument(Variant &obj) 
            : m_type( obj.GetType( ) )
            , m_data( obj.getPtr( ) ) { }

        Argument::Argument(const Variant &obj) 
            : m_type( obj.GetType( ) )
            , m_data( obj.getPtr( ) ) { }

        Argument &Argument::operator=(const Argument &rhs) 
        {
            m_data = rhs.m_data;

            const_cast<Type&>( m_type ) = rhs.m_type;

            return *this;
        }

        Type Argument::GetType(void) const
        {
            return m_type;
        }

        void *Argument::GetPtr(void) const
        {
            return const_cast<void *>( m_data );
        }
    }
}
