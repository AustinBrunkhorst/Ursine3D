/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** UIFileDialogCallback.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "FileSystem.h"

namespace ursine
{
    class UIFileDialogCallback : public CefRunFileDialogCallback
    {
    public:
        typedef std::function<void(int, const fs::FileList&)> Callback;

        UIFileDialogCallback(Callback callback);

    private:
        Callback m_callback;

        void OnFileDialogDismissed(
            int selectedAcceptFilter,
            const std::vector<CefString> &paths
        ) override;

        IMPLEMENT_REFCOUNTING( UIFileDialogCallback );
    };
}