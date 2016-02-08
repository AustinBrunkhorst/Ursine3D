/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Function.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "Function.h"

namespace ursine
{
    namespace meta
    {
        Function::Function(void)
            : Invokable( "INVALID" )
            , m_parentType( { Type::Invalid } )
            , m_invoker( nullptr ) { }

        const Function &Function::Invalid(void)
        {
            static Function invalid;

            return invalid;
        }

        Type Function::GetParentType(void) const
        {
            return m_parentType;
        }

        bool Function::IsValid(void) const
        {
            return m_invoker != nullptr;
        }

        Variant Function::InvokeVariadic(ArgumentList &arguments) const
        {
        #ifdef CONFIG_DEBUG

            UAssert( IsValid( ), "Invalid function invocation" );

        #endif
        
            return m_invoker->Invoke( arguments );
        }
    }
}
