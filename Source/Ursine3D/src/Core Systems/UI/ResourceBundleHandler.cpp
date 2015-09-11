/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ResourceBundleHandler.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/


#include "UrsinePrecompiled.h"

#include "ResourceBundleHandler.h"

namespace ursine
{
    namespace ui
    {
        bool ResourceBundleHandler::GetLocalizedString(int message_id,
            CefString &string)
        {
            return false;
        }

        bool ResourceBundleHandler::GetDataResource(int resource_id,
            void *&data, size_t &data_size)
        {
            return false;
        }
    }
}