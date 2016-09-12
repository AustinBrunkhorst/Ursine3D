/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** GameManager.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

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
        T *GetConfigComponent(void);

        virtual void OnInitialize(GameContext *context, const Json &configObj);

    protected:
        GameContext *m_context;

        ecs::World *setWorld(const resources::ResourceReference &resource, bool loadConfiguredSystems = true);

    private:
        std::unordered_map<meta::Type, ecs::Component*> m_configComponents;
    } Meta(WhiteListMethods, EnablePtrType);
}

#include "GameManager.hpp"