/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** UIFileDialogCallback.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

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
            fs::FileList output;

            for (auto &path : paths)
                output.emplace_back( path.ToString( ) );

            m_callback( selectedAcceptFilter, output );
        }
    }
}