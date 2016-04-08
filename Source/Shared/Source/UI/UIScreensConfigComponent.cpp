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

NATIVE_COMPONENT_DEFINITION( UIScreensConfig );

using namespace ursine;
using namespace ecs;

UIScreensConfig::UIScreensConfig(void)
    : BaseComponent( ) { }


const resources::ResourceReference &UIScreensConfig::GetStartingGameplayWorld(void)
{
    return m_startingGameplayWorld;
}

void UIScreensConfig::SetStartingGameplayWorld(const resources::ResourceReference &world)
{
    m_startingGameplayWorld = world;

    NOTIFY_COMPONENT_CHANGED( "startingGameplayWorld", m_startingGameplayWorld );
}

UIScreen *UIScreensConfig::addScreen(
    const resources::ResourceReference &reference, 
    const UIScreenConfig &config, 
    const Json &initData
)
{
    auto *scene = GetOwner( )->GetWorld( )->GetOwner( );

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
    auto *scene = GetOwner( )->GetWorld( )->GetOwner( );

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
