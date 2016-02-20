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

using namespace ursine;

namespace
{
    void buildResourceDirectory(rp::ResourceDirectoryNode *node, Json::object &obj);
    Json serializeResource(rp::ResourceItem::Handle resource);
}

JSFunction(ProjectGetName)
{
    auto *editor = GetCoreSystem( Editor );

	return CefV8Value::CreateString( editor->GetProject( ).GetConfig( ).title );
}

JSFunction(ProjectGetResourceTree)
{
    auto *editor = GetCoreSystem( Editor );
    auto *rootNode = editor->GetProject( ).GetResourcePipeline( ).GetRootResourceDirectory( );

    Json::object tree;

    buildResourceDirectory( rootNode, tree );

    CefRefPtr<CefV8Value> object;

    JsonSerializer::Deserialize( tree, object );

    return object;
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
        auto &sourceFile = resource->GetSourceFileName( );

        return Json::object {
            { "guid", to_string( resource->GetGUID( ) ) },
            { "displayName", sourceFile.filename( ).string( ) },
            { "sourceFile", sourceFile.string( ) },
            { "extension", sourceFile.extension( ).string( ) }
        };
    }
}