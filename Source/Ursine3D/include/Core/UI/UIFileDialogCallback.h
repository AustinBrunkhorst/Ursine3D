#pragma once

#include "FileSystem.h"

namespace ursine
{
    class UIFileDialogCallback : public CefRunFileDialogCallback
    {
    public:
        typedef std::function<void(int, std::vector<fs::path>)> Callback;

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