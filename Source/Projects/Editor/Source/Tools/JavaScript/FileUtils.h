/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** FileUtils.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <NativeJSFunction.h>
#include <UIFileDialogCallback.h>

void RunFileDialog(
    CefBrowserHost::FileDialogMode mode, 
    const std::string &title,
    const ursine::fs::path &defaultPath,
    const std::vector<CefString> &filters, 
    ursine::UIFileDialogCallback::Callback callback
);

Meta(Enable, ExposeJavaScript)
JSFunction(BrowseFile);

Meta(Enable, ExposeJavaScript)
JSFunction(BrowseFolder);