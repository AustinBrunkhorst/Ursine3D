/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ProjectBuildSettings.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "ProjectBuildSettingsComponent.h"

#include <Editor.h>
#include <Project.h>

using namespace ursine;

NATIVE_COMPONENT_DEFINITION( ProjectBuildSettings );

ProjectBuildSettings::ProjectBuildSettings(void)
    : BaseComponent( ) 
{
    auto *project = GetCoreSystem( Editor )->GetProject( );

    auto &config = project->GetConfig( );

    launchAfterBuild = config.launchAfterBuild;

    auto &settings = config.buildSettings;

    windowTitle = settings.windowTitle;
    windowFullScreen = settings.windowFullScreen;
    windowResizable = settings.windowResizable;

    gameManager = settings.manager;
}

ProjectBuildSettings::~ProjectBuildSettings(void) 
{
    
}

void ProjectBuildSettings::Save(void)
{
    auto *project = GetCoreSystem( Editor )->GetProject( );

    auto &config = project->GetConfig( );

    config.launchAfterBuild = launchAfterBuild;

    auto &settings = config.buildSettings;

    settings.windowTitle = windowTitle;
    settings.windowFullScreen = windowFullScreen;
    settings.windowResizable = windowResizable;
    settings.manager = gameManager;

    auto components = GetOwner( )->GetComponents( );

    Json::object managerConfig;

    // serialize config components
    for (auto *component : components)
    {
        auto type = component->GetType( );

        auto &meta = type.GetMeta( );

        // ignore these guys
        if (meta.GetProperty<HiddenInInspector>( ))
            continue;

        // skip this and the transform
        if (type == decltypeof( *this ) || type == typeof( ecs::Transform ))
            continue;

        managerConfig[ type.GetName( ) ] = type.SerializeJson( ObjectVariant( component ) );
    }

    settings.managerConfiguration = managerConfig;

    project->WriteConfig( );

    NotificationConfig success;

    success.type = NOTIFY_INFO;
    success.header = "Action Complete";
    success.message = "Build settings saved.";
    success.dismissible = true;

    EditorPostNotification( success );
}

void ProjectBuildSettings::OnInitialize(void)
{
    auto *project = GetCoreSystem( Editor )->GetProject( );

    auto &settings = project->GetConfig( ).buildSettings;

    for (auto &entry : settings.managerConfiguration.object_items( ))
    {
        auto componentType = meta::Type::GetFromName( entry.first );

        if (!componentType.IsValid( ) || !componentType.GetBaseClasses( ).count( typeof( Component ) ))
        {
            UWarning( "Unknown or invalid game manager game config type '%s'.",
                entry.first.c_str( )
            );

            continue;
        }

        auto componentID = componentType.GetStaticField( "ComponentID" );

        if (!componentID.IsValid( ))
        {
            UWarning( "Unknown or invalid game manager game config type '%s'.",
                entry.first.c_str( )
            );

            continue;
        }

        ecs::ComponentTypeMask componentTypeMask;

        componentTypeMask.set( componentID.GetValue( ).GetValue<ecs::ComponentTypeID>( ), true );

        auto &owner = GetOwner( );

        if (!owner->HasComponent( componentTypeMask ))
        {
            auto *component = componentType.CreateDynamic( ).GetValue<Component*>( );
            auto instance = ObjectVariant( component );

            componentType.DeserializeJson( instance, entry.second );

            owner->AddComponent( component );
        }
    }
}