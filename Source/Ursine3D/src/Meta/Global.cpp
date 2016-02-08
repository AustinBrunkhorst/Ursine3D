/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Global.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "Global.h"

namespace ursine
{
    namespace meta
    {
        Global::Global(void)
            : m_type { Type::Invalid }
            , m_parentType { Type::Invalid }
            , m_name( "INVALID" )
            , m_getter( nullptr ) 
            , m_setter( nullptr ) { }

        Global::Global(
            const std::string &name, 
            Type type, 
            GlobalGetterBase *getter,
            GlobalSetterBase *setter,
            Type parentType
        ) 
            : m_type( type )
            , m_parentType( parentType ) 
            , m_name( name ) 
            , m_getter( getter )
            , m_setter( setter ) { }

        Global::~Global(void)
        {
            delete m_getter;
            delete m_setter;
        }

        bool Global::IsValid(void) const
        {
            return m_getter != nullptr;
        }

        bool Global::IsReadOnly(void) const
        {
            return m_setter == nullptr;
        }

        Type Global::GetType(void) const
        {
            return m_type;
        }

        Type Global::GetParentType(void) const
        {
            return m_parentType;
        }

        const std::string &Global::GetName(void) const
        {
            return m_name;
        }

        Variant Global::GetValue(void) const
        {
            return m_getter->GetValue( );
        }

        bool Global::SetValue(const Argument &value) const
        {
            // read only?
            if (m_setter != nullptr)
            {
                m_setter->SetValue( value );

                return true;
            }
               
            return false;
        }
    }
}
