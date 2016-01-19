/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** NotificationType.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

namespace ursine
{
    enum NotificationType
    {
        NOTIFY_NONE,
        NOTIFY_WARNING,
        NOTIFY_ERROR,
        NOTIFY_INFO,
        NOTIFY_QUESTION
    };
}