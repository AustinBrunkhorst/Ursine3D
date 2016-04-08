/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ProjectUtils.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "ProjectUtils.h"
#include "Editor.h"

#include <JSConfig.h>

#include <ResourceItem.h>

#include <FileDialog.h>

using namespace ursine;

namespace
{
    fs::path gLastSelectedLauncherBuildDir;
    fs::path gLastSelectedOutputDir;
    fs::path gLastSelectedInstallExecutable;

    void buildResourceDirectory(rp::ResourceDirectoryNode *node, Json::object &obj);
    Json serializeResource(rp::ResourceItem::Handle resource);
}

JSFunction(ProjectSave)
{
    auto *editor = GetCoreSystem( Editor );

    editor->GetProject( )->WriteConfig( );

	return CefV8Value::CreateBool( true );
}

JSFunction(ProjectBuildStart)
{
    auto *editor = GetCoreSystem( Editor );

    auto &builder = editor->GetProject( )->GetGameBuilder( );

    if (builder.IsBuilding( ))
        return CefV8Value::CreateBool( false );

    fs::FileDialog folderDialog;

    folderDialog.config.mode = fs::FDM_DIRECTORY;
    folderDialog.config.initialPath = gLastSelectedLauncherBuildDir;
    folderDialog.config.windowTitle = "Select Launcher Build Directory";
    folderDialog.config.parentWindow = editor->GetMainWindow( ).GetWindow( );

    auto launcherResult = folderDialog.Open( );

    if (!launcherResult)
        return CefV8Value::CreateBool( false );

    auto &launcherDir = launcherResult.selectedFiles[ 0 ];

    gLastSelectedLauncherBuildDir = launcherDir;

    folderDialog.config.initialPath = gLastSelectedOutputDir;
    folderDialog.config.windowTitle = "Select Output Directory";

    auto outputResult = folderDialog.Open( );

    if (!outputResult)
        return CefV8Value::CreateBool( false );

    auto &outputDir = outputResult.selectedFiles[ 0 ];

    gLastSelectedOutputDir = outputDir;

    builder.Build( launcherDir, outputDir );

	return CefV8Value::CreateBool( true );
}

JSFunction(ProjectBuildCancel)
{
    auto *editor = GetCoreSystem( Editor );

    auto &builder = editor->GetProject( )->GetGameBuilder( );

    if (!builder.IsBuilding( ))
        return CefV8Value::CreateBool( false );

    builder.Cancel( );

	return CefV8Value::CreateBool( true );
}

JSFunction(ProjectInstallStart)
{
    auto *editor = GetCoreSystem( Editor );

    auto &installer = editor->GetProject( )->GetGameInstaller( );

    if (installer.IsBuilding( ))
        return CefV8Value::CreateBool( false );

    fs::FileDialog folderDialog;

    folderDialog.config.mode = fs::FDM_DIRECTORY;
    folderDialog.config.initialPath = gLastSelectedOutputDir;
    folderDialog.config.windowTitle = "Select Build Directory";
    folderDialog.config.parentWindow = editor->GetMainWindow( ).GetWindow( );

    auto buildResult = folderDialog.Open( );

    if (!buildResult)
        return CefV8Value::CreateBool( false );

    auto &buildDir = buildResult.selectedFiles[ 0 ];

    gLastSelectedOutputDir = buildDir;

    fs::FileDialog fileDialog;

    fileDialog.config.mode = fs::FDM_SAVE;
    fileDialog.config.initialPath = gLastSelectedInstallExecutable;
    fileDialog.config.windowTitle = "Select Installer Executable";
    fileDialog.config.filters = { { "Executable Files", { "exe" } } };
    fileDialog.config.parentWindow = editor->GetMainWindow( ).GetWindow( );

    auto outputResult = fileDialog.Open( );

    if (!outputResult)
        return CefV8Value::CreateBool( false );

    auto &outputFile = outputResult.selectedFiles[ 0 ];

    gLastSelectedInstallExecutable = outputFile;

    installer.Build( buildDir, outputFile );

	return CefV8Value::CreateBool( true );
}

JSFunction(ProjectSetFullScreen)
{
    if (arguments.size( ) != 1)
        JSThrow( "Invalid arguments.", nullptr );

    auto fullScreen = arguments[ 0 ]->GetBoolValue( );

    auto *editor = GetCoreSystem( Editor );

    editor->GetPreferences( ).fullScreen = fullScreen;

    // @@@ TODO:
    //editor->GetMainWindow( ).GetWindow( )->SetFullScreen( fullScreen );

    return CefV8Value::CreateBool( true );
}

JSFunction(ProjectGetGlobalPreferences)
{
    auto *editor = GetCoreSystem( Editor );

    auto &prefs = editor->GetPreferences( );

    CefRefPtr<CefV8Value> obj;

    JsonSerializer::Deserialize( meta::Type::SerializeJson( prefs ), obj );

	return obj;
}

JSFunction(ProjectPreferenceStoreLoad)
{
    if (arguments.size( ) < 2)
        JSThrow( "Invalid arguments.", nullptr );

    auto category = arguments[ 0 ]->GetStringValue( );
    auto item = arguments[ 1 ]->GetStringValue( );

    Json defaultValue;

    if (arguments.size( ) == 3)
        defaultValue = JsonSerializer::Serialize( arguments[ 2 ] );

    auto *editor = GetCoreSystem( Editor );

    auto value = editor->GetProject( )->GetPreferenceStore( )
        .Load( category, item, defaultValue );

    CefRefPtr<CefV8Value> obj;

    JsonSerializer::Deserialize( value, obj );

    return obj;
}

JSFunction(ProjectPreferenceStoreStore)
{
    if (arguments.size( ) != 3)
        JSThrow( "Invalid arguments.", nullptr );

    auto category = arguments[ 0 ]->GetStringValue( );
    auto item = arguments[ 1 ]->GetStringValue( );

    auto storedValue = JsonSerializer::Serialize( arguments[ 2 ] );

    auto *editor = GetCoreSystem( Editor );

    editor->GetProject( )->GetPreferenceStore( )
        .Store( category, item, storedValue );

    return CefV8Value::CreateBool( true );
}

JSFunction(ProjectGetName)
{
    auto *editor = GetCoreSystem( Editor );

	return CefV8Value::CreateString( editor->GetProject( )->GetConfig( ).title );
}

JSFunction(ProjectGetResourceTree)
{
    auto *editor = GetCoreSystem( Editor );
    auto *rootNode = editor->GetProject( )->GetResourcePipeline( ).GetRootResourceDirectory( );

    Json::object tree;

    buildResourceDirectory( rootNode, tree );

    CefRefPtr<CefV8Value> object;

    JsonSerializer::Deserialize( tree, object );

    return object;
}

JSFunction(ProjectGetResourcesByType)
{
    if (arguments.size( ) != 1)
        JSThrow( "Invalid arguments.", nullptr );

    auto typeName = arguments[ 0 ]->GetStringValue( ).ToString( );
    auto type = meta::Type::GetFromName( typeName );

    auto *editor = GetCoreSystem( Editor );
    auto items = editor->GetProject( )->GetResourcePipeline( ).GetItemsByType( type );

    Json::array itemsData;

    for (auto &item : items)
        itemsData.emplace_back( serializeResource( item ) );

    CefRefPtr<CefV8Value> object;

    JsonSerializer::Deserialize( itemsData, object );

    return object;
}

JSFunction(ProjectGetResource)
{
    if (arguments.size( ) != 1)
        JSThrow( "Invalid arguments.", nullptr );

    auto *editor = GetCoreSystem( Editor );

    try
    {
        auto guid = GUIDStringGenerator( )( arguments[ 0 ]->GetStringValue( ).ToString( ) );

        auto item = editor->GetProject( )->GetResourcePipeline( ).GetItem( guid );

        CefRefPtr<CefV8Value> object;

        JsonSerializer::Deserialize( serializeResource( item ), object );

        return object;
    }
    catch (...)
    {
        return nullptr;
    }
}

JSFunction(ProjectDeleteResource)
{
    if (arguments.size( ) != 1)
        JSThrow( "Invalid arguments.", nullptr );

    auto *editor = GetCoreSystem( Editor );

    try
    {
        auto guid = GUIDStringGenerator( )( arguments[ 0 ]->GetStringValue( ).ToString( ) );

        auto &pipeline = editor->GetProject( )->GetResourcePipeline( );

        auto item = pipeline.GetItem( guid );

        if (!item)
            return CefV8Value::CreateBool( false );

        pipeline.RemoveItem( item );

        return CefV8Value::CreateBool( true );
    }
    catch (...)
    {
        return CefV8Value::CreateBool( false );
    }
}

JSFunction(ProjectRenameResource)
{
    if (arguments.size( ) != 2)
        JSThrow( "Invalid arguments.", nullptr );

    auto *editor = GetCoreSystem( Editor );

    try
    {
        auto guid = GUIDStringGenerator( )( arguments[ 0 ]->GetStringValue( ).ToString( ) );

        auto &pipeline = editor->GetProject( )->GetResourcePipeline( );

        auto item = pipeline.GetItem( guid );

        if (!item)
            return CefV8Value::CreateBool( false );

        auto newName = arguments[ 1 ]->GetStringValue( ).ToString( );

        return CefV8Value::CreateBool( pipeline.ChangeItemDisplayName( item, newName ) );
    }
    catch (...)
    {
        return CefV8Value::CreateBool( false );
    }
}

JSFunction(ProjectMakeEditorResource)
{
    if (arguments.size( ) != 1)
        JSThrow( "Invalid arguments.", nullptr );

    auto *editor = GetCoreSystem( Editor );

    try
    {
        auto guid = GUIDStringGenerator( )( arguments[ 0 ]->GetStringValue( ).ToString( ) );

        if (editor->GetProject( )->CreateEditorResource( guid ))
            utils::CopyToClipboard( to_string( guid ) );
    }
    catch (...)
    {
        // do nothing
    }

    return nullptr;
}

JSFunction(ProjectSetEmptyScene)
{
    auto *editor = GetCoreSystem( Editor );

    editor->GetProject( )->SetEmptyScene( );

	return CefV8Value::CreateBool( true );
}

JSFunction(ProjectOpenNew)
{
    auto *editor = GetCoreSystem( Editor );

    editor->GetProject( )->SetEmptyScene( );

    fs::FileDialog openDialog;

    openDialog.config.mode = fs::FDM_OPEN;
    openDialog.config.initialPath = "";
    openDialog.config.windowTitle = "Open Project";
    openDialog.config.parentWindow = editor->GetMainWindow( ).GetWindow( );
    openDialog.config.filters = {
        { "Ursine3D Projects", { "*.ursineproj" } }
    };

    auto result = openDialog.Open( );

    if (result)
    {
        Application::PostMainThread( [=] {
            editor->LoadProject( result.selectedFiles[ 0 ].string( ) );
        } );
    }

	return CefV8Value::CreateBool( true );
}

namespace
{
    void buildResourceDirectory(rp::ResourceDirectoryNode *node, Json::object &obj)
    {
        obj[ "path" ] = node->GetPath( ).string( );
        obj[ "displayName" ] = node->GetPath( ).filename( ).string( );

        Json::array subDirectories;

        for (auto *subDirectory : node->GetSubDirectories( ))
        {
            Json::object dirObj;

            buildResourceDirectory( subDirectory, dirObj );

            subDirectories.push_back( dirObj );
        }

        obj[ "subDirectories" ] = subDirectories;

        Json::array resources;

        for (auto &resource : node->GetResources( ))
            resources.emplace_back( serializeResource( resource ) );

        obj[ "resources" ] = resources;
    }

    Json serializeResource(rp::ResourceItem::Handle resource)
    {
        return JsonSerializer::Serialize<const rp::ResourceItem::Handle>( resource );
    }
}