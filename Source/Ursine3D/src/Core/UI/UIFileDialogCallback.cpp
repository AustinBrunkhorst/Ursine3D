#include "UrsinePrecompiled.h"

#include "UIFileDialogCallback.h"

namespace ursine
{
    UIFileDialogCallback::UIFileDialogCallback(Callback callback)
        : m_callback( callback )
    {
        
    }

    void UIFileDialogCallback::OnFileDialogDismissed(
        int selectedAcceptFilter, 
        const std::vector<CefString> &paths
    )
    {
        if (m_callback) 
        {
            std::vector<fs::path> output;

            for (auto &path : paths)
                output.emplace_back( path.ToString( ) );

            m_callback( selectedAcceptFilter, output );
        }
    }
}