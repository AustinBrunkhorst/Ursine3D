/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
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
    class ResourceBundleHandler : public CefResourceBundleHandler
    {
    private:
        bool GetLocalizedString(
            int messageID, 
            CefString &string
        ) override;

        bool GetDataResource(
            int resourceID, 
            void *&data, 
            size_t &dataSize
        ) override;

        bool GetDataResourceForScale(
            int resourceID, 
            ScaleFactor scaleFactor,
            void *&data,
            size_t &dataSize
        ) override;
    };
}