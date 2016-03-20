/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** JSScreenManager.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "JSUIScreenManager.h"

namespace ursine
{
    JSConstructor(JSUIScreenManager::JSUIScreenManager)
        : m_manager( JSGetGlobalScreenManager( ) ) { }

    JSUIScreenManager::JSUIScreenManager(const JSUIScreenManager &rhs)
        : m_manager( rhs.m_manager ) { }
}