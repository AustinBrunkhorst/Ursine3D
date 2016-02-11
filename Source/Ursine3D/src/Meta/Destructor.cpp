/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Destructor.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "Destructor.h"

namespace ursine
{
    namespace meta
    {
        Destructor::Destructor(void)
            : Invokable( "INVALID" )
            , m_classType( { Type::Invalid } )
        {
        
        }

        Destructor::Destructor(Type classType, DestructorInvokerBase *invoker)
            : Invokable( "destructor" )
            , m_classType( classType )
            , m_invoker( invoker )
        {
        
        }

        Type Destructor::GetClassType(void) const
        {
            return m_classType;
        }

        void Destructor::Invoke(Variant &instance) const
        {
            UAssert( IsValid( ), 
                "Invalid constructor invoked" );

            UAssert( m_classType == instance.GetType( ), 
                "Destructor called on incompatible type" );
        
            m_invoker->Invoke( instance );

            delete instance.m_base;

            instance.m_base = nullptr;
        }

        const Destructor &Destructor::Invalid(void)
        {
            static Destructor invalid;

            return invalid;
        }

        bool Destructor::IsValid(void) const
        {
            return m_invoker != nullptr;
        }
    }
}