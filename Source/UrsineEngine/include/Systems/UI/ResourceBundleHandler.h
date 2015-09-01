/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ResourceBundleHandler.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include <cef_app.h>

namespace ursine
{
    namespace UI
    {
        class ResourceBundleHandler : public CefResourceBundleHandler
        {
            bool GetLocalizedString(int message_id, CefString &string) override;

            bool GetDataResource(int resource_id, void *&data, 
                size_t &data_size) override;
        };
    }
}