/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Invokable.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "Invokable.h"

namespace ursine
{
    namespace meta
    {
        Invokable::Invokable(const std::string &name) 
            : m_name( name ) { }

        const InvokableSignature &Invokable::GetSignature(void) const
        {
            return m_signature;
        }

        const std::string &Invokable::GetName(void) const
        {
            return m_name;
        }
    }
}
