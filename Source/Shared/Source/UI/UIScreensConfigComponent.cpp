/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** UIConfigComponent.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "UIScreensConfigComponent.h"

#include <Scene.h>

NATIVE_COMPONENT_DEFINITION( UIScreensConfig )

using namespace ursine;
using namespace ecs;

UIScreensConfig::UIScreensConfig(GameManager *manager)
    : BaseComponent( )
    , m_debugMode( false ) 
    , m_debugSegment( LevelSegments_enum::Empty )
    , m_gameManager( manager ) { }

UIScreensConfig::UIScreensConfig(void)
    : BaseComponent( )
    , m_debugMode( false )
    , m_debugSegment( LevelSegments_enum::Empty ) 
    , m_gameManager( nullptr ) { }

const resources::ResourceReference &UIScreensConfig::GetMainMenuWorld(void)
{
    return m_mainMenuWorld;
}

void UIScreensConfig::SetMainMenuWorld(const resources::ResourceReference &world)
{
    m_mainMenuWorld = world;

    NOTIFY_COMPONENT_CHANGED( "mainMenuWorld", m_mainMenuWorld );
}

const resources::ResourceReference &UIScreensConfig::GetStartingGameplayWorld(void)
{
    return m_startingGameplayWorld;
}

void UIScreensConfig::SetStartingGameplayWorld(const resources::ResourceReference &world)
{
    m_startingGameplayWorld = world;

    NOTIFY_COMPONENT_CHANGED( "startingGameplayWorld", m_startingGameplayWorld );
}


bool UIScreensConfig::GetDebugMode(void) const
{
    return m_debugMode;
}

void UIScreensConfig::SetDebugMode(bool debug)
{
    m_debugMode = debug;

    NOTIFY_COMPONENT_CHANGED( "debugMode", m_debugMode );
}

LevelSegments UIScreensConfig::GetDebugSegment(void) const
{
    return m_debugSegment;
}

void UIScreensConfig::SetDebugSegment(LevelSegments segment)
{
    m_debugSegment = segment;

    NOTIFY_COMPONENT_CHANGED( "debugSegment", m_debugSegment );
}

const resources::ResourceReference &UIScreensConfig::GetDebugWorld(void)
{
    return m_debugWorld;
}

void UIScreensConfig::SetDebugWorld(const resources::ResourceReference &world)
{
    m_debugWorld = world;

    NOTIFY_COMPONENT_CHANGED( "debugWorld", m_debugWorld );
}

Scene *UIScreensConfig::getScene(void) const
{
    if (m_gameManager)
        return m_gameManager->GetContext( )->GetScene( );

    return GetOwner( )->GetWorld( )->GetOwner( );
}

UIScreen *UIScreensConfig::addScreen(
    const resources::ResourceReference &reference, 
    const UIScreenConfig &config, 
    const Json &initData
)
{
    auto *scene = getScene( );

    UAssert( scene, 
        "Scene was null." 
    );

    auto *screenData = reference.Load<resources::UIScreenData>( scene->GetResourceManager( ) );

    auto *screen = scene->GetScreenManager( ).CreateScreen( screenData, config, initData );

    if (!screen)
        return nullptr;

    m_created[ reference.GetGUID( ) ] = screen->GetID( );

    return screen;
}

UIScreen *UIScreensConfig::getScreen(const resources::ResourceReference &reference) const
{
    auto *scene = getScene( );

    UAssert( scene, 
        "Scene was null." 
    );

    auto search = m_created.find( reference.GetGUID( ) );

    return search == m_created.end( ) ? nullptr : 
        scene->GetScreenManager( ).GetScreen( search->second );
}

void UIScreensConfig::removeScreen(const resources::ResourceReference &reference)
{
    auto *screen = getScreen( reference );

    if (!screen)
        return;

    screen->Remove( );

    m_created.erase( reference.GetGUID( ) );
}
