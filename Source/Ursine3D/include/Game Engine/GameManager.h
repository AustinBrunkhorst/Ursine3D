#pragma once

#define NATIVE_GAME_MANAGER META_OBJECT

#include "Type.h"
#include "Component.h"

namespace ursine
{
    class GameContext;

    class GameManager : public meta::Object
    {
    public:
        typedef std::shared_ptr<GameManager> Handle;

        GameManager(GameContext *context, const ursine::Json &configObj);
        ~GameManager(void);

        template<typename T>
        ecs::Component *GetConfigComponent(void);

    protected:
        GameContext *m_context;

    private:
        std::unordered_map<meta::Type, ecs::Component*> m_configComponents;
    } Meta(WhiteListMethods);
}

#include "GameManager.hpp"