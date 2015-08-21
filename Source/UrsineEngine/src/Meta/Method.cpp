#include "UrsinePrecompiled.h"

#include "Method.h"

namespace Ursine
{
    Method::Method(void)
        : Invokable( "INVALID" )
        , m_classType( { Type::Invalid } )
        , m_invoker( nullptr ) { }

    const Method &Method::Invalid(void)
    {
        static Method invalid;

        return invalid;
    }

    Type Method::GetClassType(void) const
    {
        return m_classType;
    }

    bool Method::IsValid(void) const
    {
        return m_invoker != nullptr;
    }

    Variant Method::Invoke(Variant &instance, ArgumentList &arguments) const
    {
#ifdef CONFIG_DEBUG

		UAssert( IsValid( ), "Invalid method invoked" );

#endif

		UAssert( instance.GetType( ) == m_classType, 
			"Incompatible method invoked with instance" );

        return m_invoker( instance, arguments );
    }
}
