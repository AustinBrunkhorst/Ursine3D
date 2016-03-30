/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** UIConfigComponent.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

#include <UIScreen.h>
#include <UIScreenManager.h>
#include <UIScreenData.h>

#include "UIEvents.h"

#define DECLARE_SCREEN(screenName)                                                                                           \
public:                                                                                                                      \
    EditorResourceField(ursine::resources::UIScreenData, screenName, Get##screenName##Ref, Set##screenName##Ref)             \
    inline const ursine::resources::ResourceReference &Get##screenName##Ref(void) const                                      \
    {                                                                                                                        \
        return m_##screenName;                                                                                               \
    }                                                                                                                        \
    inline void Set##screenName##Ref(const ursine::resources::ResourceReference &value)                                      \
    {                                                                                                                        \
        m_##screenName = value;                                                                                              \
        NOTIFY_COMPONENT_CHANGED( STRINGIFY( screenName ), value );                                                          \
    }                                                                                                                        \
    inline ursine::UIScreen *Add##screenName(const ursine::UIScreenConfig &config = { }, const ursine::Json &initData = { }) \
    {                                                                                                                        \
        return addScreen( m_##screenName, config, initData );                                                                \
    }                                                                                                                        \
    template<typename EventType>                                                                                             \
    inline void Trigger##screenName##Event(const EventType &data) const                                                      \
    {                                                                                                                        \
        triggerEvent( #screenName, m_##screenName, EventType::DefaultName, data );                                           \
    }                                                                                                                        \
    inline void Remove##screenName(void)                                                                                     \
    {                                                                                                                        \
        removeScreen( m_##screenName );                                                                                      \
    }                                                                                                                        \
    inline ursine::UIScreen *Get##screenName(void)                                                                           \
    {                                                                                                                        \
        return getScreen( m_##screenName );                                                                                  \
    }                                                                                                                        \
    inline bool Has##screenName(void) const                                                                                  \
    {                                                                                                                        \
        return getScreen( m_##screenName ) != nullptr;                                                                       \
    }                                                                                                                        \
private:                                                                                                                     \
    ursine::resources::ResourceReference m_##screenName;                                                                     \
public:                                                                                                                      \

class UIScreensConfig : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    UIScreensConfig(void);

    DECLARE_SCREEN( Splash );
    DECLARE_SCREEN( MainMenu );
    DECLARE_SCREEN( Pause );
    DECLARE_SCREEN( Credits );
    DECLARE_SCREEN( PlayerHUD );

private:
    std::unordered_map<ursine::GUID, ursine::UIScreenID, ursine::GUIDHasher> m_created;

    ursine::UIScreen *addScreen(
        const ursine::resources::ResourceReference &reference,
        const ursine::UIScreenConfig &config,
        const ursine::Json &initData
    );

    template<typename EventType>
    void triggerEvent(
        const char *screenName, 
        const ursine::resources::ResourceReference &target,
        const std::string &eventName, 
        const EventType &data
    ) const;

    ursine::UIScreen *getScreen(const ursine::resources::ResourceReference &reference) const;
    void removeScreen(const ursine::resources::ResourceReference &reference);
} Meta(Enable);

#include "UIScreensConfigComponent.hpp"

#undef DECLARE_SCREEN
