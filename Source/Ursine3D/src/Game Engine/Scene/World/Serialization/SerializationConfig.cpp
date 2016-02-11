/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SerializationConfig.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "SerializationConfig.h"

namespace ursine
{
    namespace ecs
    {
        SerializationException::SerializationException(const std::string &error)
            : m_error( error )
        {
      
        }

        const std::string &SerializationException::GetError(void) const 
        {
            return m_error;
        }
    }
}