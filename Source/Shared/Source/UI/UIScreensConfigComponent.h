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

#include <UIScreenManager.h>
#include <UIScreenData.h>

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

    DECLARE_SCREEN( Pause );
    DECLARE_SCREEN( MainMenu );
    DECLARE_SCREEN( Credits );

private:
    std::unordered_map<ursine::GUID, ursine::UIScreen*, ursine::GUIDHasher> m_created;

    ursine::UIScreen *addScreen(
        const ursine::resources::ResourceReference &reference,
        const ursine::UIScreenConfig &config,
        const ursine::Json &initData
    );

    ursine::UIScreen *getScreen(const ursine::resources::ResourceReference &reference) const;
    void removeScreen(const ursine::resources::ResourceReference &reference);
} Meta(Enable);

#undef DECLARE_SCREEN
