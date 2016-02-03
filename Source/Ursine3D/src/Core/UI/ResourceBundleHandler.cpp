/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
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
    bool ResourceBundleHandler::GetLocalizedString(
        int messageID,
        CefString &string
    )
    {
        return false;
    }

    bool ResourceBundleHandler::GetDataResource(
        int resourceID,
        void *&data, 
        size_t &dataSize
    )
    {
        return false;
    }

    bool ResourceBundleHandler::GetDataResourceForScale(
        int resourceID, 
        ScaleFactor scaleFactor, 
        void *&data, 
        size_t & dataSize
    )
    {
        return false;
    }
}