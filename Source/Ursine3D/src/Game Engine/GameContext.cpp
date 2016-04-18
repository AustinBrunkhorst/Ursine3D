/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** GameContext.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "GameContext.h"

namespace ursine
{
    GameContext::GameContext(Scene *scene, void *defaultSender) 
        : EventDispatcher( defaultSender )
        , m_scene( scene )
        , m_manager( nullptr ) { }

    Scene *GameContext::GetScene(void)
    {
        return m_scene;
    }

    GameManager *GameContext::GetManager(void)
    {
        return m_manager;
    }

    void GameContext::SetManager(GameManager *manager)
    {
        m_manager = manager;
    }
}