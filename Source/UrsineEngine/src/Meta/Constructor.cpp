#include "UrsinePrecompiled.h"

#include "Invokable.h"

#include "Constructor.h"

namespace Ursine
{
    Constructor::Constructor(void)
        : Invokable( "INVALID" )
        , m_classType( { Type::Invalid } )
        , m_invoker( nullptr ) { }

    Constructor::Constructor(Type classType, InvokableSignature signature, Invoker invoker)
        : Invokable( "constructor" )
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

    Variant Constructor::InvokeVariadic(ArgumentList &arguments) const
    {
		UAssert( IsValid( ), "Invalid constructor invoked" );

        return m_invoker( arguments );
    }
}
