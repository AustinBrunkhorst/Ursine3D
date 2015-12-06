/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Constructor.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "Invokable.h"

#include "Constructor.h"

namespace ursine
{
    namespace meta
    {
        Constructor::Constructor(void)
            : Invokable( "INVALID" )
            , m_isDynamic( false )
            , m_classType( { Type::Invalid } )
            , m_invoker( nullptr ) { }

        Constructor::Constructor(
            Type classType, 
            InvokableSignature signature, 
            Invoker invoker, 
            bool isDynamic
        )
            : Invokable( "constructor" )
            , m_isDynamic( isDynamic )
            , m_classType( classType )
            , m_invoker( invoker )
        {
            m_signature = signature;
        }

        const Constructor &Constructor::Invalid(void)
        {
            static Constructor invalid;

            return invalid;
        }

        Type Constructor::GetClassType(void) const
        {
            return m_classType;
        }

        bool Constructor::IsValid(void) const
        {
            return m_invoker != nullptr;
        }

        bool Constructor::IsDynamic(void) const
        {
            return m_isDynamic;
        }

        Variant Constructor::InvokeVariadic(ArgumentList &arguments) const
        {
            UAssert( IsValid( ), "Invalid constructor invoked" );

            return m_invoker( arguments );
        }
    }
}
