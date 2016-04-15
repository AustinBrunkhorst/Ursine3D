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

        GameManager(void);
        ~GameManager(void);

        GameContext *GetContext(void);

        template<typename T>
        ecs::Component *GetConfigComponent(void);

        virtual void OnInitialize(GameContext *context, const ursine::Json &configObj);

    protected:
        GameContext *m_context;

    private:
        std::unordered_map<meta::Type, ecs::Component*> m_configComponents;
    } Meta(WhiteListMethods, EnablePtrType);
}

#include "GameManager.hpp"