/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EnumBase.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "EnumBase.h"

#include "Variant.h"
#include "Argument.h"

namespace ursine
{
    namespace meta
    {
        EnumBase::EnumBase(const std::string &name, TypeID owner)
            : m_parentType( owner )
            , m_name( name ) { }

        Type EnumBase::GetParentType(void) const
        {
            return m_parentType;
        }

        const std::string &EnumBase::GetName(void) const
        {
            return m_name;
        }
    }
}