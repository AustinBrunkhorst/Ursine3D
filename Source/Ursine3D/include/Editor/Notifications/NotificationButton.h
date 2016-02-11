/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** NotificationButton.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include <string>
#include <functional>

namespace ursine
{
    class Notification;

    struct NotificationButton
    {
        typedef std::function<void(Notification &)> Callback;

        // text to be displayed in the button
        std::string text;

        // callback when the button is clicked
        Callback onClick;
    };
}