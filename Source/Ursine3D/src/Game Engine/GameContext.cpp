#include "UrsinePrecompiled.h"

#include "GameContext.h"

namespace ursine
{
    GameContext::GameContext(Scene *scene, void *defaultSender) 
        : EventDispatcher( defaultSender )
        , m_scene( scene )
        , m_manager( nullptr ) { }

    GameManager *GameContext::GetManager(void)
    {
        return m_manager;
    }

    void GameContext::SetManager(GameManager *manager)
    {
        m_manager = manager;
    }
}