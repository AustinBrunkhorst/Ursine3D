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
        ursine::kUIGameResourceScheme,
        &editor->GetProject( )->GetScene( ).GetResourceManager( ) 
    );
}