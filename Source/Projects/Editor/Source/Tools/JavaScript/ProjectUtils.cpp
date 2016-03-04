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

#include <FileDialog.h>

using namespace ursine;

namespace
{
    void buildResourceDirectory(rp::ResourceDirectoryNode *node, Json::object &obj);
    Json serializeResource(rp::ResourceItem::Handle resource);
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
        if (!resource)
            return nullptr;

        auto &sourceFile = resource->GetSourceFileName( );

        return Json::object {
            { "guid", to_string( resource->GetGUID( ) ) },
            { "type", resource->GetDataType( ).GetName( ) },
            { "displayName", resource->GetDisplayName( ) },
            { "relativePathDisplayName", resource->GetRelativePathDisplayName( ) },
            { "sourceFile", sourceFile.string( ) },
            { "hasPreview", resource->HasPreview( ) },
            { "previewFile", resource->GetPreviewFileName( ).string( ) },
            { "extension", sourceFile.extension( ).string( ) }
        };
    }
}