#pragma once

#include "NotificationType.h"
#include "NotificationButton.h"

#include "TimeSpan.h"

#include <string>
#include <vector>

namespace ursine
{
    typedef uint32 NotificationID;
    typedef uint32 NotificationButtonID;

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
        std::function<void(void)> closeCallback { nullptr };

        NotificationConfig(void) = default;
    };

    #if defined(URSINE_WITH_EDITOR)

    class Notification;

    extern Notification EditorPostNotification(const NotificationConfig &config);

    #endif
}
