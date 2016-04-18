/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** EditorUIResourceHandlerFactory.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "EditorUIResourceHandlerFactory.h"

#include "Editor.h"

#include <UIResourceHandler.h>

CefRefPtr<CefResourceHandler> EditorUIResourceHandlerFactory::Create(
    CefRefPtr<CefBrowser> browser, 
    CefRefPtr<CefFrame> frame, 
    const CefString &schemeName, 
    CefRefPtr<CefRequest> request
)
{
    auto *editor = GetCoreSystem( Editor );

    return new ursine::UIResourceHandler(
        ursine::kUIGameResourceDomain,
        &editor->GetProject( )->GetScene( ).GetResourceManager( ) 
    );
}