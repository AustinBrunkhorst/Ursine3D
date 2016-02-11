/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** NotificationConfig.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "NotificationType.h"
#include "NotificationButton.h"

#include "TimeSpan.h"

#include <string>
#include <vector>

namespace ursine
{
    class Notification;

    typedef uint32 NotificationID;
    typedef uint32 NotificationButtonID;
    typedef std::function<void(const Notification &)> NotificationClosedCallback;

    struct NotificationConfig
    {
        // duration to use for an infinite duration 
        // (does not automatically close)
        const TimeSpan InfiniteDuration = 0;

        // determines if this notification is dimissible with
        // the close button in the top right
        bool dismissible = true;

        // type of the notification: determines the icon
        // displayed next to the heading
        NotificationType type { NOTIFY_NONE };

        std::string header;
        std::string message;

        // optional list of buttons to display with the notification
        std::vector<NotificationButton> buttons;

        // duration to be displayed before closing.
        // use "NotificationConfig::InfiniteDuration"
        // to never close automatically
        TimeSpan duration { TimeSpan::FromSeconds( 8 ) };

        // called when the notification has been closed
        NotificationClosedCallback closeCallback { nullptr };

        NotificationConfig(void) = default;
    };

    #if defined(URSINE_WITH_EDITOR)

    class Notification;

    extern Notification EditorPostNotification(const NotificationConfig &config);

    #endif
}
